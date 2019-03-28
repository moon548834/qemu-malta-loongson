TOPDIR=$(shell pwd)
BUILDDIR=$(TOPDIR)
obj=$(BUILDDIR)
src=$(TOPDIR)
bld=$(BUILDDIR)

export TOPDIR BUILDDIR obj src bld



all: clean allbins
ifeq ($(LOADADDR), )
	make LOADADDR=0x980000001fc00000 clean all
	make LOADADDR=0x8000f000 clean  all
	make LOADADDR=0xffffffff80010000 clean all
endif
	make clean

testbin:
	gcc -o testbin testbin.c
clean:
	rm -f *.o *.s *.elf *.a testbin *.o64 *.s64 *.elf64 *.ELF *.O *.ELF64 *.O64 *.lds
	make -C lib clean
	make -C libm clean
cleanall:
	rm -f *.bin* *.bin64* *.BIN *.BIN64 *.bin-* *.BIN-* *.bin64-* *.BIN64-*
	make clean

include rules.make
