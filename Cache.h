#ifndef __Cache_H__
#define __Cache_H__
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include <stdbool.h> 


typedef struct{
	unsigned char CLO;
	bool validData;
	unsigned int data[8];
	char flags[8];
	
}Cache;

void cacheReset(Cache* cache);
void setCacheOn(Cache* cache);
void setCacheOff(Cache* cache);
void cacheDump(Cache* cache);
void tryCacheLW(Memory* memory, Cache* cache, unsigned int address, unsigned int count, unsigned int *answerPtr, bool *fetchDonePtr);
void tryCacheSW(Memory* memory, Cache* cache, unsigned int address, unsigned int count, unsigned int answerVal, bool *fetchDonePtr);


#endif