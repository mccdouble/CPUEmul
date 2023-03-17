#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"

bool memTickStarted = false;
bool memMoreWork = false;
bool fetching = false;
bool setting = false;
unsigned int* savedAnswer;
unsigned int  memAddress, storeVal;
bool* fetchOrSetDone;
int memCount = 0; 
int dataCount = 0;

void memStartTick(){
	memTickStarted = true;
	memCount = memCount + 1;
}


void memDoCycleWork(CPU* cpu, Memory* memory, Cache* cache){
	if(memCount == 5){
		memCount = 0;
		//fetching command
		if(fetching == true){ 
			if(dataCount == 1){ //just returning 1 byte
				*savedAnswer = memory->ptr[memAddress];
				*fetchOrSetDone = true; //done fetching
				memMoreWork = false;
				fetching = false;
			}
			
			else
			{//filling cache and returning requested byte to cpu
				
				int cloTemp = memAddress >> 3;
				int startAddress = cloTemp << 3; //to get start of line for the cache
				int i;
				int j = 0;
				for(i = startAddress; i < startAddress + dataCount; i++){
					cache->data[j] = memory->ptr[i];
					j = j+1;
				}
				
				cache->CLO = cloTemp;		
				cache->validData = true;
				*savedAnswer = memory->ptr[memAddress]; //returns to cpu the requested value
				*fetchOrSetDone = true;
				memMoreWork = false;
				fetching = false;
			
			}
		}
		
	
		//setting command	
		else {
		if(setting == true){

			if(dataCount == 1){
				memory->ptr[memAddress] = storeVal;
				*fetchOrSetDone = true; //done setting
				memMoreWork = false;
				setting = false;
			}
		}	
			else
			{//setting memory with flagged write bytes in cache
				int j = (memAddress >> 3) << 3; //starting address to write to memory
				//setting memory where flagged
				for(int i = 0; i < 8; i++){
					if(cache->flags[i] == 'W'){
						memory->ptr[j] = cache->flags[i];
					}
					j++;
				}
				//resets flags to no W
				for(int i = 0; i < 8; i++){
					cache->flags[i] = '\0';
				}
				cache->CLO = memAddress >> 3;
				int index = (memAddress >> 3) << 3;
				cache->data[memAddress - index] = storeVal;
				cache->flags[memAddress - index] = 'W';
				*fetchOrSetDone = true; //done setting
				memMoreWork = false;
				setting = false;
		}
	}
  }
}

bool memIsMoreCycleWorkNeeded(){
	return false; //since there is only ever 1 chance to do work during cycle for assignment 2
}


//sets proper variables to fetch from memory
void beginMemFetch(Memory* memory, unsigned int address, unsigned int count, unsigned int* answer, bool* waitState){
	fetching = true; //tells memDoCycleWork what work its supposed to be doing
        memAddress = address;
	dataCount = count;
        savedAnswer = answer; //pointer to where the retreived answer is to go
        *waitState = false;
        fetchOrSetDone = waitState; 
 	memCount = 0;
}
 
//sets proper variables to set memory
void beginMemSet(Memory* memory, unsigned int address, unsigned int count, unsigned int value, bool* waitState){
	setting = true;
	storeVal = value; //value needed to store
	memAddress = address;
	dataCount = count;
	*waitState = false;
	fetchOrSetDone = waitState; //local pointer to cpu wait state
	memCount = 0;
}


void memCreate(Memory* memory, unsigned int size){
	memory->size = size;
	memory->ptr = (unsigned int*) malloc(sizeof(unsigned int)*size);
}

void memReset(Memory* memory){	
	unsigned int i;
	for (i = 0; i < memory->size; i++) { 
		memory->ptr[i] = 0;
	}
}


void memDump(Memory* memory, unsigned int hexaddress, unsigned int hexcount){
	//dump and format all of the allocated memory
	printf("Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");

	unsigned int i;
	for(i = 0; i < hexaddress + hexcount; i++){
		if(i % 16 == 0){
		   printf("0x%02X ", i);
		}
		if(i < hexaddress){
		  printf("   ");
		}
		else
		if((i + 1) % 16 == 0){
			printf("%02X\n", memory->ptr[i]);
		}
		else{
			printf("%02X ", memory->ptr[i]);
		}
	}

	printf("\n");
}

//sets memory values
void memSet(Memory* memory, unsigned int hexaddress, unsigned int hexcount, unsigned int values[]){
	unsigned int i;
	for (i = hexaddress; i < (hexaddress + hexcount); i++) { 
		memory->ptr[i] = values[i-hexaddress]; 
   	} 
		
}


