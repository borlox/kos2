#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

/* The magic number for the Multiboot header. */
#define MB_HEADER_MAGIC          0x1BADB002

#define MB_BOOT_MAGIC            0x2BADB002

/* Types. */

/* The Multiboot header. */
typedef struct multiboot_header
{
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	uint32_t header_addr;
	uint32_t load_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
	uint32_t entry_addr;
} mb_header_t;

/* The symbol table for a.out. */
typedef struct aout_symbol_table
{
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
} aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct elf_section_header_table
{
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
} elf_section_header_table_t;

enum mb_flags
{
	MB_BASIC_MEM = 0x0001,
	MB_BOOT_DEV  = 0x0002,
	MB_CMDLINE   = 0x0004,
	MB_MODULES   = 0x0008,
	MB_AOUT_SYMS = 0x0010,
	MB_ELF_SYMS  = 0x0020,
	MB_MMAP      = 0x0040,
	MB_DRIVES    = 0x0080,
	MB_CONFIG    = 0x0100,
	MB_NAME      = 0x0200,
	MB_APM_TAB   = 0x0400,
	MB_VBE_INFO  = 0x0800,
};

/* The Multiboot information. */
typedef struct multiboot_info
{
	uint32_t flags;

	uint32_t mem_lower;
	uint32_t mem_upper;

	uint32_t boot_device;

	uint32_t cmdline;

	uint32_t mods_count;
	uint32_t mods_addr;

	union
	{
		aout_symbol_table_t aout_sym;
		elf_section_header_table_t elf_sec;
	} u;

	uint32_t mmap_length;
	uint32_t mmap_addr;

	uint32_t drives_length;
	uint32_t drives_addr;

	uint32_t config_table;

	uint32_t boot_loader_name;

	uint32_t apm_table;

	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
} mb_info_t;

/* The module structure. */
typedef struct multiboot_mod
{
	uint32_t mod_start;
	uint32_t mod_end;
	uint32_t cmdline;
	uint32_t reserved;
} mb_mod_t;

/* The memory map. Be careful that the offset 0 is base_addr_low
	but no size. */
typedef struct multiboot_mmap
{
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} mb_mmap_t;

#endif /*MULTIBOOT_H*/
