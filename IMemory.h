#ifndef __IMemory_H__
#define __IMemory_H__
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"


typedef struct
{
 unsigned int size;
 unsigned int* ptr;
}IMemory;


void iMemCreate(IMemory* imemory, unsigned int size);
void iMemReset(IMemory* imemory);
void iMemDump(IMemory* imemory, unsigned int hexaddress, unsigned int hexcount);
void iMemSet(IMemory* imemory, unsigned int hexaddress, char* file);


#endif
