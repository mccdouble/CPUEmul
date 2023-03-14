#ifndef __CPU_H__
#define __CPU_H__
#include "Clock.h"
#include "Memory.h"
#include "IMemory.h"
#include "Cache.h"
#include <stdbool.h>

typedef struct
{
 unsigned int RA, RB, RC, RD, RE, RF, RG, RH, PC, TC;

}CPU;

void cpuReset(CPU* cpu);
void setreg(CPU* cpu, char reg[], unsigned int hexbyte);
void cpuDump(CPU* cpu);
bool cpuIsMoreCycleWorkNeeded();
unsigned int* regAddress(CPU* cpu, unsigned int dest);
void cpuDoCycleWork(CPU* cpu, Memory* memory, IMemory* imemory, Cache* cache);
void cpuStartTick();

#endif
