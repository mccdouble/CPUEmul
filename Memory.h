#ifndef __Memory_H__
#define __Memory_H__
#include "Clock.h"
#include "CPU.h"
#include <stdbool.h>


typedef struct
{
 unsigned int size;
 unsigned int* ptr;
}Memory;

void memCreate(Memory* memory, unsigned int hexsize);
void memReset(Memory* memory);
void memDump(Memory* memory, unsigned int hexaddress, unsigned int hexcount);
void memSet(Memory* memory, unsigned int hexaddress, unsigned int hexcount, unsigned int values[]);
void beginMemFetch(Memory* memory ,unsigned int address, unsigned int count, unsigned int* savedAnswer, bool* waitState);
void beginMemSet(Memory* memory, unsigned int address, unsigned int count, unsigned int value, bool* waitState);
void memStartTick();
void memDoCycleWork();
bool memIsMoreCycleWorkNeeded();


#endif
