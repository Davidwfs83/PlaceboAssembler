/*
 * file_manager.c
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "file_manager.h"
#include "memory_buffer.h"
#include "utility.h"

void verifyAndOpenFiles(int numFiles, char *filenames[], struct FilePacket *fileInputs) {

	// declare variables for loop
	char filename_with_extension[MAX_FILENAME_LENGTH];
	size_t filename_length;
    for (int i = 0; i < numFiles; i++) {
        // Get the length of the filename
        filename_length = strlen(filenames[i]);

        // SIDE NOTE: HERE WE MADE THE ASSUMPTION THAT FILES PROVIDED ARE INDEED OF TYPE
        // .as AND WE ALLOW THE USER TO CHOOSE TO SPECIFY THE PATH WITH THE EXTENSION OR WITHOUT

        // Check if the filename already ends with ".as" or has an extension
        if (filename_length >= 3 && strcmp(&filenames[i][filename_length - 3], ".as") == 0) {
            strcpy(filename_with_extension, filenames[i]);
        } else {
            strcpy(filename_with_extension, filenames[i]);
            strcat(filename_with_extension, ".as");
        }

        FILE *file = fopen(filename_with_extension, "r");

        if (file != NULL) {

            fileInputs[i].InputFile = file;
        } else {

        	fileInputs[i].InputFile = NULL;
        	errorObjInserter(&fileInputs[i],FILE_OPENING_ERR,GENERAL_ERROR , 0);
            return;
        }
        // Remove the extension from filename_with_extension before storing it into filename
        filename_with_extension[filename_length - 3] = '\0';
        strcpy(fileInputs[i].filename, filename_with_extension);
        fileInputs[i].PreAssFile = NULL;
        fileInputs[i].AssFile = NULL;
        fileInputs[i].ErrCount = 0;
        fileInputs[i].Errors = NULL;
    }
}

// Function to convert the buffer to a Base64 string
void bufferToBase64(const char *buffer, char *base64String) {
    int bufferLength = strlen(buffer);


    int base64Index = 0;
    for (int i = 0; i < bufferLength; i += 12) {
        char binaryStr[13]; // 12 bits + null-terminator
        strncpy(binaryStr, buffer + i, 12);
        binaryStr[12] = '\0';

        base64String[base64Index++] = toBase64(binaryStr);
        base64String[base64Index++] = toBase64(binaryStr + 6); // Second 6 bits
    }
    base64String[base64Index] = '\0'; // Null-terminate the Base64 string
}

// Function to write Base64 string, instruction count, and data count to a file
void writeBase64ToObjectFile(const char *buffer, FilePacket *filePack, int instructionCount, int dataCount) {
    // Write instruction count and data count on the first line
    fprintf(filePack->AssFile, "%d %d\n", instructionCount, dataCount);

    // Create a Base64 string to hold the result
    char base64String[(strlen(buffer) * 4 + 2) / 3 + 1];

    // Convert the buffer to a Base64 string
    bufferToBase64(buffer, base64String);

    // Write the Base64 string to the file
    fprintf(filePack->AssFile, "%s\n", base64String);
}
