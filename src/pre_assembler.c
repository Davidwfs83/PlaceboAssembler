/*
 * pre_assembler.c
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "file_manager.h"
#include "constants.h"
#include "utility.h"

#include "binary_searchtree_string.h"

static TreeNode* macroTreeRoot = NULL; // Macros binary search tree
static char line[MAX_INPUTFILE_LINE_LENGTH];// Initialize Line Buffer
static int lineCount = 0;
static FilePacket *filePack; // File Pack




// Function to extract two words from a line
// Returns 1 on success, 0 if line is empty
int ExtractTwoWords(char *word1, char *word2) {
    // Initialize word indices
    size_t word_index = 0;

    // Iterate through the line characters
    for (size_t i = 0; line[i] != '\0'; i++) {
        // Skip leading whitespaces
        while (line[i] == ' ' || line[i] == '\t') {
            i++;
        }

        // Extract word
        size_t word_start = i;
        while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') {
            i++;
        }
        size_t word_length = i - word_start;

        // Store word based on index
        if (word_index == 0) {
            strncpy(word1, &line[word_start], word_length);
            word1[word_length] = '\0'; // Null-terminate the word
        } else if (word_index == 1) {
            strncpy(word2, &line[word_start], word_length);
            word2[word_length] = '\0'; // Null-terminate the word
        }

        // Increment word index
        word_index++;

        // Break if we have extracted two words
        if (word_index == 2) {
            break;
        }
    }

    // Handle the case of only one word
    if (word_index == 1) {
        word2[0] = '\0'; // Reset word2 to an empty string
    }

    // Return whether any words were extracted
    return word_index > 0;
}

// Reads a macro content and returns a string
char *readMacroContent() {
    char *macroContent = NULL;
    size_t macroContentLength = 0;
    int endmcroEncountered = 0; // Flag to track if "endmcro" is encountered

    while (fgets(line, sizeof(line), filePack->InputFile) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        // Check if the first word of the line is "endmcro"
        char *firstWord = line; // Start at the beginning of the line
        // Find the end of the first word
        while (*firstWord != '\0' && (*firstWord == ' ' || *firstWord == '\t')) {
            firstWord++; // Skip leading whitespace
        }
        char *endOfFirstWord = firstWord;
        while (*endOfFirstWord != '\0' && *endOfFirstWord != ' ' && *endOfFirstWord != '\t') {
            endOfFirstWord++; // Scan to the end of the first word
        }

        // Check if the first word is "endmcro"
        if (strncmp(firstWord, "endmcro", endOfFirstWord - firstWord) == 0) {
            endmcroEncountered = 1;
            break; // Stop reading when "endmcro" is encountered
        }

        // Concatenate the line to the macroContent
        macroContent = realloc(macroContent, macroContentLength + len + 1);
        if (macroContent == NULL) {
            errorObjInserter(filePack, MEM_ALLOCATION_ERR, GENERAL_ERROR, 0);
            return NULL;
        }
        strcpy(macroContent + macroContentLength, line);
        strcat(macroContent + macroContentLength, "\n"); // Add a newline
        macroContentLength += len + 1; // +1 for newline
    }

    // Check if end of file is reached without encountering "endmcro"
    if (!endmcroEncountered && feof(filePack->InputFile)) {
    	errorObjInserter(filePack,MISSING_END_MACRO_STATEMENT_ERR,PREASSEMBLY_ERROR , lineCount);
    	return NULL;
    }
    macroContent[macroContentLength] = '\0';
    return macroContent;
}


// Clean up macro content
void cleanupMacroContent(char *macroContent) {
    if (macroContent != NULL) {
        free(macroContent);
    }
}


// the main function of this file/ Entry point
/*
 * Side Note 1: We work here under the assumption that a macro has no label
 * Side Note 2:
 */

void preAssembler(FilePacket *sourceFilePack) {

	filePack = sourceFilePack;

	// Creation of output file
	FILE *preAssembledFile = createOutputFileWithExtension(filePack,".am", "+w");
	    if (preAssembledFile == NULL) {
	    	errorObjInserter(filePack,MEM_ALLOCATION_ERR,GENERAL_ERROR , 0);
	    	return;
	    }

    // Variables Initializations



    char word1 [MAX_WORD_LENGTH], word2 [MAX_WORD_LENGTH]; // First Array to possibly hold the "mcro" keyword, Second array to possibly hold the macro name
    char *macroContentP;
    TreeNode * expandedMacro;


    while (fgets(line, sizeof(line), filePack->InputFile) != NULL) {

    	lineCount++;
    	// Extract 2 first words from the line
    	ExtractTwoWords(word1, word2); // word1 - Possible macro keyword, word2 - possbile macro name

    	// Check if we are inside a macro definition
    	if (strcmp(word1, "mcro") == 0) {

    		// We check if macro name is in correct format

    		if (!firstAlphaRestAlphaNum(word2))
    		{
    		    errorObjInserter(filePack,INVALID_MACRO_NAME_ERR,PREASSEMBLY_ERROR , lineCount);
    		    return;
    		}

    		//  we check if the macro provided isnt a procedure name
    		if (!isNotProcedureName(word2))
    		{
    			errorObjInserter(filePack,INVALID_MACRO_NAME_ERR,PREASSEMBLY_ERROR , lineCount);
    			return;
    		}

    		// Now we check if the macro name provided dosent already exist in tree and if so we throw an error
    		if (searchNode(macroTreeRoot, word2) != NULL) {
    			errorObjInserter(filePack,REDIFINITION_OF_EXISTING_MACRO_ERR,PREASSEMBLY_ERROR , lineCount);
    		    return;
    		}

    		macroContentP =readMacroContent();
    		// Check for Errors in ReadMacroContent
    		if (macroContentP == NULL)
    			break;
    		// Insert macro name into tree
    		macroTreeRoot = insertNode(macroTreeRoot, word2, macroContentP);
    		// Cleanup macro conent allocated memory
    		cleanupMacroContent(macroContentP);
    		continue;
    	}
    	// If we reached here means were not inside a macro definiton
    	// We check if its macro expanion but seeing if there is only one word and if its already in the tree
    	if ((expandedMacro = searchNode(macroTreeRoot, word1)) != NULL && word2[0] == '\0')
    	{
    		fputs(expandedMacro->value, preAssembledFile);
    		continue;
    	}


        // if we reached here meaning its just a regular line
    	if (fputs(line, preAssembledFile) == EOF) {
    	    fprintf(stderr, "Error writing to file: %s\n", strerror(errno));
    	    // Handle the error as needed
    	}



    }
// Free memory used by macroTree
    freeTree(macroTreeRoot);

    // Assign preAssembledFile to PreAssFile in filePack
    filePack->PreAssFile = preAssembledFile;


}
