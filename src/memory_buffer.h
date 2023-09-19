#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H

#include "constants.h"

#define MEMORY_SIZE 1024
#define BIT_GROUP_SIZE 12

typedef struct {
	int instructionSegmentSize;
    int insCounter;  // Index for instruction segment
    int dataCounter; // Index for data segment
    char memory[MEMORY_SIZE * BIT_GROUP_SIZE + 1]; // +1 for null terminator
} MemoryBuffer;

void initMemoryBuffer(MemoryBuffer *buffer, int instructionSegSize);

int isBinary(char c);

void instructionInsertToMemory(MemoryBuffer *buffer, const char *bits);

void dataInsertToMemory(MemoryBuffer *buffer, const char *bits);

#endif /* MEMORY_BUFFER_H */


