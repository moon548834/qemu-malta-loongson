#ifndef __BITOPS_H
#define __BITOPS_H
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
#endif
