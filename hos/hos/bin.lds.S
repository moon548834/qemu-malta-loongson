OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradlittlemips", "elf32-tradlittlemips")

OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = LOADADDR;
	_stack = . + MEMSIZE -32;

	. = ALIGN(8);
	.text       :
	{
	  *(.text)
	}

	. = ALIGN(8);
	.rodata  : { *(.rodata) }

	. = ALIGN(8);
	.data  : { 
		_fdata = . ;
	*(.data) 
	}

	.ctors         :
	{
		__CTOR_LIST__ = .;
			*(.ctors)
			__CTOR_END__ = .;
	}
	.dtors         :
	{
		__DTOR_LIST__ = .;
			*(.dtors)
			__DTOR_END__ = .;
	}

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

	. = ALIGN(8);
	.sbss (NOLOAD)  : { *(.sbss) *(.scommon) }
	.bss (NOLOAD)  : { *(.bss) *(COMMON)  . = ALIGN(8); }
	uboot_end = .;
	_heap = . ;
}
