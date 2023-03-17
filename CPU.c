#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include "IMemory.h"
#include "Cache.h"

bool cpuTickStarted;
bool workState = true;
bool cpuMoreWork = false;
bool haltState = false;
bool instWorkState = true;
unsigned int command, inst, dest, src, tgt, reg, address;
signed char sourceVal, tgtVal, destVal; //signed 8 bit intermediate values 
unsigned char immVal;
unsigned int* source;
unsigned int* target;
unsigned int* destination;
int cpuCount = 0; //cycle counter
int instCount = 0;


void cpuStartTick(){
	cpuTickStarted = true;	
}

//different commands for cpu
void cpuDoCycleWork(CPU* cpu, Memory* memory, IMemory* imemory, Cache* cache){
	if(cpuTickStarted == true && haltState == false){
		cpu->TC = cpu->TC + 1;
		if(instCount == 1){
			instCount = 0;
			instWorkState = false;
		}
		else{
			instWorkState = true;
		}
		
		
			
	//fetch instruction from imem pointed to by PC
	if(workState && instWorkState){ //cpu not waiting for memory 
		command = imemory->ptr[cpu->PC];
		inst = command >> 17;
		dest = command >> 14 & 7;
		src  = command >> 11 & 7;
		tgt  = command >> 8 & 7; 
		immVal = command & 255;
		printf("inst: %x\n", imemory->ptr[cpu->PC]);
		 
		//add instruction	 
		if(inst == 0){ 
			destination = regAddress(cpu, dest);
			source = regAddress(cpu, src);
			sourceVal = *source;
			tgtVal = *target;
			*destination = sourceVal + tgtVal;
			cpu->PC = cpu->PC + 1;
		}
		
		//addi instruction
		else
		if(inst == 1){	 
			destination = regAddress(cpu, dest); //address of destination reg
			source = regAddress(cpu, src);
			sourceVal = *source;
			*destination = immVal + sourceVal;
			 cpu->PC = cpu->PC + 1;
		}	
	
		//mul instruction
		//takes 2 cycles
		else
		if(inst == 2){
			destination = regAddress(cpu, dest);
			source = regAddress(cpu, src);
			unsigned char lsb = *source & 15;
			unsigned char msb = *source >> 4 & 15;
			*destination = lsb * msb;
			cpu->PC = cpu->PC + 1;
			instCount = 1;

		}
		
		//inv instruction
		else 
		if(inst == 3){
			destination = regAddress(cpu, dest);
			source = regAddress(cpu, src);
			sourceVal = *source;
			*destination = ~sourceVal & 255;
			cpu->PC = cpu->PC + 1;

		}
		
		//beq instruction
		else
		if(inst == 4 && dest == 0){
			source = regAddress(cpu, src);
			target = regAddress(cpu, tgt);
			sourceVal = *source;
			tgtVal = *target;

			if(sourceVal == tgtVal){
			 cpu->PC = immVal;
			 instCount = 1;
			}
			else{
			   cpu->PC = cpu->PC + 1;	
			} 

		}
		//bneq instruction
		else
		if(inst == 4 && dest == 1){
			source = regAddress(cpu, src);
			target = regAddress(cpu, tgt);
			sourceVal = *source;
			tgtVal = *target;

			if(sourceVal != tgtVal){
			 cpu->PC = immVal;
			 instCount = 1;
			}
			else{
			   cpu->PC = cpu->PC + 1;	
			}
		}
		//blt instruction
		else
	    	if(inst == 4 && dest == 2){
			source = regAddress(cpu, src);
			target = regAddress(cpu, tgt);
			sourceVal = *source;
			tgtVal = *target;
			if(sourceVal < tgtVal){
				cpu->PC = immVal;
				instCount = 1;14
			}
			else{
				cpu->PC = cpu->PC + 1;
			}
		}
		
		//lw instruction
		else
		if(inst == 5){ 
			printf("address of cache[1] %d\n", &cache->data[1]);
			destination = regAddress(cpu, dest);//getting address of source register
			printf("address of destination %d\n", destination);
			target = regAddress(cpu, tgt);//getting address of target register
			tgt = *target;
			tryCacheLW(memory, cache, tgt, 1, destination, &workState);
			cpu->PC = cpu->PC + 1;	
		}
			
		//sw instruction
           	else  
		if(inst == 6){  
			  source = regAddress(cpu, src);    //getting address of target destination
			  target = regAddress(cpu, tgt);     //getting value of target register
			  tgt = *target;
			  src = *source;
		      	  tryCacheSW(memory, cache, tgt, 1, src, &workState);
			  cpu->PC = cpu->PC + 1;	
            	}	 
		   
	    	//halt instruction
		else 
		if(inst == 7){
			//increment pc
			//ignore all future clock ticks
			 cpu->PC = cpu->PC + 1;
			 haltState = true;
		}
	}
    }
}	



bool cpuIsMoreCycleWorkNeeded(){	   
	return false; //since there is only ever 1 chance to do work during cycle for assignment 2
}

void cpuReset(CPU* cpu){
   cpu->RA = 0;
   cpu->RB = 0;
   cpu->RC = 0;
   cpu->RD = 0;
   cpu->RE = 0;
   cpu->RF = 0;
   cpu->RG = 0;
   cpu->RH = 0;
   cpu->PC = 0;
   cpu->TC = 0;
   haltState = false;
}

//loop through to see what register is being set,
//and set its value to hexbyte
void setreg(CPU* cpu, char reg[], unsigned int hexbyte){
	 if(strcmp(reg, "RA") == 0){
		cpu->RA = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RB") == 0){
		cpu->RB = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RC") == 0){
		cpu->RC = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RD") == 0){
		cpu->RD = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RE") == 0){
		cpu->RE = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RF") == 0){
		 cpu->RF = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RG") == 0){
		cpu->RG = hexbyte;
	 }
	 else
	 if(strcmp(reg, "RH") == 0){
		cpu->RH = hexbyte;
	 }
	 else
	 if(strcmp(reg, "PC") == 0){
		cpu->PC = hexbyte;
	 }

}

//format a dump to stdout of all the registers and
void cpuDump(CPU* cpu){
	printf("\n");
	printf("PC: 0x%02X\n", cpu->PC);
	printf("RA: 0x%02X\n", cpu->RA);
	printf("RB: 0x%02X\n", cpu->RB);
	printf("RC: 0x%02X\n", cpu->RC);
	printf("RD: 0x%02X\n", cpu->RD);
	printf("RE: 0x%02X\n", cpu->RE);
	printf("RF: 0x%02X\n", cpu->RF);
	printf("RG: 0x%02X\n", cpu->RG);
	printf("RH: 0x%02X\n", cpu->RH);
	printf("TC: %d\n", cpu->TC);
	printf("\n");
}

//called by cpuDoWork to get the memory address of the specified register
unsigned int* regAddress(CPU* cpu, unsigned int dest){
     if(dest == 0){
	    return &cpu->RA;
	 }
	 else
     if(dest == 1){
	    return &cpu->RB;
	 }
	 else
     if(dest == 2){
	    return &cpu->RC;
	 }
	 else
	 if(dest == 3){
	    return &cpu->RD;
	 }
	 else
     if(dest == 4){
	    return &cpu->RE;
	 }
	 else
     if(dest == 5){
	    return &cpu->RF;
	 }
     else
     if(dest == 6){
	    return &cpu->RG;
	 }
	 else
     if(dest == 7){
	    return &cpu->RH;
	 }
	 else
		 return 0;
}




































