#ifndef MM_INTERN_H
#define MM_INTERN_H

/* some internal macros for mm */

#include "bitop.h"
#include "mm/page.h"
//#include "mm/virt.h"
#include "mm/types.h"

/* some helper functions to clarify the code */
static inline paddr_t getaddr(pany_entry_t entry)
{
	return (paddr_t)bmask((uint32_t)entry, BMASK_PE_ADDR);
}

static inline uint32_t getflags(pany_entry_t entry)
{
	return (uint32_t)bmask((uint32_t)entry, BMASK_PE_FLAGS);
}

#define page2addr(page) ((paddr_t)(page * PAGE_SIZE))
#define addr2page(addr) ((uint32_t)addr / PAGE_SIZE)

#define align_addr(addr)      ((paddr_t)((uint32_t)addr - PAGE_OFFSET(addr)))
#define align_size(addr,size) (size + PAGE_OFFSET(addr))

// returns the page directory/table index from an virtual addr
#define pdir_index(addr) (addr2page(addr) / PDIR_LEN)
#define ptab_index(addr) (addr2page(addr) % PTAB_LEN)

// assembles a virtual addr of the page directory/table indices and an offset
#define create_addr(pdi,pti,off) ((vaddr_t)((pdi << PE_ADDR_SHIFT) + (pti << PAGE_SHIFT) + off))

// returns a NULL safe first page table index
#define safe_pti(pdi) ((pdi == 0) ? 1 : 0)

extern ptab_t map_working_table(ptab_t tab); /* in mm.c */

#if 0
static inline ptab_entry_t get_ptab_entry(pdir_t pdir, _unaligned_ vaddr_t vaddr)
{
	ptab_t       tab;
	ptab_entry_t pte = 0;

	int pdidx = pdir_index(vaddr);

	if (bnotset(pdir[pdidx], PE_PRESENT)) {
		return 0;
	}
	else {
		tab = getaddr(pdir[pdidx]);
	}

	tab = map_working_table(tab);

	if (tab[ptab_index(vaddr)] & PE_PRESENT) {
		pte = tab[ptab_index(vaddr)];
	}

	return pte;
}
#endif

#endif /*MM_INTERN_H*/
