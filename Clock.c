#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include "IMemory.h"
#include "Cache.h"

void tick(Clock* clock, Memory* memory, CPU* cpu, IMemory* imemory, Cache* cache, int ticks){
	int i;
	for(i = 0; i < ticks; i++){
		clock->count = clock->count + 1;
		bool workToDo = true;
		
		memStartTick();
		cpuStartTick(cpu);
		while(workToDo)
		{
            memDoCycleWork(cpu, memory);
			cpuDoCycleWork(cpu, memory, imemory, cache);
			workToDo = cpuIsMoreCycleWorkNeeded() ||
					   memIsMoreCycleWorkNeeded();
					   
		}
	 
	}

}


void clockReset(Clock* clock){
       clock->count = 0;
}

void clockDump(Clock* clock){
	printf("Clock: %d\n", clock->count);
}





	
	
	


