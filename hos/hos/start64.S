#include "asm/regdef.h"
#include "asm/asm.h"
#include "asm/context.h"

#define	PRINTSTR(x) \
	.rdata;98: .asciz x; .text; dla a0, 98b; bal stringserial; nop

.text;
.global except_vector_base;
except_vector_base:
tlb_exception:
/*ls2k does not use tlb, use xtlb*/
	.dword 0
	.dword 0
	.dword 0
	.dword 0
#if 0
	dla	a0, v200_msg
	bal	stringserial
	nop
	b	exc_common
	nop
#endif
	.align 7;
xtlb_exception:
	dla	a0, v200_msg
	bal	stringserial
	nop
	b	exc_common
	nop
	.align 8;
cache_exception:
	dla	a0, v100_msg
	bal	stringserial
	nop
	b	exc_common
	nop

	.align 7;
generic_exception:
	mfc0	k0, c0_cause
	and		k0, 0x7c
	beqz	k0,is_int
	nop
is_except:
	dla	a0, v380_msg
	bal	stringserial
	nop
	b	exc_common
	nop

	.align 8;

.set noreorder;
.global _start;
_start:
dla t0,uboot_end_data;
dla t1,uboot_end;
3:
sw zero,(t0);
bne t0,t1,3b;
daddu t0,4;

dla sp, _stack
b mymain
nop


exc_common:
	PRINTSTR("\r\nCAUSE=")
	mfc0	a0, c0_cause
	bal	hexserial
	nop
	PRINTSTR("\r\nSTATUS=")
	mfc0	a0, c0_status
	bal	hexserial
	nop
	PRINTSTR("\r\nERRORPC=")
	mfc0	a0, c0_errorepc
	bal	hexserial
	nop
	PRINTSTR("\r\nEPC=")
	mfc0	a0, c0_epc
	bal	hexserial
	nop

	PRINTSTR("\r\nBADADDR=")
	mfc0	a0, c0_badvaddr
	bal	hexserial
	nop

1:
	b 1b
	nop

is_int:
	PTR_MFC0 k0, c0_epc
	STORE_REG_RET(k0)
	CLI()
	move a0,sp
	ADDIU sp,-8
	dla k0, handle_int
	jalr k0
	nop
	ADDIU sp,8
	RESTORE_REG_ERET()
	eret


LEAF(stringserial)
	move	a2, ra
	move	a1, a0
	lbu	a0, 0(a1)
1:
	beqz	a0, 2f
	nop
	bal	tgt_putchar
	addiu	a1, 1
	b	1b
	lbu	a0, 0(a1)

2:
	j	a2
	nop
END(stringserial)


LEAF(hexserial)
	nop
	move	a2, ra
	move	a1, a0
	li	a3, 7
1:
	rol	a0, a1, 4
	move	a1, a0
	and	a0, 0xf
	dla	v0, hexchar
	daddu	v0, a0
	bal	tgt_putchar
	lbu	a0, 0(v0)

	bnez	a3, 1b
	addu	a3, -1

	j	a2
	nop
END(hexserial)

LEAF(tgt_putchar)
	nop
	dla	v0, COM1_BASE_ADDR
1:
	lbu	v1, 5(v0)
	and	v1, 0x20
	beqz	v1, 1b
	nop

	sb	a0, 0(v0)
	j	ra
	nop	
END(tgt_putchar)

LEAF(tgt_getchar)
	dla	v0, COM1_BASE_ADDR
1:
	lbu	v1, 5(v0)
	and	v1, 1
	beqz	v1, 1b
	nop
	lb	v0, 0(v0)
	jr ra
	nop
END(tgt_getchar)

LEAF(tgt_testchar)
	dla	v0, COM1_BASE_ADDR
	lbu	v0, 5(v0)
	and	v0, 1
	jr ra
	nop
END(tgt_testchar)

	.rdata
v100_msg:
	.asciz	"\r\nPANIC! Unexpected Cache error!\r\n"
v200_msg:
	.asciz	"\r\nPANIC! Unexpected TLB refill exception!\r\n"
v380_msg:
	.asciz	"\r\nPANIC! Unexpected General exception!\r\n"
hexchar:
	.ascii	"0123456789abcdef"

