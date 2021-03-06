OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradlittlemips", "elf32-tradlittlemips")

OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = LOADADDR;

	. = ALIGN(4);
	.text       :
	{
	  *(.text)
	}

	. = ALIGN(4);
	.rodata  : { *(.rodata) }

	. = ALIGN(6);
	.data  : { *(.data) }

	. = .;
	_gp = ALIGN(16) + 0x7ff0;

	.got : {
	  __got_start = .;
	  *(.got)
	  __got_end = .;
	}

	.sdata  : { *(.sdata) }

	.u_boot_cmd : {
	  __u_boot_cmd_start = .;
	  *(.u_boot_cmd)
	  __u_boot_cmd_end = .;
	}

	uboot_end_data = .;
	num_got_entries = (__got_end - __got_start) >> 2;

	. = ALIGN(6);
	.sbss (NOLOAD)  : { *(.sbss) *(.scommon) }
	.bss (NOLOAD)  : { *(.bss) *(COMMON)  . = ALIGN(6); }
	uboot_end = .;
	_heap = . ;
}
