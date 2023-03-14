#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Clock.h"
#include "CPU.h"
#include "Memory.h"
#include "IMemory.h"


void iMemCreate(IMemory* imemory, unsigned int size){
	imemory->size = size;
    imemory->ptr = (unsigned int*) malloc(sizeof(unsigned int)*size);
}



void iMemReset(IMemory* imemory){
	unsigned int i;
	for (i = 0; i < imemory->size; i++) { 
        imemory->ptr[i] = 0;
    }
}


void iMemDump(IMemory* imemory, unsigned int hexaddress, unsigned int hexcount){
	 //dump and format all of the allocated memory
		printf("\nAddr     0     1     2     3     4     5     6     7\n");
		
		unsigned int i;
		for(i = 0; i < hexaddress + hexcount; i++){
			if(i % 8 == 0){
			   printf("0x%02X ", i);
			}
			
			if(i < hexaddress){
			printf("     ");
			}
			
			else 
			if((i+1) % 8 == 0){
				printf("%X\n", imemory->ptr[i]);
			}
			else{
				printf("%X ", imemory->ptr[i]);
			}
			
		}
		
		printf("\n");
	
}
	

void iMemSet(IMemory* imemory, unsigned int hexaddress, char file[]){
	unsigned int value;
	int i = 0;
	FILE* fp2 = fopen(file, "r");
    while(fscanf(fp2, "%x", &value) == 1){
			imemory->ptr[hexaddress+i] = value;
			i = i + 1;
	}
	
	fclose(fp2);
	fp2 = NULL;
	
	
}
