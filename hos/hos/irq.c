#include "asm/mipsregs.h"
#include "asm/addrspace.h"
#include "errno.h"
#include "irq.h"
#include "types.h"
#include "2k1000.h"
#define UART16550BASE 0xb80003f8


int irqdepth;
int ls_msi_irq_mask[2];

static int no_action(int cpl, void *dev_id)
{
	return 0;
}

static struct irqaction cascade_irqaction = {
	.handler	= no_action,
	.name		= "cascade",
};

struct irq_desc irq_desc[256];
int handle_IRQ_event(unsigned int irq,struct irqaction *action)
{
	do {
		action->handler(irq, action->dev_id);
		action = action->next;
	}while (action);
}

void handle_level_irq(unsigned int irq,struct irq_desc *desc)
{
	struct irqaction *action;
	desc->chip->mask(irq);
	action = desc->action;
	handle_IRQ_event(irq,action);
	desc->chip->unmask(irq);
}

void generic_handle_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_desc + irq;
	desc->handle_irq(irq,desc);
}

#define do_IRQ(irq) do{	generic_handle_irq(irq);}while(0)

void __set_irq_handler(unsigned int irq, irq_flow_handler_t handler, int is_chained, const char *name)
{
	struct irq_desc *desc;
	desc = irq_desc + irq;
	desc->handle_irq = handler;
	
}

void default_enable(unsigned int irq)
{
	struct irq_desc *desc = irq_desc + irq;
	desc->chip->unmask(irq);
}

void default_disable(unsigned int irq)
{
	
}

unsigned int default_startup(unsigned int irq)
{
	irq_desc[irq].chip->enable(irq);
	return 0;
}

void irq_chip_set_defaults(struct irq_chip *chip)
{
	if(!chip->enable)
		chip->enable = default_enable;
	if(!chip->disable)
		chip->disable = default_disable;
	if(!chip->startup)
		chip->startup = default_startup;
	if(!chip->shutdown)
		chip->shutdown = chip->disable;
}

int set_irq_chip(unsigned int irq,struct irq_chip *chip)
{
	struct irq_desc *desc;
	desc = irq_desc + irq;
	irq_chip_set_defaults(chip);
	desc->chip = chip;
	return 0;
}

void set_irq_chip_and_handler(unsigned int irq,struct irq_chip *chip,irq_flow_handler_t handle)
{
	set_irq_chip(irq, chip);
	__set_irq_handler(irq, handle, 0, NULL);
}

int setup_irq(unsigned int irq, struct irqaction *newaction)
{
	struct irq_desc *desc = irq_desc + irq;
	struct irqaction *old, **p;
	int shared = 0;
	p = &desc->action;
	old = *p;
	if(old)
	{
		do
		{
			p = &old->next;
			old= *p;
		}while(old);
		shared = 1;
	}
	*p = newaction;
	if(!shared)	
	{
		irq_chip_set_defaults(desc->chip);
		if(newaction->flags & IRQF_TRIGGER_MASK)
			if(desc->chip && desc->chip->set_type)
				desc->chip->set_type(irq,newaction->flags & IRQF_TRIGGER_MASK);
		if(desc->chip->startup)
			desc->chip->startup(irq);
		else
			desc->chip->enable(irq);
	}
	
	return 0;
	
	
}

int request_irq(unsigned int irq, irq_handler_t handler, unsigned long irqflags,const char *devname, void *dev_id)
{
	struct irqaction *action;
	int retval;
	if(!handler)
		return -EINVAL;
	action = malloc(sizeof(struct irqaction));
	if(!action)
		return -ENOMEM;
	action->handler = handler;
	action->flags = irqflags;
	action->next = NULL;
	action->name = devname;
	action->dev_id = dev_id;
	action->irq = irq;
	setup_irq(irq,action);
}


void enable_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_desc + irq;
	desc->chip->unmask(irq);
}

void disable_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_desc + irq;
	desc->chip->mask(irq);
}

///////interrupt
#define INT_PCI_INTA (1<<6)
#define INT_PCI_INTB (1<<7)
#define INT_PCI_INTC (1<<8)
#define INT_PCI_INTD (1<<9)
#define ST0_IM  0x0000ff00
#define CAUSEF_IP7      ( 1 << 15)
#define CAUSEF_IP6      ( 1 << 14)
#define CAUSEF_IP5      ( 1 << 13)
#define CAUSEF_IP4      ( 1 << 12)                                                                                                                     
#define CAUSEF_IP3      ( 1 << 11)
#define CAUSEF_IP2      ( 1 << 10)

void ls64_ipi_interrupt(struct pt_regs *regs)
{
}

void ls_unmask_cpu_irq(unsigned int irq)
{
	unsigned int index;
	unsigned long base;

	index = irq - CPU_IRQ_BASE;
	write_c0_status(read_c0_status()|(0x100<<index));
}

void ls_mask_cpu_irq(unsigned int irq)
{
	unsigned int index;
	unsigned long base;

	index = irq - CPU_IRQ_BASE;
	write_c0_status(read_c0_status()&~(0x100<<index));
}

static struct irq_chip cpu_irq_chip = {
	.name		= "cpu",
	.unmask	= ls_unmask_cpu_irq,
	.mask	= ls_mask_cpu_irq,
};

void plat_irq_dispatch(struct pt_regs *regs)
{
        unsigned int cause = read_c0_cause() & ST0_IM;
        unsigned int status = read_c0_status() & ST0_IM;
        unsigned int pending = cause & status;
	if (pending & (CAUSEF_IP4 | CAUSEF_IP5)) {
	int i = (read_c0_ebase() & 0x3ff);
	unsigned long addr = CKSEG1ADDR(CONF_BASE);
	int hi = ls64_conf_read32((void*)(addr + INTSR1_OFF + (i << 8)));
	int lo = ls64_conf_read32((void*)(addr + INTSR0_OFF + (i << 8)));

	if (pending & CAUSEF_IP4) {
		int enl = ls64_conf_read32((void*)(addr + INT_LO_OFF + INT_EN_OFF));
		int enh = ls64_conf_read32((void*)(addr + INT_HI_OFF + INT_EN_OFF));
		lo = (enl & lo & ~ls_msi_irq_mask[0]);
		hi = (enh & hi & ~ls_msi_irq_mask[1]);

		if(!hi && !lo)
		{
			printf("Unknow interrpt intstatus 0x%08x%08x\n", hi, lo);
		}
		else if(lo)
		{
			do_IRQ(ffs(lo) -1 + LS2K_IRQ_BASE);
		}
		else
		{
		
			do_IRQ(ffs(hi) -1 + LS2K_IRQ_BASE + 32);
		}

	}
	else if (pending & CAUSEF_IP5) {
		lo = lo & ls_msi_irq_mask[0];
		hi = hi & ls_msi_irq_mask[1];

		if(!hi && !lo)
		{
			printf("Unknow interrpt intstatus 0x%08x%08x\n", hi, lo);
		}
		else if(lo)
		{
			do_IRQ(ffs(lo) -1 + LS64_MSI_IRQ_BASE);
		}
		else
		{
		
			do_IRQ(ffs(hi) -1 + LS64_MSI_IRQ_BASE + 32);
		}
	}
	}
        else if(pending & CAUSEF_IP7)
        {
		do_IRQ(CPU_IRQ_BASE+7);
        }
	else if (pending & CAUSEF_IP6) {
		ls64_ipi_interrupt(regs);
	}

}


void ls_unmask_icu_irq(unsigned int irq)
{
	unsigned int index;
	unsigned long base;

	index = irq - LS2K_IRQ_BASE;
	base = CKSEG1ADDR(CONF_BASE) + (index >= 32) * 0x40 + INT_LO_OFF ;
	ls64_conf_write32(1 << (index & 0x1f), (void *)(base + INT_SET_OFF));
}

void ls_mask_icu_irq(unsigned int irq)
{
	unsigned int index;
	unsigned long base;

	index = irq - LS2K_IRQ_BASE;
	base = CKSEG1ADDR(CONF_BASE) + (index >= 32) * 0x40 + INT_LO_OFF;
	ls64_conf_write32(1 << (index & 0x1f), (void *)(base + INT_CLR_OFF));
}

int ls_set_type(unsigned int irq,unsigned int flow_type)
{
	int mode;
	unsigned int index;
	unsigned long base;
	unsigned int pole, edge;

	if (flow_type & IRQF_TRIGGER_PROBE)
		return 0;
	switch (flow_type & IRQF_TRIGGER_MASK) {
	case IRQF_TRIGGER_RISING:	mode = 3;	break;
	case IRQF_TRIGGER_FALLING:	mode = 2;	break;
	case IRQF_TRIGGER_HIGH:	mode = 1;	break;
	case IRQF_TRIGGER_LOW:	mode = 0;	break;
	default:
		return -EINVAL;
	}

	index = irq - LS2K_IRQ_BASE;
	base = CKSEG1ADDR(CONF_BASE) + (index >= 32) * 0x40 + INT_LO_OFF ;

	pole = ls64_conf_read32((void *)(base + INT_PLE_OFF));
	edge = ls64_conf_read32((void *)(base + INT_EDG_OFF));

	if(mode & 1)
		pole &= ~(1 << (index & 0x1f));
	else
		pole |= 1 << (index & 0x1f);

	if(mode & 2)
		edge |= 1 << (index & 0x1f);
	else
		edge &= ~(1 << (index & 0x1f));

//printf("irq %d type 0x%x pole 0x%x edge 0x%x\n", irq, flow_type, pole, edge);
	ls64_conf_write32(pole, (void *)(base + INT_PLE_OFF));
	ls64_conf_write32(edge, (void *)(base + INT_EDG_OFF));

return 0;
}

static struct irq_chip ls64_irq_chip = {
	.name		= "ls64soc",
	.unmask	= ls_unmask_icu_irq,
	.mask	= ls_mask_icu_irq,
	.set_type = ls_set_type,
};

void  setup_irq_default(void)
{
	unsigned int i;
	int core_id = 0;

	for (i = LS2K_IRQ_BASE; i < LS2K_IRQ_BASE + 64; i++) {
		set_irq_chip_and_handler(i, &ls64_irq_chip, handle_level_irq);
	}

	set_irq_chip_and_handler(CPU_IRQ_BASE+7, &cpu_irq_chip, handle_level_irq);
}


void set_irq_attr(int irq, unsigned int imask, unsigned int core_mask, int mode)
{
	unsigned int index;
	unsigned long base;
	unsigned int ret;
	int hi;
	int au;
	int bounce;

	if (irq >= LS64_MSI_IRQ_BASE)
		index = irq - LS64_MSI_IRQ_BASE;
	else
		index = irq - LS2K_IRQ_BASE;

	hi = (index >= 32);
	index = index & 0x1f;
	au = (mode & 0x1);
	bounce = ((mode >> 1) & 0x1);
	base = CKSEG1ADDR(CONF_BASE) + INT_LO_OFF + hi * 0x40;

	ls64_conf_write8(imask << 4 | core_mask, (void *)(base + index));

	ret = ls64_conf_read32((void*)(base + INT_BCE_OFF)) & ~(1<<index);
	ret |= (bounce << index);
	ls64_conf_write32(ret, (void *)(base + INT_BCE_OFF));

	ret = ls64_conf_read32((void*)(base + INT_AUTO_OFF)) & ~(1<<index);
	ret |= (au << index);
	ls64_conf_write32(ret, (void *)(base + INT_AUTO_OFF));
}

void init_IRQ()
{
	putchar('3');
	extern char except_vector_base[];
	write_c0_ebase(&except_vector_base);
	putchar('4');
	setup_irq_default();
	putchar('5');
	//clear_c0_status(ST0_IM|ST0_BEV|ST0_EXL|ST0_ERL);
	//set_c0_status(STATUSF_IP4 | STATUSF_IP5 | STATUSF_IP6);
	putchar('6');
	putchar('7');
	putchar('8');
	//write_c0_status(0x7f01);
	//set_c0_status(0x7f01); /*diable timer irq*/
	putchar('7');	
	printf("ebase=0x%x\n", read_c0_ebase());
	printf("sr=0x%x\n", read_c0_status());
}


int handle_int(struct pt_regs *regs)
{
irqdepth++;
plat_irq_dispatch(&regs);
irqdepth--;
return 0;
}
