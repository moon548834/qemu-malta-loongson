#ifndef __MYIRQ_H__
#include <asm/mipsregs.h>
#define __MYIRQ_H__
#define IRQF_SHARED		0x00000080
#define SA_SHIRQ		IRQF_SHARED
#define IRQF_TRIGGER_NONE	0x00000000
#define IRQF_TRIGGER_RISING 	0x00000001
#define IRQF_TRIGGER_FALLING 	0x00000002
#define IRQF_TRIGGER_HIGH	0x00000004
#define IRQF_TRIGGER_LOW	0x00000008
#define IRQF_TRIGGER_MASK 	(IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_TRIGGER_HIGH | IRQF_TRIGGER_LOW)
#define IRQF_TRIGGER_PROBE	0x00000010


#define MIPS_CPU_IRQ_BASE 0
#define NR_IRQS 192

enum LS2K_ICU {
	CPU_IRQ_BASE = 0,
	LS2K_IRQ_BASE = 8,

	/* 8 */
	UART_I0 = LS2K_IRQ_BASE,
	UART_I1,
	UART_I2,
	E1_I,
	HDA_I,
	I2S_I,
	AC97_I,
	THEN_I,

	/* 16 */
	TOY_TICK_I,
	RTC_TICK_I,
	RESERVED_I0,
	RESERVED_I1,
	GMAC0_I0,
	GMAC0_I1,
	GMAC1_I0,
	GMAC1_I1,

	/* 24 */
	CAN0_I,
	CAN1_I,
	BOOT_I,
	SATA_I,
	NAND_I,
	HPET_I,
	I2C0_I,
	I2C1_I,

	/* 32 */
	PWM0_I,
	PWM1_I,
	PWM2_I,
	PWM3_I,
	DC_I,
	GPU_I,
	RESERVED_I3,
	SDIO_I,

	/* 40 */
	PCI0_I0,
	PCI0_I1,
	PCI0_I2,
	PCI0_I3,
	PCI1_I0,
	PCI1_I1,
	HPET1_I,
	HPET2_I,

	/* 48 */
	TOY_I0,
	TOY_I1,
	TOY_I2,
	TOY_I3,
	DMA_I0,
	DMA_I1,
	DMA_I2,
	DMA_I3,

	/* 56 */
	DMA_I4,
	OTG_I,
	EHCI_I,
	OHCI_I,
	RTC_I0,
	RTC_I1,
	RTC_I2,
	RSA_I,

	/* 64 */
	AES_I,
	DES_I,
	GPIO_INT_LO_I,
	GPIO_INT_HI_I,
	GPIO_INT0,
	GPIO_INT1,
	GPIO_INT2,
	GPIO_INT3,

	LS64_MSI_IRQ_BASE = 128,
	LS64_IRQ_LAST = NR_IRQS - 1,
};



struct irq_desc;
typedef void (*irq_flow_handler_t)(unsigned int irq,struct irq_desc *desc);
typedef int  (*irq_handler_t)(int, void *);
struct irq_chip{
		char *name;
		unsigned int (*startup)(unsigned int irq);
		unsigned int (*shutdown)(unsigned int irq);
		unsigned int (*enable)(unsigned int irq);
		unsigned int (*disable)(unsigned int irq);
		
		
		unsigned int (*ack)(unsigned int irq);
		unsigned int (*mask)(unsigned int irq);
		unsigned int (*mask_ack)(unsigned int irq);
		unsigned int (*unmask)(unsigned int irq);
		unsigned int (*eoi)(unsigned int irq);

		void (*end)(unsigned int irq);
		int (*set_type)(unsigned int irq,unsigned int flow_type);
		};

struct irqaction{
		irq_handler_t handler;
		unsigned long flags;
		const char* name;
		struct irqaction *next;
		int irq;
		void *dev_id;
		};

struct	irq_desc{
		irq_flow_handler_t handle_irq;
		struct irq_chip *chip;
		struct irqaction *action;
		};

static inline int fls(int word)
{
	__asm__(".set mips32");
	__asm__("clz %0, %1" : "=r" (word) : "r" (word));
	__asm__(".set mips0");
	return 32-word;
}

static int inline ffs(int word)
{
	if(!word)
		return 0;
	return fls(word & -word);
}
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long irqflags,const char *devname, void *dev_id);
#define IRQ_RETVAL(x)	((x) != 0)
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};
#define ETIME 62
typedef long long cycle_t;
static int smp_processor_id()
{
return (read_c0_ebase() & 0x3ff);
}

void enable_irq(unsigned int irq);
#endif
