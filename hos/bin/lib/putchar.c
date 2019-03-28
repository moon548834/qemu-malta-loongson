int putchar(int c)
{
	volatile int *lsr = (volatile int*)(0xb80003f8 + 0x0028);
	// Transmitter holding register.
	volatile int *thr = (volatile int*)(0xb80003f8);
	// Wait until THR is empty.
	while(((*lsr) & 0x20) == 0) { }
	*thr = c;
	//*(volatile char *)SERIAL_REG=c;
	return 0;
}
