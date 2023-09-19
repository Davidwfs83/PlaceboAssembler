/*
 ============================================================================
 Name        : Assembler.c
 Author      : 
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "file_manager.h"
#include "pre_assembler.h"
#include "utility.h"
#include "machine_code_factory.h"

static FilePacket *filesPacket; // Global array of file packets

// Function to initialize FilePacket structures
void initializeFilePackets(int count) {
	for (int i = 0; i < count; i++) {
		filesPacket[i].InputFile = NULL;
		filesPacket[i].PreAssFile = NULL;
		filesPacket[i].AssFile = NULL;
		filesPacket[i].Errors = NULL;
		filesPacket[i].filename = malloc(MAX_FILENAME_LENGTH);
		filesPacket[i].ErrCount = 0;
		filesPacket[i].ErrorsCapacity = 0;
	}
}

// Function to clean up resources for a single FilePacket
void cleanupSingleFilePacket(FilePacket *filePacket) {
	if (filePacket->InputFile != NULL) {
		fclose(filePacket->InputFile);
		filePacket->InputFile = NULL; // Reset the pointer to NULL after closing
	}
	if (filePacket->PreAssFile != NULL) {
		fclose(filePacket->PreAssFile);
		filePacket->PreAssFile = NULL; // Reset the pointer to NULL after closing
	}
	if (filePacket->AssFile != NULL) {
		fclose(filePacket->AssFile);
		filePacket->AssFile = NULL; // Reset the pointer to NULL after closing
	}
	free(filePacket->filename);
	free(filePacket->Errors);
}

int main(int argc, char* argv[]) {
	// Check if Files were provided
	if (argc < 2) {
		printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
		return 1;
	}

	// Allocate memory for an array of FilePacket structures
	filesPacket = malloc((argc - 1) * sizeof(FilePacket));
	if (filesPacket == NULL) {
		perror("Memory allocation error");
		return 1;
	}

	// Initialize the FilePacket structures
	initializeFilePackets(argc - 1);

	// Open and Verify files
	verifyAndOpenFiles(argc - 1, &argv[1], filesPacket);
	for (int i = 0; i < argc - 1; i++) {
		// Check for Errors in the InputFile
		if (filesPacket[i].InputFile == NULL) {
			continue;
		}

		// Call PreAssembler
		preAssembler(&filesPacket[i]);

		// Check for Errors in the InputFile
		if (filesPacket[i].PreAssFile == NULL) {
			continue;
		}

		// Call machine code factory
		assembler(&filesPacket[i]);

		// Print file packet
		printFilePacket(&filesPacket[i]);

		// Clean file packet
		cleanupSingleFilePacket(&filesPacket[i]);
	}

	return 0;
}
