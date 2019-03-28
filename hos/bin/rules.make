CROSS_COMPILE?=mipsel-linux-
CFLAGS = -march=mips64 $(EXTRA_CFLAGS)
ifdef HAVE_FLOAT
CFLAGS += -DHAVE_FLOAT -DFLOATINGPT
endif
export CROSS_COMPILE


try-run = $(shell set -e;		\
	TMP="$(TMPOUT).$$$$.tmp";	\
	if ($(1)) >/dev/null 2>&1;	\
	then echo "$(2)";		\
	else echo "$(3)";		\
	fi;				\
	rm -f "$$TMP")


# cc-option
# Usage: cflags-y += $(call cc-option,-march=winchip-c6,-march=i586)

cc-option = $(call try-run,\
	$(CROSS_COMPILE)gcc -march=mips64 $(1) -c -xc /dev/null -o "$$TMP",$(1),$(2))

comma   := ,

CFLAGS1 :=-O2 -g  -DGUEST -I include -I . -fno-pic -mno-abicalls -nostdinc -nostdlib -fno-builtin -mno-branch-likely -DCPU_COUNT_PER_US=10 -I $(TOPDIR)/include -include common.h $(CFLAGS)

ifdef LOADADDR
ifndef NOFIXUP
CFLAGS += $(call cc-option,-Wa$(comma)-mfix-jrjalr-with-hb -Wa$(comma)--mfix-ls-jump=0xfffff -Wa$(comma)--mfix-ls-jump1=$(LOADADDR) -DFIXUP,)
endif

CFLAGS :=-O2 -g -DGUEST -I include -I . -fno-pic -mno-abicalls -nostdinc -nostdlib -fno-builtin -mno-branch-likely -DCPU_COUNT_PER_US=10 -I $(TOPDIR)/include -include common.h $(CFLAGS)
affix=-$(LOADADDR)
LDADDR=$(LOADADDR)
else
CFLAGS :=-O2 -g -DGUEST -I include -I . -fpic -mabicalls -nostdinc -nostdlib -fno-builtin -mno-branch-likely -DCPU_COUNT_PER_US=10 -I $(TOPDIR)/include -include common.h $(CFLAGS)
LDADDR = 0
affix=
endif

srcs := $(shell echo *.c|sed 's/\stest[^.]*\.c/ /g' ) test.c
bins := $(patsubst %.c,%.bin$(affix),$(srcs)) $(patsubst %.c,%.bin64$(affix),$(srcs)) $(shell echo *.S|sed 's/bin.lds.S//;s/bin64.lds.S//;s/start.S//;s/start64.S//;'|sed 's/[.]S/.BIN64/g') $(shell echo *.S|sed 's/bin.lds.S//;s/bin64.lds.S//;s/start.S//;s/start64.S//;s/loongson3\S*.S//g;'|sed 's/[.]S/.BIN/g')

allbins: $(bins)

export CFLAGS LOADADDR

cmd_lib = make -C $(1) src=$(1)  obj=$(obj)/$(@:.a=)  lib=$(obj)/$@ $(obj)/$(@:.a=) $(obj)/$@

libtinyc.a libtinyc64.a: $(TOPDIR)/lib
	$(call cmd_lib,$<)
libm.a libm64.a: $(TOPDIR)/libm
	$(call cmd_lib,$<)


.SUFFIXES: .elf .o64 .elf64 .bin64 .BIN .i .O .O64 .BIN .BIN64 .ELF .BIN64


%.elf:%.o start.o libtinyc.a libm.a bin.lds
	$(CROSS_COMPILE)ld --emit-relocs -g -T  bin.lds $(LDFLAGS) -o $@ start.o $<  -L . -ltinyc -lm


%.bin$(affix):%.elf
	$(CROSS_COMPILE)objcopy -O binary $< $@ 

%.elf64:%.o64 start64.o64 libtinyc64.a libm.a bin64.lds
	$(CROSS_COMPILE)ld --emit-relocs -g -T  bin64.lds $(LDFLAGS) -o $@ start64.o64 $<  -L . -ltinyc64 -lm


%.bin64$(affix):%.elf64
	$(CROSS_COMPILE)objcopy -O binary $< $@ 


%.ELF:%.O  bin.lds
	$(CROSS_COMPILE)ld -g -T  bin.lds $(LDFLAGS) -o $@  $< 

%.BIN:%.ELF
	$(CROSS_COMPILE)objcopy -O binary  $< $@$(affix)

%.ELF64:%.O64  bin64.lds
	$(CROSS_COMPILE)ld -g -T  bin64.lds $(LDFLAGS) -o $@  $< 

%.BIN64:%.ELF64
	$(CROSS_COMPILE)objcopy -O binary  $< $@$(affix)

bin.lds: $(TOPDIR)/bin.lds.S
	$(CROSS_COMPILE)gcc -Umips -DLOADADDR=$(LDADDR) -E -P -o $@ $(TOPDIR)/bin.lds.S 

bin64.lds: $(TOPDIR)/bin.lds.S
	$(CROSS_COMPILE)gcc -Umips -DLOADADDR=$(LDADDR) -E -P -o $@ $(TOPDIR)/bin64.lds.S

$(obj):
	mkdir -p $@
$(obj)/%.o %.o: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -mabi=32 -c $< 
$(obj)/%.o %.o: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -mabi=32 -c $< 
$(obj)/%.i %.i: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -mabi=32 -g3 -C -E $< 
$(obj)/%.i %.i: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -mabi=32 -g3 -C -E $< 
$(obj)/%.o64 %.o64: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -c $<  -mabi=64 -DHAVE_QUAD
$(obj)/%.o64 %.o64: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o $@ -c $<  -mabi=64 -DHAVE_QUAD
$(obj)/%.s %.s: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS) -S -fverbose-asm -o $@ $< 
$(obj)/%.s %.s: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS) -S -fverbose-asm -o $@  $< 
$(obj)/%.s64 %.s64: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS) -S -fverbose-asm -o $@ -c $<  -mabi=64 -DHAVE_QUAD
$(obj)/%.s64 %.s64: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS) -S -fverbose-asm -o $@ -c $<  -mabi=64 -DHAVE_QUAD
$(obj)/%.O %.O: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS1) -o $@ -c $<  -mabi=32
$(obj)/%.O %.O: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS1) -o $@ -c $<  -mabi=32
$(obj)/%.O64 %.O64: $(src)/%.S
	$(CROSS_COMPILE)gcc  $(CFLAGS1) -o $@ -c $<  -mabi=64 -DHAVE_QUAD
$(obj)/%.O64 %.O64: $(src)/%.c
	$(CROSS_COMPILE)gcc  $(CFLAGS1) -o $@ -c $<  -mabi=64 -DHAVE_QUAD

objs64=$(objs:.o=.o64)

objs32    := $(addprefix $(obj)/, $(objs))
objs64  := $(addprefix $(obj)/, $(objs64))

lib32=$(patsubst  %64.a,%.a,$(lib))
lib64=$(patsubst  %.a,%64.a,$(lib32))

$(lib32): $(objs32)
	$(CROSS_COMPILE)$(AR) -cr  $@ $?

$(lib64): $(objs64)
	$(CROSS_COMPILE)$(AR) -cr  $@ $?
