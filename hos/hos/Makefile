NOFIXUP=1
TOPDIR=$(PWD)/../bin/
BUILDDIR=$(PWD)/
obj=$(BUILDDIR)
src=$(PWD)
export LOADADDR=0xffffffff80200000
export COMBASE=0xffffffffb80003f8

export MEMSIZE=0x400000
export obj src NOFIXUP TOPDIR BUILDDIR
export HAVE_FLOAT=1
EXTRA_CFLAGS := $(EXTRA_CFLAGS) -DCOM1_BASE_ADDR=$(COMBASE) -DHAVE_FLOAT -D_KERNEL -DCONFIG_PAGE_SIZE_16KB -DLOADADDR=$(LDADDR) -I$(PWD)/include -DFLOATINGPT -DMYHZ=1 -DCPUFREQ=1000000000 #-Wa,-mfix-loongson2k-jump=2
export CROSS_COMPILE=/opt/mips64_crosscompile/cross-gcc-4.9.3-n64-loongson-rc6.1/usr/bin/mips64el-loongson-linux-
#export CROSS_COMPILE=/opt/gcc-4.4-64-gnu/bin/mips64el-linux-
#export CROSS_COMPILE=/opt/gcc-4.3-64-nopic/bin/mips64el-linux-

all:
	#make clean test.elf test.bin
	make clean test.elf64 test.bin64

bin1.lds: bin.lds.S
	${CROSS_COMPILE}gcc -Umips -DLOADADDR=$(LDADDR) -DMEMSIZE=$(MEMSIZE) -E -P -o $@ bin.lds.S 

bin641.lds: bin64.lds.S
	${CROSS_COMPILE}gcc -Umips -DLOADADDR=$(LDADDR) -DMEMSIZE=$(MEMSIZE) -E -P -o $@ bin64.lds.S 

clean:
	rm -f *.o *.a *.s *.o64 *.O *.O64 *.elf *.ELF64 *.lds *.bin *.bin64 *.i
	rm -rf lib* *.elf*
cleanall:
	make clean
	rm -f *.bin* *.BIN*
#modified delete crtbegin.o
test.elf: bin1.lds start.o test.o irq.o time.o idle.o crtbegin.o libtinyc.a libm.a
	${CROSS_COMPILE}ld -g -T  bin1.lds $(LDFLAGS) -o $@  start.o test.o irq.o time.o idle.o crtbegin.o -L . -ltinyc -lm

test.elf64: bin641.lds start64.o64 test.o64 irq.o64  time.o64 idle.o64 crtbegin.o64 libtinyc64.a libm64.a
	${CROSS_COMPILE}ld -g -T  bin641.lds $(LDFLAGS) -o $@  start64.o64 test.o64 irq.o64  time.o64 idle.o64 crtbegin.o64 -L . -ltinyc64 -lm64

test.bin: test.elf
	${CROSS_COMPILE}objcopy -O binary $< $@

test.bin64: test.elf64
	${CROSS_COMPILE}objcopy -O binary $< $@


include $(TOPDIR)/rules.make

