/* md5.c - an implementation of the MD5 algorithm and MD5 crypt */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000, 2001  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* See RFC 1321 for a description of the MD5 algorithm.
 */

#include <asm/mipsregs.h>
#include <time.h>

#ifdef HAVE_FLOAT
#include <math.h>
#define printf newprintf
#endif
#define SERIAL_REG 0xb80003f8
#define UART16550BASE 0xb80003f8
//#define SERIAL_REG 0xbfe00000

int putchar(int c)
{
tgt_putchar(c);
return 0;
}

int idle();
int getchar()
{
	while(!tgt_testchar())
	{
		idle();
	}
return tgt_getchar();
}

int initserial()
{
volatile char *p = COM1_BASE_ADDR;
int div = 125000000/115200/16;
p[3] = 0x80;
p[0] = div;
p[1] = div>>8;
p[3] = 3;
p[2] = 0x47;
p[4] = 3;
p[1] = 0;
}

int errno;
int *
__errno_location (void)
{
  return (int *) &errno;
}


int mymain()
{
	putchar('1');
#ifdef HAVE_FLOAT
	tgt_fpuenable();
#endif
	putchar('2');
	init_IRQ();
	putchar('#');
	initserial();
	__init();
	cmdloop();
	return 0;
}

