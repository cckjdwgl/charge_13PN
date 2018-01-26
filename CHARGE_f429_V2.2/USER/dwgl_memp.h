/**
 * @file
 * Dynamic pool memory manager
 *
 * lwIP has dedicated pools for many structures (netconn, protocol control blocks,
 * packet buffers, ...). All these pools are managed here.
 */

#ifndef dwgl_memp_h
#define dwgl_memp_h			//dwgl modify in dns.c

#define DefNum_KB1  		1	//
#define DefNum_KB2  		2	//
#define DefNum_KB4  		2	//
#define DefNum_KB  		(DefNum_KB1+DefNum_KB2+DefNum_KB4)	//
#define DefSize_KB1  		1	//
#define DefSize_KB2  		2050	//
#define DefSize_KB4  		4100	//

#include "dwgl_function.h"

struct  DwglRamFlag {
	unsigned char head;
	unsigned char priority;
	unsigned char use;
	unsigned char link;
	void* point;
	unsigned int size;
	unsigned int time;
};	  

struct  dwgl_RAM {
	unsigned char state;
	struct  DwglRamFlag flag[DefNum_KB];
	unsigned char Kb1[DefNum_KB1][DefSize_KB1];
	unsigned char Kb2[DefNum_KB2][DefSize_KB2];
	unsigned char Kb4[DefNum_KB4][DefSize_KB4];
};	  

struct  dwgl_RAM DwglRam;
extern void dwglmemp_init(void);
extern void * dwglmemp_malloc(unsigned short size);
extern unsigned char dwglmemp_free(void *mem);
#endif /* MEMP_MEM_MALLOC */
