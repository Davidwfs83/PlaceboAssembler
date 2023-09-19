#include <string.h>
#include "memory_buffer.h"

void initMemoryBuffer(MemoryBuffer *buffer,int instructionSegSize ) {
    buffer->insCounter = 0;
    buffer->instructionSegmentSize = instructionSegSize;
    buffer->dataCounter = instructionSegSize;
    buffer->memory[0] = '\0';
}

int isBinary(char c) {
    return c == '0' || c == '1';
}

void instructionInsertToMemory(MemoryBuffer *buffer, const char *bits) {
    if (strlen(bits) != BIT_GROUP_SIZE || buffer->insCounter + 1 > buffer->instructionSegmentSize) {
        return;
    }

    for (int i = 0; i < BIT_GROUP_SIZE; i++) {
        if (!isBinary(bits[i])) {
            return;
        }
    }

    int index = buffer->insCounter * BIT_GROUP_SIZE;
    strncpy(buffer->memory + index, bits, BIT_GROUP_SIZE);
    buffer->insCounter++;
}

void dataInsertToMemory(MemoryBuffer *buffer, const char *bits) {
    if (strlen(bits) != BIT_GROUP_SIZE || buffer->dataCounter + 1 > MEMORY_SIZE) {
        return;
    }

    for (int i = 0; i < BIT_GROUP_SIZE; i++) {
        if (!isBinary(bits[i])) {
            return;
        }
    }

    int index = (buffer->instructionSegmentSize + buffer->dataCounter) * BIT_GROUP_SIZE;
    strncpy(buffer->memory + index, bits, BIT_GROUP_SIZE);
    buffer->dataCounter++;

    if (buffer->dataCounter == MEMORY_SIZE) {
        buffer->memory[buffer->dataCounter * BIT_GROUP_SIZE] = '\0';
    }
}
