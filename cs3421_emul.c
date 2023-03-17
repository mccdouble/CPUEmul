#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include "IMemory.h"
#include "Cache.h"

int main(int argc, char** argv){ 
  if(argc < 2){
     printf("cs3421_emul <input file>\n");
     return 0;
  }

  //parse token
  char token[7];
  char* fileName = argv[1];
  FILE* fp = fopen(fileName, "r");
  
  //device instantiations
  Clock clock;
  Memory memory;
  CPU cpu;
  IMemory imemory;
  Cache cache;
   
  while(fscanf(fp, "%s", token) == 1){
	//if specified device is clock
	if(strcmp(token, "clock") == 0){
		fscanf(fp, "%s", token);
		//reset command
		if(strcmp(token, "reset") == 0){
			clockReset(&clock);
		}
		//tick command
		else
		if(strcmp(token, "tick") == 0){
			int ticks;
			fscanf(fp, "%d", &ticks);
			tick(&clock, &memory, &cpu, &imemory, &cache, ticks);
		}
		else 
		if(strcmp(token, "dump") == 0){
			clockDump(&clock);	
		}

	}
	
	//if specified device is cpu
	else 
	if(strcmp(token, "cpu") == 0){
	fscanf(fp, "%s", token);
		//reset command
		if(strcmp(token, "reset") == 0){
			cpuReset(&cpu);
		}
		
		//setreg command
		else
		if(strcmp(token, "set") == 0){
		   fscanf(fp, "%s", token);
		   if(strcmp(token, "reg") == 0){
			  char reg[1];
			  unsigned int hexbyte;
			  fscanf(fp, "%s", reg);		  
			  fscanf(fp, "%x", &hexbyte);		
			  setreg(&cpu, reg, hexbyte);
			}
		}
		
		//cpuDump command
		else
		if(strcmp(token, "dump") == 0){
		    cpuDump(&cpu);
		}
	   
	}

	else
	if(strcmp(token, "memory") == 0){
		fscanf(fp, "%s", token);
		//memCreate command
		if(strcmp(token, "create") == 0){
		unsigned int size;
		fscanf(fp, "%x", &size);
		memCreate(&memory, size);
	}
				
	//mem set command 
	else
	if(strcmp(token, "set") == 0){
		unsigned int hexaddress;
		unsigned int hexcount;
		fscanf(fp, "%x", &hexaddress);
		fscanf(fp, "%x", &hexcount);
		unsigned int values[hexcount];

		unsigned int i;
		for(i = 0; i<hexcount; i++){
		  fscanf(fp, "%x", &values[i]);
		}
		//set memory to values specified at their respective locations
		memSet(&memory, hexaddress, hexcount, values);
	}

	//memDump command
	else
	if(strcmp(token, "dump") == 0){
		unsigned int hexaddress;
		unsigned int hexcount;
		fscanf(fp, "%x", &hexaddress);		  
		fscanf(fp, "%x", &hexcount);
		memDump(&memory, hexaddress, hexcount);
	}

	//memReset command
	else
	if(strcmp(token, "reset") == 0){
		memReset(&memory);
	}
		
  }
	
	//imem commands
	else
	if(strcmp(token, "imemory") == 0){
		fscanf(fp, "%s", token);
	  	//imemCreate command
		if(strcmp(token, "create") == 0){
		  unsigned int size;
		  fscanf(fp, "%x", &size);
		  iMemCreate(&imemory, size);
		}
				
			//imem set command 
		else
		if(strcmp(token, "set") == 0){
		  unsigned int hexaddress;
		  char file[30] = "";
		  char* ptr = file;
		  fscanf(fp, "%x", &hexaddress);
		  fscanf(fp, "%*s %s", ptr);	
		  iMemSet(&imemory, hexaddress, file);
  
		}
			
		//imemDump command
		else
		if(strcmp(token, "dump") == 0){
		  unsigned int hexaddress;
		  unsigned int hexcount;
		  fscanf(fp, "%x", &hexaddress);		  
		  fscanf(fp, "%x", &hexcount);
		  iMemDump(&imemory, hexaddress, hexcount);
		}
		
		//imemReset command
		else
		if(strcmp(token, "reset") == 0){
			iMemReset(&imemory);
		}
		
  	}
	
	//cache commands 
	else
	if(strcmp(token, "cache") == 0){
	   fscanf(fp, "%s", token);
	   
	   if(strcmp(token, "reset") == 0){
		   cacheReset(&cache);
	   }
	   
	   else
	   if(strcmp(token, "on") == 0){
		   setCacheOn(&cache);		   
	   }
	   
	   else
	   if(strcmp(token, "off") == 0){
		   setCacheOff(&cache);
	   }
	   
	   else
	   if(strcmp(token, "dump") == 0){
			cacheReset(&cache);
	   }
	   
	
	}
 }

 //free memory storage
 free(memory.ptr);
 free(imemory.ptr);
 fclose(fp);
 fp = NULL; 
 return 0;
	
}

