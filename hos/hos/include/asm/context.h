/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CONTEXT.h                                               *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS 4Kc (32 bit RISC) - ATLAS board                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   Context switch macros.                                                *
*                                                                         *
**************************************************************************/


#ifndef __CONTEXT_H__
#define __CONTEXT_H__


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

#ifdef __ASSEMBLER__
#if (_MIPS_SZPTR == 32)
#define DD 1
#define PTR_MTC0 mtc0
#define PTR_MFC0 mfc0
#else
#define DD 2
#define PTR_MTC0 dmtc0
#define PTR_MFC0 dmfc0
#endif

// push context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define STORE_REG_RET(Retaddr) \
    .set noat; \
    .set noreorder; \
        PTR_SUBU sp,120*DD; \
        PTR_S ra,0*DD(sp); \
        PTR_S fp,4*DD(sp); \
        PTR_S gp,8*DD(sp); \
        PTR_S t9,12*DD(sp); \
        PTR_S t8,16*DD(sp); \
        PTR_S s7,20*DD(sp); \
        PTR_S s6,24*DD(sp); \
        PTR_S s5,28*DD(sp); \
        PTR_S s4,32*DD(sp); \
        PTR_S s3,36*DD(sp); \
        PTR_S s2,40*DD(sp); \
        PTR_S s1,44*DD(sp); \
        PTR_S s0,48*DD(sp); \
        PTR_S t7,52*DD(sp); \
        PTR_S t6,56*DD(sp); \
        PTR_S t5,60*DD(sp); \
        PTR_S t4,64*DD(sp); \
        PTR_S t3,68*DD(sp); \
        PTR_S t2,72*DD(sp); \
        PTR_S t1,76*DD(sp); \
        PTR_S t0,80*DD(sp); \
        PTR_S a3,84*DD(sp); \
        PTR_S a2,88*DD(sp); \
        PTR_S a1,92*DD(sp); \
        PTR_S a0,96*DD(sp); \
        PTR_S v1,100*DD(sp); \
        PTR_S v0,104*DD(sp); \
        PTR_S AT,108*DD(sp); \
        mfc0 t0,c0_status; \
        PTR_S t0,112*DD(sp); \
        PTR_S Retaddr,116*DD(sp); \
    .set at

// pop context (normal execution): at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_RET() \
    .set noat; \
    .set noreorder; \
        PTR_L ra,0*DD(sp); \
        PTR_L fp,4*DD(sp); \
        PTR_L gp,8*DD(sp); \
        PTR_L t9,12*DD(sp); \
        PTR_L t8,16*DD(sp); \
        PTR_L s7,20*DD(sp); \
        PTR_L s6,24*DD(sp); \
        PTR_L s5,28*DD(sp); \
        PTR_L s4,32*DD(sp); \
        PTR_L s3,36*DD(sp); \
        PTR_L s2,40*DD(sp); \
        PTR_L s1,44*DD(sp); \
        PTR_L s0,48*DD(sp); \
        PTR_L t7,52*DD(sp); \
        PTR_L t6,56*DD(sp); \
        PTR_L t5,60*DD(sp); \
        PTR_L t4,64*DD(sp); \
        PTR_L t3,68*DD(sp); \
        PTR_L t2,72*DD(sp); \
        PTR_L t1,76*DD(sp); \
        PTR_L t0,80*DD(sp); \
        PTR_L a3,84*DD(sp); \
        PTR_L a2,88*DD(sp); \
        PTR_L a1,92*DD(sp); \
        PTR_L a0,96*DD(sp); \
        PTR_L v1,100*DD(sp); \
        PTR_L v0,104*DD(sp); \
        PTR_L AT,108*DD(sp); \
        PTR_L k0,112*DD(sp); \
        mtc0 k0,c0_status; \
        PTR_L k0,116*DD(sp); \
        PTR_ADDU sp,120*DD; \
        jr k0; \
        nop; \
    .set at

// pop context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_ERET() \
    .set noat; \
    .set noreorder; \
        PTR_L ra,0*DD(sp); \
        PTR_L fp,4*DD(sp); \
        PTR_L gp,8*DD(sp); \
        PTR_L t9,12*DD(sp); \
        PTR_L t8,16*DD(sp); \
        PTR_L s7,20*DD(sp); \
        PTR_L s6,24*DD(sp); \
        PTR_L s5,28*DD(sp); \
        PTR_L s4,32*DD(sp); \
        PTR_L s3,36*DD(sp); \
        PTR_L s2,40*DD(sp); \
        PTR_L s1,44*DD(sp); \
        PTR_L s0,48*DD(sp); \
        PTR_L t7,52*DD(sp); \
        PTR_L t6,56*DD(sp); \
        PTR_L t5,60*DD(sp); \
        PTR_L t4,64*DD(sp); \
        PTR_L t3,68*DD(sp); \
        PTR_L t2,72*DD(sp); \
        PTR_L t1,76*DD(sp); \
        PTR_L t0,80*DD(sp); \
        PTR_L a3,84*DD(sp); \
        PTR_L a2,88*DD(sp); \
        PTR_L a1,92*DD(sp); \
        PTR_L a0,96*DD(sp); \
        PTR_L v1,100*DD(sp); \
        PTR_L v0,104*DD(sp); \
        PTR_L AT,108*DD(sp); \
        PTR_L k0,112*DD(sp); \
        mtc0 k0,c0_status; \
        PTR_L k0,116*DD(sp); \
        PTR_MTC0 k0,c0_epc; \
        PTR_ADDU sp,120*DD; \
        eret; \
        nop; \
    .set at

#define ST0_CU0			0x10000000
#define STATMASK 0x1f

#define CLI() \
		mfc0	t0, c0_status; \
		li	t1, ST0_CU0 | STATMASK; \
		or	t0, t1; \
		xori	t0, STATMASK; \
		mtc0	t0, c0_status; \


#else
struct pt_regs {
	long ra;
	long fp;
	long gp;
	long t9;
	long t8;
	long s7;
	long s6;
	long s5;
	long s4;
	long s3;
	long s2;
	long s1;
	long s0;
	long t7;
	long t6;
	long t5;
	long t4;
	long t3;
	long t2;
	long t1;
	long t0;
	long a3;
	long a2;
	long a1;
	long a0;
	long v1;
	long v0;
	long AT;
	long status;
	long epc;
};
#endif /* _ASSEMBLER_ */


/* ********************************************************************* */
/* Interface function definition */


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__CONTEXT_H__*/
