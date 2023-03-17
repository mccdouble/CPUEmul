#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include "Cache.h"

bool cacheOn = false;
bool byteToWrite = false;
unsigned char requestedCLO = 0;


void cacheReset(Cache* cache){
	cacheOn = false;
	cache->CLO = 0;
	cache->validData = false;
	int i;
	
	for(i = 0; i < 8; i++){
		cache->data[i] = 0;
		cache->flags[i] = '\0';
	}		
}


void setCacheOn(Cache* cache){
	cacheOn = true;
}


void setCacheOff(Cache* cache){
	cacheOn = false;
}


void cacheDump(Cache* cache){
	printf("cache dump\n");
}


//takes lw command if cache is on do cache stuff
void tryCacheLW(Memory* memory, Cache* cache, unsigned int address, unsigned int count, unsigned int *answerPtr, bool *fetchDonePtr){
	if(cacheOn){
			requestedCLO = address >> 3;
			if(address == 0xFF){
				*answerPtr = 0;
				cache->validData = false;
			}
			else if(requestedCLO != cache->CLO || cache->validData == false){
				//cache miss, fill memory
				beginMemFetch(memory, address, 8, answerPtr, fetchDonePtr);
			}
			else if(requestedCLO == cache->CLO){
				requestedCLO = requestedCLO << 3;
				int i = address - requestedCLO;
				*answerPtr = cache->data[i];	 
				*fetchDonePtr = true;
		 
			}
	}
	else{//cache is off or there is no valid data, pass request on to memory
		 beginMemFetch(memory, address, 1, answerPtr, fetchDonePtr);
	}
}

//takes sw command if caache is on do cache stuff
void tryCacheSW(Memory* memory, Cache* cache, unsigned int address, unsigned count, unsigned int answerVal, bool *fetchDonePtr){
     if(cacheOn){
		 if(address == 0xFF){
			 for(int i = 0; i < 8; i++){
				if(cache->flags[i] == 'W'){
					byteToWrite = true;
				}
			}
			if(byteToWrite){
				beginMemSet(memory, address, 8, answerVal, fetchDonePtr);
			}		 
		 }
		 
		 else{
		 requestedCLO = address >> 3;
			if(requestedCLO != cache->CLO){
				for(int i = 0; i < 8; i++){
						if(cache->flags[i] == 'W'){
						byteToWrite = true;
					}
				}
				if(byteToWrite){
					beginMemSet(memory, address, 8, answerVal, fetchDonePtr);
				}
			}
			else{
				if(!cache->validData){
				cache->data[address - requestedCLO] = answerVal;
				cache->flags[address - requestedCLO] = 'W';
			}
	     	 	} 
		 }
	}
	
    else{//cache is off, pass request on to memory
		 beginMemSet(memory, address, count, answerVal, fetchDonePtr);
	}
}
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
