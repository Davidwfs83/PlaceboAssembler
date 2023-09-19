/*
 * utility.c
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "file_manager.h"

// Function to print errors stored in an array
void printErrors(const ErrorObj *errors, size_t count) {
	for (size_t i = 0; i < count; i++) {
		const ErrorObj *error = &errors[i];
		if (error->lineNumber > 0) {
			printf("<Line:%d>%s\n", error->lineNumber, error->errorMessage);
		} else {
			printf("%s\n", error->errorMessage);
		}
	}
}
// Function to print the details of a FilePacket
void printFilePacket(const FilePacket *filePacket) {
	printf("Assembling file: <%s>\n", filePacket->filename);

	// Check for errors
	if (filePacket->ErrCount > 0) {
		printf("Errors:\n");
		printErrors(filePacket->Errors, filePacket->ErrCount);
	} else {
		if (filePacket->PreAssFile != NULL) {
			printf("Pre Assembled File || STATUS: SUCCESS || <%s>\n",
					filePacket->filename);
		} else {
			printf("Pre Assembled File || STATUS: FAILURE\n");
		}
	}
}

// Function to check if a string doesn't match any procedure name
int isNotProcedureName(const char *str) {
	for (int i = 0; i < MAX_NUMBER_OF_PROCEDURES; i++) {
		if (strcmp(str, PROCEDURE_INFO[i].name) == 0) {
			// The string matches a procedure name, return 0 (false)
			return 0;
		}
	}
	// The string doesn't match any procedure name, return 1 (true)
	return 1;
}

// string validator function checking if first characeters is alpha and the rest alphanumeric
int firstAlphaRestAlphaNum(const char *str) {
	if (str == NULL || str[0] == '\0') {
		return 0; // Empty string is not valid
	}

	// Check if the first character is a letter
	if (!isalpha(str[0])) {
		return 0;
	}

	// Check the rest of the characters
	for (size_t i = 1; str[i] != '\0'; i++) {
		if (!isalnum(str[i])) {
			return 0;
		}
	}

	return 1; // The string follows the format
}

// Function to check if a string starts with a given prefix
int startsWith(const char *str, const char *prefix) {
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Function to check if a string is a valid number
int isNumber(const char *str) {
	if (str == NULL || str[0] == '\0') {
		return 0; // Empty string is not a number
	}

	for (size_t i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i])) {
			return 0; // If any character is not a digit, it's not a number
		}
	}

	return 1; // The string consists of digits only
}

// Function to create and validate the output file
FILE *createOutputFileWithExtension(FilePacket *filePack, const char *extension,
		const char *permission) {


	char *outputFileName = (char *) malloc(
			strlen(filePack->filename) + strlen(extension) + 5);

	if (outputFileName == NULL) {
		errorObjInserter(filePack, MEM_ALLOCATION_ERR, GENERAL_ERROR, 0);
		return NULL;
	}



	strcpy(outputFileName, filePack->filename);
	strcat(outputFileName, extension); // Add the extension


	FILE *openedFile = fopen(outputFileName, permission);
	if (openedFile == NULL) {
		errorObjInserter(filePack, FILE_CREATION_ERR, GENERAL_ERROR, 0);
		free(outputFileName); // Free allocated memory for file name
		return NULL;
	}

	free(outputFileName); // Free allocated memory for file name
	return openedFile;
}
int extractRegisterNumber(const char *str) {
	if (str == NULL || strlen(str) < 4 || str[0] != '@' || str[1] != 'r') {
		return -1; // Not in the expected format
	}

	int num = atoi(str + 2); // Skip '@r' and convert the remaining part to an integer

	if (num >= 0 && num <= 7) {
		return num; // Valid register number
	} else {
		return -1; // Out of valid register range
	}
}

void intToFiveBitString(int value, char *bitString) {
	if (value < 0 || value > 31) {
		bitString[0] = '\0'; // Empty string for out-of-range value
		return;
	}

	for (int i = 0; i < 5; i++) {
		bitString[4 - i] = (value & (1 << i)) ? '1' : '0';
	}
	bitString[5] = '\0'; // Null-terminate the string
}
int intToTenBitString(int value, char *positionInArray) {
	if (value < 0 || value > 1023) {
		return 0; // Out of range
	}

	// Ensure that the positionInArray is not null
	if (positionInArray == NULL) {
		return 0;
	}

	for (int i = 0; i < 10; i++) {
		positionInArray[i] = (value & (1 << (9 - i))) ? '1' : '0';
	}
	positionInArray[10] = '\0'; // Null-terminate the string
	return 1;
}

// Generate the first machine word string word of every instruction line
void firstWordFactory(char* word, LineArgument sourceOperand,
		LineArgument destinationOperand, const char *opCodeBits) {
	// Check if the array has enough space for 12 characters
	if (strlen(word) >= BIT_GROUP_SIZE) {
		// First word always get 'A' absolute encoding a.k.a 00
		word[0] = '0';
		word[1] = '0';

		// Set the source operand bits
		if (sourceOperand.val == NULL) {
			word[9] = '0';
			word[10] = '0';
			word[11] = '0';
		} else if (sourceOperand.addressMode == IMMEDIATE_ADDRESSING) {
			word[9] = '0';
			word[10] = '0';
			word[11] = '1';
		} else if (sourceOperand.addressMode == DIRECT_ADDRESSING) {
			word[9] = '0';
			word[10] = '1';
			word[11] = '1';
		} else if (sourceOperand.addressMode == REGISTER_ADDRESSING) {
			word[9] = '1';
			word[10] = '0';
			word[11] = '1';
		}

		// Set the destination operand bits
		// Set the source operand bits
		if (destinationOperand.val == NULL) {
			word[2] = '0';
			word[3] = '0';
			word[4] = '0';
		}
		if (destinationOperand.addressMode == IMMEDIATE_ADDRESSING) {
			word[2] = '0';
			word[3] = '0';
			word[4] = '1';
		} else if (destinationOperand.addressMode == DIRECT_ADDRESSING) {
			word[2] = '0';
			word[3] = '1';
			word[4] = '1';
		} else if (destinationOperand.addressMode == REGISTER_ADDRESSING) {
			word[2] = '1';
			word[3] = '0';
			word[4] = '1';
		}

		// Set opcode bits
		for (int i = 0; i < 4 && opCodeBits[i] != '\0'; i++) {
			word[5 + i] = opCodeBits[i];
		}
		word[12] = '/0';
	}
}

// Generate the second and third words for instruction line
void AddditionalWordFactory(char* word, LineArgument sourceOperand,
		LineArgument destinationOperand, int externalFlag, int labelVal) {

	int destinationRegisterNumb = 0, sourceRegisterNumb = 0;
	// Check if the array has enough space for 12 characters
	if (strlen(word) >= BIT_GROUP_SIZE) {
		// Meaning its a 2 register operands additional word
		if (sourceOperand.val != NULL && destinationOperand.val != NULL) {
			sourceRegisterNumb = extractRegisterNumber(sourceOperand.val);
			destinationRegisterNumb = extractRegisterNumber(
					destinationOperand.val);
			// In this case the 2 first big are 'A' or 00
			word[0] = '0';
			word[1] = '0';
			intToFiveBitString(sourceRegisterNumb, word + 7);
			intToFiveBitString(destinationRegisterNumb, word + 2);
		} else if (sourceOperand.val == NULL && destinationOperand.val != NULL) {
			if (destinationOperand.addressMode == IMMEDIATE_ADDRESSING) {
				// In this case the 2 first bit are 'A' or 00
				word[0] = '0';
				word[1] = '0';
				intToTenBitString(atoi(destinationOperand.val), word + 2);
			} else if (destinationOperand.addressMode == DIRECT_ADDRESSING) {
				// In this case the 2 first bit are 'E' or 01
				if (externalFlag) {
					word[0] = '0';
					word[1] = '1';

				} else {
					// In this case the 2 first bit are 'R' or 10
					word[0] = '1';
					word[1] = '0';
				}
				intToTenBitString(atoi(labelVal), word + 2);
			} else if (destinationOperand.addressMode == REGISTER_ADDRESSING) {
				// In this case the 2 first bit are 'A' or 00
				destinationRegisterNumb = extractRegisterNumber(
						destinationOperand.val);
				word[0] = '0';
				word[1] = '0';
				intToFiveBitString(destinationRegisterNumb, word + 2);
				for (int i = 7; i < 5; i++)
					word[i] = '0';

				intToTenBitString(atoi(labelVal), word + 2);
			}
		} else if (sourceOperand.val != NULL && destinationOperand.val == NULL) {
			// In this case the 2 first bit are 'A' or 00
			destinationRegisterNumb = extractRegisterNumber(
					destinationOperand.val);
			word[0] = '0';
			word[1] = '0';
			intToFiveBitString(destinationRegisterNumb, word + 7);
			for (int i = 2; i < 5; i++)
				word[i] = '0';

			intToTenBitString(atoi(labelVal), word + 7);
		}

		word[12] = '/0';
	}
}

// Function to transform a character into a 12-bit ASCII string
void charTo12BitString(char c, char *bitString) {
    snprintf(bitString, 13, "%012d", c);
}

// Function to convert a 12-bit binary string to a Base64 character
char toBase64(const char *binaryStr) {
    // Base64 character mapping
    const char *base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Convert the 12-bit binary string to an integer
    int value = 0;
    for (int i = 0; i < 12; i++) {
        value <<= 1;
        if (binaryStr[i] == '1') {
            value |= 1;
        }
    }

    // Get the corresponding Base64 character
    return base64Chars[value];
}

// Function to transform an integer into a 12-bit binary string
void intTo12BitBinary(int value, char *bitString) {
    snprintf(bitString, 13, "%012o", value);
}

