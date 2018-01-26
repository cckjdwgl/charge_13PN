/**
 * @file
 * Dynamic pool memory manager
 *
 * lwIP has dedicated pools for many structures (netconn, protocol control blocks,
 * packet buffers, ...). All these pools are managed here.
 */


#ifndef dwgl_memp_c
#define dwgl_memp_c			//dwgl modify in dns.c

#include "dwgl_memp.h"

struct  dwgl_RAM DwglRam;

void 
dwglmemp_init(void)
{
	unsigned short i;
	unsigned char ram_num;
//	unsigned short buffer_size;	
	DwglRam.state = '1';
	ram_num = DefNum_KB1;
	for(i=0;i<ram_num;i++)
	{
		DwglRam.flag[i].head = 'F'; //使用开关
		DwglRam.flag[i].priority = '0';//优先级
		DwglRam.flag[i].use = '0';
		DwglRam.flag[i].link = '0';
		DwglRam.flag[i].point = DwglRam.Kb1[DefNum_KB1-(ram_num-i)];
		DwglRam.flag[i].size  = sizeof(DwglRam.Kb1[DefNum_KB1-(ram_num-i)]);
		DwglRam.flag[i].time  = time_sys;
	}
	ram_num += DefNum_KB2;
	for(;i<ram_num;i++)
	{
		DwglRam.flag[i].head = 'F'; //使用开关
		DwglRam.flag[i].priority = '0';//优先级
		DwglRam.flag[i].use = '0';
		DwglRam.flag[i].link = '0';
		DwglRam.flag[i].point = DwglRam.Kb4[DefNum_KB2-(ram_num-i)];
		DwglRam.flag[i].size  = sizeof(DwglRam.Kb2[DefNum_KB2-(ram_num-i)]);
		DwglRam.flag[i].time  = time_sys;
	}
	ram_num += DefNum_KB4;
	for(;i<ram_num;i++)
	{
		DwglRam.flag[i].head = 'F'; //使用开关
		DwglRam.flag[i].priority = '0';//优先级
		DwglRam.flag[i].use = '0';
		DwglRam.flag[i].link = '0';
		DwglRam.flag[i].point = DwglRam.Kb4[DefNum_KB4-(ram_num-i)];
		DwglRam.flag[i].size  = sizeof(DwglRam.Kb4[DefNum_KB1-(ram_num-i)]);
		DwglRam.flag[i].time  = time_sys;
	}
}

/**
 * Get an element from a specific pool.
 *
 * @param type the pool to get an element from
 *
 * the debug version has two more parameters:
 * @param file file name calling this function
 * @param line number of line where this function is called
 *
 * @return a pointer to the allocated memory or a NULL pointer on error
 */
void *
dwglmemp_malloc(unsigned short size)
{
	unsigned char i;
	unsigned char flag_temp;
	unsigned short size_temp;
	void* memp;
	
	memp = NULL;	
	if(DwglRam.state!='1')
	{
		dwglmemp_init();
	}
	else
	{
		size_temp = 0;
		for(i=0;i<DefNum_KB;i++)
		{
			if(DwglRam.flag[i].head=='F')
			{
				if(DwglRam.flag[i].use!='U' )
				{
					if(size_temp==0)
					{
						memp = DwglRam.flag[i].point;
						flag_temp = i;
					}
					size_temp += DwglRam.flag[i].size;
				}
				else
				{
					size_temp = 0;
					memp = NULL;
				}
				if(size_temp>= size)
				{
					break;
				}				
			}
			else
			{	
				DwglRam.state='0';   //重新初始
			}		
		}
	
	}
	if(size_temp>= size)
	{
		size_temp = 0;
		for(i=flag_temp;i<DefNum_KB;i++)
		{
			size_temp += DwglRam.flag[i].size;
			DwglRam.flag[i].head = 'F'; //使用开关
			DwglRam.flag[i].priority = '1';//优先级
			DwglRam.flag[i].use = 'U';
			DwglRam.flag[i].link = 'L';			
			DwglRam.flag[i].time  = time_sys;
			if(size_temp>= size)
			{
				DwglRam.flag[i].link = '0';
				break;
			}
		}		
	}
	else
	{
		memp = NULL;
	}
	return memp;	
}

/**
 * Put an element back into its pool.
 *
 * @param type the pool where to put mem
 * @param mem the memp element to free
 */
unsigned char 
dwglmemp_free(void *mem)
{
	unsigned char i;
	unsigned char flag_temp;
//	unsigned short size_temp;
//	void* memp;
	
	if(DwglRam.state!='1')
	{
		dwglmemp_init();
	}
	else
	{
		flag_temp = 0;  
		for(i=0;i<DefNum_KB;i++)
		{
			if(DwglRam.flag[i].head=='F')
			{
				if(DwglRam.flag[i].point==mem )  //
				{					
					flag_temp = 1;
				}
				if(flag_temp== 1)
				{
					if(DwglRam.flag[i].link == 'L')
					{
						DwglRam.flag[i].head = 'F'; //使用开关
						DwglRam.flag[i].priority = '0';//优先级
						DwglRam.flag[i].use = '0';
						DwglRam.flag[i].link = '0';		
					}						
					else
					{
						DwglRam.flag[i].head = 'F'; //使用开关
						DwglRam.flag[i].priority = '0';//优先级
						DwglRam.flag[i].use = '0';
						DwglRam.flag[i].link = '0';		
						break;
					}
				}				
			}
			else
			{	
				DwglRam.state='0';   //重新初始
			}		
		}
	
	}
	return flag_temp;		
}

#endif /* MEMP_MEM_MALLOC */
