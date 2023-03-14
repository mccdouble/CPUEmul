#ifndef __Clock_H__
#define __Clock_H__
#include "CPU.h"
#include "Memory.h"
#include "IMemory.h"
#include "Cache.h"


typedef struct{
  int count;
}Clock;

//function prototypes implemented in Clock.c
void clockReset(Clock* clock);
void tick(Clock* clock,  Memory* memory, CPU* cpu, IMemory* imemory, Cache* cache, int ticks);
void clockDump(Clock* clock);
void clockTick(Clock* clock, Memory* memory, CPU* cpu);

#endif
