#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "memlib.h"
#include "system/debug/sys_debug.h"
#include "Gui/DisplayControl.h"

//#define MAX_DDR_HEAP                      (22 * 1024 * 1024 + 512 * 1024 - 100 * 1024 * 10 /*SQI Interface*/) 
#define MAX_DDR_HEAP                      0x1633FFF //(31 * 1024 * 1024)
//char ddr_mem[MAX_DDR_HEAP] __attribute__((section(".ddr_data"), space(prog)));

/* private variables */
static char *heap;
static char *mem_brk;
static char *mem_max_addr;

/* 
 * mem_init - initialize the memory system model
 */
void mem_init(void){
//	int dev_zero = open("/dev/zero", O_RDWR);
//	heap = mmap((void *)0x800000000, /* suggested start*/
//			MAX_HEAP,				/* length */
//			PROT_WRITE,				/* permissions */
//			MAP_PRIVATE,			/* private or shared? */
//			dev_zero,				/* fd */
//			0);						/* offset (dunno) */
//    heap = (char*)0xA88CC000;
//    heap = (void*)&ddr_mem[0];
    
    // TODO: ddr_data_mem (w!x) : ORIGIN = 0xA88CD000, LENGTH = 0x100000
    heap = (void*)0xA89CD000;       // Calculated by (0xA8000000 + 32MB - MAX_DDR_HEAP) 
	mem_max_addr = heap + MAX_DDR_HEAP;
	mem_brk = heap;					/* heap is empty initially */
    
//    SYS_PRINT("\n heap = %x", heap);
//    DisplayControl_PrintHex("\n heap first address = ", (int)heap);
    
}

/* 
 * mem_deinit - free the storage used by the memory system model
 */
void mem_deinit(void){
//	munmap(heap, MAX_HEAP);
}

/*
 * mem_reset_brk - reset the simulated brk pointer to make an empty heap
 */
void mem_reset_brk(){
	mem_brk = heap;
}

/* 
 * mem_sbrk - simple model of the sbrk function. Extends the heap 
 *		by incr bytes and returns the start address of the new area. In
 *		this model, the heap cannot be shrunk.
 */
void *mem_sbrk(int incr) {
	char *old_brk = mem_brk;

    // call sbrk() in an attempt to have similar semantics as a real allocator.
//	if ( (incr < 0) || ((mem_brk + incr) > mem_max_addr) ||
//            sbrk(incr) == (void *) -1) {
//		errno = ENOMEM;
//		fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
//		return (void *)-1;
//	}
    if ( (incr < 0) || ((mem_brk + incr) > mem_max_addr) )
    {
		errno = ENOMEM;
		fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
		return (void *)-1;
	}        
    
	mem_brk += incr;
	return (void *)old_brk;
}

/*
 * mem_heap_lo - return address of the first heap byte
 */
void *mem_heap_lo(){
	return (void *)heap;
}

/* 
 * mem_heap_hi - return address of last heap byte
 */
void *mem_heap_hi(){
	return (void *)(mem_brk - 1);
}

/*
 * mem_heapsize() - returns the heap size in bytes
 */
size_t mem_heapsize() {
	return (size_t)((void *)mem_brk - (void *)heap);
}

/*
 * mem_pagesize() - returns the page size of the system
 */
size_t mem_pagesize(){
//	return (size_t)getpagesize();
    return 0;
}
