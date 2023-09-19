#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"
#include "machine_code_factory.h"
#include "file_manager.h"
#include "binary_searchtree_labels.h"
#include "utility.h"
#include "memory_buffer.h"

static MemoryBuffer *memBuffer; // Global static MemoryBuffer variable
static ITreeNode* instructionLabelTree = NULL; // Initialize an empty tree
static ITreeNode* entryLabelsTree = NULL; // For entries labels
static ITreeNode* externLabelsTree = NULL; // For extern labels
static ITreeNode* dataLabelTree = NULL; // Initialize an empty tree
static ITreeNode* mainLabelTree = NULL; // Initialize an empty tree
static char assemblyLine[MAX_INPUTFILE_LINE_LENGTH]; // Initialize Line Buffer
static FilePacket *filePack; // File Pack
static int lineCount = 0;
static LineObj lineObj;

void initLineObj() {
	// Initialize LineType
	lineObj.lineType = COMMENT_LINE;

	// Initialize ProcedureObj
	lineObj.procedureInfo.procedureDecimalIndex = 0;
	lineObj.procedureInfo.firstArg.addressMode = DIRECT_ADDRESSING;
	lineObj.procedureInfo.firstArg.val = NULL;
	lineObj.procedureInfo.secondArg.addressMode = DIRECT_ADDRESSING;
	lineObj.procedureInfo.secondArg.val = NULL;

	// Initialize DirectiveInfo
	lineObj.directiveInfo.type = UNKNOWN_DIRECTIVE;

	// Allocate memory for label name
	lineObj.labelObj.labelName = (char *) malloc(MAX_LABEL_NAME_LENGTH);
	if (lineObj.labelObj.labelName != NULL) {
		lineObj.labelObj.labelName[0] = '\0'; // Initialize as empty string
	}

	// Allocate memory for directive name
	lineObj.directiveInfo.name = (char *) malloc(MAX_DIRECTIVE_NAME_LENGTH);
	if (lineObj.directiveInfo.name != NULL) {
		lineObj.directiveInfo.name[0] = '\0'; // Initialize as empty string
	}

	// Allocate memory for stringValue
	lineObj.directiveInfo.stringValue = (char *) malloc(
	MAX_STRING_DIRECTIVE_VALUE);
	if (lineObj.directiveInfo.stringValue != NULL) {
		lineObj.directiveInfo.stringValue[0] = '\0'; // Initialize as empty string
	}

	// Allocate memory for procedureInfo first argument value
	lineObj.procedureInfo.firstArg.val = (char *) malloc(MAX_LABEL_NAME_LENGTH);
	if (lineObj.procedureInfo.firstArg.val != NULL) {
		lineObj.procedureInfo.firstArg.val[0] = '\0'; // Initialize as empty string
	}

	// Allocate memory for procedureInfo second argument value
	lineObj.procedureInfo.secondArg.val = (char *) malloc(
	MAX_LABEL_NAME_LENGTH);
	if (lineObj.procedureInfo.secondArg.val != NULL) {
		lineObj.procedureInfo.secondArg.val[0] = '\0'; // Initialize as empty string
	}
}
int validateLabel(const char *label) {
	if (label == NULL || label[0] == '\0') {
		return 0; // Empty label is not valid
	}

	// Check if the first character is a letter
	if (!isalpha((unsigned char )label[0])) {
		return 0;
	}

	// Check the rest of the characters
	for (size_t i = 1; label[i] != '\0'; i++) {
		if (!isalnum((unsigned char )label[i])) {
			return 0;
		}
	}

	return 1; // The label follows the format
}

// Function to free memory allocated for LineObj and its descendants
void cleanupLineObj() {
	// Free memory for label name
	if (lineObj.labelObj.labelName != NULL) {
		free(lineObj.labelObj.labelName);
	}

	// Free memory for directive name
	if (lineObj.directiveInfo.name != NULL) {
		free(lineObj.directiveInfo.name);
	}

	// Free memory for stringValue
	if (lineObj.directiveInfo.stringValue != NULL) {
		free(lineObj.directiveInfo.stringValue);
	}

	// Free memory for procedureInfo first argument value
	if (lineObj.procedureInfo.firstArg.val != NULL) {
		free(lineObj.procedureInfo.firstArg.val);
	}

	// Free memory for procedureInfo second argument value
	if (lineObj.procedureInfo.secondArg.val != NULL) {
		free(lineObj.procedureInfo.secondArg.val);
	}
}
// Function to process an directive
int processDirective(LineObj *lineObj, const char *directiveLine) {
	// Check if the directive is .string, .data, .entry, or .extern
	if (strstr(directiveLine, ".string") == directiveLine) {
		lineObj->directiveInfo.type = STRING_DIRECTIVE;

		// Extract the string value between double quotes
		char *quoteStart = strchr(directiveLine, '"');
		char *quoteEnd = quoteStart ? strchr(quoteStart + 1, '"') : NULL;
		if (quoteStart && quoteEnd) {
			// Allocate memory for stringValue and copy the string
			lineObj->directiveInfo.stringValue = strndup(quoteStart + 1,
					quoteEnd - quoteStart - 1);
		} else {
			errorObjInserter(filePack, UNVALID_DIRECTIVE_NAME, ASSEMBLY_ERROR,
					lineCount);
			return 0;
		}
	} else if (strstr(directiveLine, ".data") == directiveLine) {
		lineObj->directiveInfo.type = DATA_DIRECTIVE;

		// Extract and parse the integer values separated by commas and whitespaces
		char *valuesStart = strchr(directiveLine, '.');
		if (valuesStart) {
			valuesStart = strchr(valuesStart, ' '); // Find the space after the .data
			if (valuesStart) {
				valuesStart++; // Move past the space
				char *token = strtok(valuesStart, ", \t");
				int count = 0;
				int prevComma = 0; // Flag to check for consecutive commas
				while (token) {
					// Check for consecutive commas
					if (*token == ',' && prevComma) {
						errorObjInserter(filePack, DOUBLE_COMMA_ERR,
								ASSEMBLY_ERROR, lineCount);
						return 0;
					}
					if (*token == ',') {
						prevComma = 1;
					} else {
						prevComma = 0;
					}

					// Allocate memory for dataValues and store the integer value
					lineObj->directiveInfo.dataValues = realloc(
							lineObj->directiveInfo.dataValues,
							(count + 1) * sizeof(int));
					lineObj->directiveInfo.dataValues[count] = atoi(token);
					count++;
					token = strtok(NULL, ", \t");
				}
				lineObj->directiveInfo.dataValuesCount = count;
			}
		}
	} else if (strstr(directiveLine, ".entry") == directiveLine) {
		char *labelName = strtok(NULL, " "); // Get the next word after the directive
		if (labelName) {
			if (!firstAlphaRestAlphaNum(labelName)) {
				errorObjInserter(filePack, UNVALID_LABEL_NAME, ASSEMBLY_ERROR,
						lineCount);
				return 0;
			}
			lineObj->directiveInfo.type = ENTRY_DIRECTIVE;
			lineObj->labelObj.labelName = strdup(labelName);
			lineObj->labelObj.labelType = RELOCATABLE_LABEL; // Entry label is relocatable
		} else {
			errorObjInserter(filePack, UNVALID_DIRECTIVE_NAME, ASSEMBLY_ERROR,
					lineCount);
			return 0;
		}
	} else if (strstr(directiveLine, ".extern") == directiveLine) {
		char *labelName = strtok(NULL, " "); // Get the next word after the directive
		if (labelName) {
			if (!firstAlphaRestAlphaNum(labelName)) {
				errorObjInserter(filePack, UNVALID_LABEL_NAME, ASSEMBLY_ERROR,
						lineCount);
				return 0;
			}
			lineObj->directiveInfo.type = EXTERN_DIRECTIVE;
			lineObj->labelObj.labelName = strdup(labelName);
			lineObj->labelObj.labelVal = 0;
			lineObj->labelObj.labelType = EXTERNAL_LABEL; // External label
		} else {
			// Handle other directive types here
			// ...
			errorObjInserter(filePack, UNVALID_DIRECTIVE_NAME, ASSEMBLY_ERROR,
					lineCount);
			return 0;
		}
	}
	return 1;
}

// Function to process an instruction
int processInstruction(LineObj *lineObj, const char *instructionLine) {
	// Find the corresponding procedure index
	int procedureIndex = -1;
	for (int i = 0; i < MAX_NUMBER_OF_PROCEDURES; i++) {
		if (strcmp(instructionLine, PROCEDURE_INFO[i].name) == 0) {
			procedureIndex = i;
			break;
		}
	}

	if (procedureIndex == -1) {
		errorObjInserter(filePack, UNVALID_PROCEDURE_NAME, ASSEMBLY_ERROR,
				lineCount);
		return 0;
	}

	lineObj->procedureInfo.procedureDecimalIndex = procedureIndex;

	// Determine the number of allowed arguments
	int allowedArgs = PROCEDURE_INFO[procedureIndex].numberOfAllowedArgs;

	// Extract arguments based on the procedure's allowed number of args
	if (allowedArgs == 0) {
		// No arguments allowed
		lineObj->procedureInfo.firstArg.addressMode = DIRECT_ADDRESSING;
		lineObj->procedureInfo.firstArg.val = NULL;
		lineObj->procedureInfo.secondArg.addressMode = DIRECT_ADDRESSING;
		lineObj->procedureInfo.secondArg.val = NULL;
	} else if (allowedArgs == 1) {
		// Extract the first argument
		char *firstArgToken = strtok(NULL, " ");
		if (firstArgToken) {
			// Check for consecutive commas
			if (strcmp(firstArgToken, ",") == 0) {
				errorObjInserter(filePack, DOUBLE_COMMA_ERR, ASSEMBLY_ERROR,
						lineCount);
				return 0;
			}

			// Determine addressing mode based on the argument type
			AddressingMode addressMode;
			if (isNumber(firstArgToken)) {
				addressMode = IMMEDIATE_ADDRESSING;
			} else if (startsWith(firstArgToken, "@r")
					&& isNumber(firstArgToken + 2)) {
				addressMode = REGISTER_ADDRESSING;
			} else if (validateLabel(firstArgToken)) {
				addressMode = DIRECT_ADDRESSING;
			} else {
				errorObjInserter(filePack, UNVALID_ADDRESING_MODE_SOUCE_OPERAND,
						ASSEMBLY_ERROR, lineCount);
				return 0;
			}

			lineObj->procedureInfo.firstArg.addressMode = addressMode;
			lineObj->procedureInfo.firstArg.val = strdup(firstArgToken);
		} else {
			errorObjInserter(filePack, MISSING_PROCEDURE_ARGUMENT,
					ASSEMBLY_ERROR, lineCount);
			return 0;
		}

		lineObj->procedureInfo.secondArg.addressMode = DIRECT_ADDRESSING;
		lineObj->procedureInfo.secondArg.val = NULL;
	} else if (allowedArgs == 2) {
		// Extract the first argument
		char *firstArgToken = strtok(NULL, " ");
		if (firstArgToken) {
			// Check for consecutive commas
			if (strcmp(firstArgToken, ",") == 0) {
				errorObjInserter(filePack, DOUBLE_COMMA_ERR, ASSEMBLY_ERROR,
						lineCount);
				return 0;
			}

			// Determine addressing mode based on the argument type
			AddressingMode addressMode;
			if (isNumber(firstArgToken)) {
				addressMode = IMMEDIATE_ADDRESSING;
			} else if (startsWith(firstArgToken, "@r")
					&& isNumber(firstArgToken + 2)) {
				addressMode = REGISTER_ADDRESSING;
			} else if (validateLabel(firstArgToken)) {
				addressMode = DIRECT_ADDRESSING;
			} else {
				errorObjInserter(filePack, UNVALID_ADDRESING_MODE_SOUCE_OPERAND,
						ASSEMBLY_ERROR, lineCount);
				return 0;
			}

			lineObj->procedureInfo.firstArg.addressMode = addressMode;
			lineObj->procedureInfo.firstArg.val = strdup(firstArgToken);
		} else {
			errorObjInserter(filePack, MISSING_PROCEDURE_ARGUMENT,
					ASSEMBLY_ERROR, lineCount);
			return 0;
		}

		// Extract the second argument
		char *secondArgToken = strtok(NULL, " ");
		if (secondArgToken) {
			// Check for consecutive commas
			if (strcmp(secondArgToken, ",") == 0) {
				errorObjInserter(filePack, DOUBLE_COMMA_ERR, ASSEMBLY_ERROR,
						lineCount);
				return 0;
			}

			// Determine addressing mode based on the argument type
			AddressingMode addressMode;
			if (isNumber(secondArgToken)) {
				addressMode = IMMEDIATE_ADDRESSING;
			} else if (startsWith(secondArgToken, "@r")
					&& isNumber(secondArgToken + 2)) {
				addressMode = REGISTER_ADDRESSING;
			} else if (validateLabel(secondArgToken)) {
				addressMode = DIRECT_ADDRESSING;
			} else {
				errorObjInserter(filePack,
						UNVALID_ADDRESING_MODE_DESTINATION_OPERAND,
						ASSEMBLY_ERROR, lineCount);
				return 0;
			}

			lineObj->procedureInfo.secondArg.addressMode = addressMode;
			lineObj->procedureInfo.secondArg.val = strdup(secondArgToken);
		} else {
			errorObjInserter(filePack, MISSING_PROCEDURE_ARGUMENT,
					ASSEMBLY_ERROR, lineCount);
			return 0;
		}
	}

	return 0;
}

// Function to determine how many machine words are needed for a LineObj
int howManyMachineWord() {
	if (lineObj.lineType != INSTRUCTION_LINE) {
		// The LineObj should be of type INSTRUCTION_LINE
		return -1; // Return an error code or handle this case accordingly
	}

	int procedureIndex = lineObj.procedureInfo.procedureDecimalIndex;
	const ProcedureInfo *procedure = &PROCEDURE_INFO[procedureIndex];

	if (procedure->numberOfAllowedArgs == 0) {
		return 1;
	} else if (procedure->numberOfAllowedArgs == 1) {
		return 2;
	} else if (procedure->numberOfAllowedArgs == 2) {
		// Check the addressing modes of the arguments
		AddressingMode firstArgMode = lineObj.procedureInfo.firstArg.addressMode;
		AddressingMode secondArgMode =
				lineObj.procedureInfo.secondArg.addressMode;

		if (firstArgMode == REGISTER_ADDRESSING
				&& secondArgMode == REGISTER_ADDRESSING) {
			return 2;
		} else {
			return 3;
		}
	} else {
		// Handle invalid procedure number of allowed arguments
		return -1; // Return an error code or handle this case accordingly
	}
}
void resetLineObj() {
	// Reset LineType
	lineObj.lineType = COMMENT_LINE;

	// Reset ProcedureObj
	lineObj.procedureInfo.procedureDecimalIndex = 0;
	lineObj.procedureInfo.firstArg.addressMode = DIRECT_ADDRESSING;
	lineObj.procedureInfo.firstArg.val[0] = '\0';
	lineObj.procedureInfo.secondArg.addressMode = DIRECT_ADDRESSING;
	lineObj.procedureInfo.secondArg.val[0] = '\0';

	// Reset DirectiveInfo
	lineObj.directiveInfo.type = UNKNOWN_DIRECTIVE;
	lineObj.directiveInfo.name[0] = '\0';
	lineObj.directiveInfo.stringValue[0] = '\0';

	// Reset LabelObj
	lineObj.labelObj.labelName[0] = '\0';
	lineObj.labelObj.labelType = RELOCATABLE_LABEL;
	lineObj.labelObj.labelVal = 0;
}
// Function to count elements based on directive type
int countElementsInDirective() {
	if (lineObj.lineType != DIRECTIVE_LINE) {
		return -1; // Return an error code or handle this case accordingly
	}

	if (lineObj.directiveInfo.type == STRING_DIRECTIVE) {
		// Calculate the length of the string value
		return strlen(lineObj.directiveInfo.stringValue);
	} else if (lineObj.directiveInfo.type == DATA_DIRECTIVE) {
		// Count the number of int elements
		return lineObj.directiveInfo.dataValuesCount;
	} else {
		// Handle other directive types if needed
		return -1; // Return an error code or handle this case accordingly
	}
}
// Functions that adds to the buffer the appropiate machine code for an instruction line
void MachineWordsForInstructionHandler() {
	char firstWord[BIT_GROUP_SIZE + 1]; // +1 for null-terminator
	char additionalWord1[BIT_GROUP_SIZE + 1];
	char additionalWord2[BIT_GROUP_SIZE + 1];

	int externalFirstLabelFlag = 0;
	int externalSecondLabelFlag = 0;
	ITreeNode* firstLabelNode = NULL;
	ITreeNode* secondLabelNode = NULL;
	LineArgument placeboArgument;
	placeboArgument.val = NULL;

	if (lineObj.procedureInfo.firstArg.addressMode == DIRECT_ADDRESSING) {
		firstLabelNode = IsearchNode(mainLabelTree,
				lineObj.procedureInfo.firstArg.val);
	}
	if (lineObj.procedureInfo.secondArg.addressMode == DIRECT_ADDRESSING) {
		secondLabelNode = IsearchNode(mainLabelTree,
				lineObj.procedureInfo.secondArg.val);
	}
	firstWordFactory(firstWord, lineObj.procedureInfo.firstArg,
			lineObj.procedureInfo.secondArg,
			PROCEDURE_INFO[lineObj.procedureInfo.procedureDecimalIndex].machineCode);

	// Number of allowed arguments is 1
	if (PROCEDURE_INFO[lineObj.procedureInfo.procedureDecimalIndex].numberOfAllowedArgs
			== 1) {

		firstWordFactory(firstWord, lineObj.procedureInfo.firstArg,
				lineObj.procedureInfo.secondArg,
				PROCEDURE_INFO[lineObj.procedureInfo.procedureDecimalIndex].machineCode);
		if (lineObj.procedureInfo.firstArg.addressMode == IMMEDIATE_ADDRESSING)
			AddditionalWordFactory(additionalWord1,
					lineObj.procedureInfo.secondArg,
					lineObj.procedureInfo.firstArg, 0, 0);
		else if (lineObj.procedureInfo.firstArg.addressMode
				== DIRECT_ADDRESSING) {

			if (firstLabelNode->value->labelType == EXTERNAL_LABEL) {
				externalFirstLabelFlag = 1;
			}
			AddditionalWordFactory(additionalWord1,
					lineObj.procedureInfo.secondArg,
					lineObj.procedureInfo.firstArg, externalFirstLabelFlag,
					firstLabelNode->value->labelVal);
		} else if (lineObj.procedureInfo.firstArg.addressMode
				== REGISTER_ADDRESSING) {
			AddditionalWordFactory(additionalWord1,
					lineObj.procedureInfo.secondArg,
					lineObj.procedureInfo.firstArg, 0, 0);
		}
	}
	// Number of allowed arguments is 2
	else if (PROCEDURE_INFO[lineObj.procedureInfo.procedureDecimalIndex].numberOfAllowedArgs
			== 2) {
		// 2 registers that merge into a single word
		if (lineObj.procedureInfo.firstArg.addressMode == REGISTER_ADDRESSING
				&& lineObj.procedureInfo.secondArg.addressMode
						== REGISTER_ADDRESSING) {
			AddditionalWordFactory(additionalWord1,
					lineObj.procedureInfo.firstArg,
					lineObj.procedureInfo.secondArg, 0, 0);

		}
		// actualy 2 additional word each for each operand
		else {
			AddditionalWordFactory(additionalWord1, placeboArgument,
					lineObj.procedureInfo.firstArg, externalFirstLabelFlag,
					firstLabelNode->value->labelVal);
			AddditionalWordFactory(additionalWord2, placeboArgument,
					lineObj.procedureInfo.secondArg, externalSecondLabelFlag,
					secondLabelNode->value->labelVal);
		}
	}

	// Insert wordS into the memory buffer
	if (strlen(firstWord) > 0) {
		dataInsertToMemory(memBuffer, firstWord);
	}

	if (strlen(additionalWord1) > 0) {
		dataInsertToMemory(memBuffer, additionalWord1);
	}

	if (strlen(additionalWord2) > 0) {
		dataInsertToMemory(memBuffer, additionalWord2);
	}
}
// Process and insert data based on directive type
void DirectiveHandler() {
    if (lineObj.directiveInfo.type == STRING_DIRECTIVE) {

        for (int i = 0; i < strlen(lineObj.directiveInfo.stringValue); i++) {
            char asciiValue[13]; // 12 bits + 1 null-terminator
            charTo12BitString(lineObj.directiveInfo.stringValue[i], asciiValue);
            dataInsertToMemory(memBuffer, asciiValue);
        }
    } else if (lineObj.directiveInfo.type == DATA_DIRECTIVE) {
        for (int i = 0; i < lineObj.directiveInfo.dataValuesCount; i++) {
            char binaryValue[13]; // 12 bits + 1 null-terminator
            intTo12BitBinary(lineObj.directiveInfo.dataValues[i], binaryValue);
            dataInsertToMemory(memBuffer, binaryValue);
        }
    }
}
void processAssemblyLine() {
	LineObj lineObj;
	lineObj.labelObj.labelName = NULL; // Initialize labelName to NULL by default

	// Remove leading whitespaces
	const char *trimmedLine = assemblyLine;
	while (*trimmedLine == ' ' || *trimmedLine == '\t') {
		trimmedLine++;
	}

	// Check if the line is empty or a comment
	if (*trimmedLine == '\0' || *trimmedLine == ';') {
		lineObj.lineType = COMMENT_LINE;

	}

	// Check if the line has a label
	char *colonPtr = strchr(trimmedLine, ':');
	if (colonPtr != NULL) {
		*colonPtr = '\0'; // Remove the colon

		// Validate the label name (excluding the colon)
		if (!firstAlphaRestAlphaNum(trimmedLine)) {
			errorObjInserter(filePack, UNVALID_LABEL_NAME, ASSEMBLY_ERROR,
					lineCount);

		}

		lineObj.labelObj.labelName = trimmedLine;
		trimmedLine = colonPtr + 1;
		while (*trimmedLine == ' ' || *trimmedLine == '\t') {
			trimmedLine++;
		}
	}

	if (isNotProcedureName(trimmedLine)) {
		// Process as a directive
		lineObj.lineType = DIRECTIVE_LINE;
		processDirective(&lineObj, trimmedLine);
	} else {
		// Process as an instruction
		lineObj.lineType = INSTRUCTION_LINE;
		processInstruction(&lineObj, trimmedLine);
	}

}

// SIDE NOTE: ENTRY POINT
// Function to assemble the code
void assembler(FilePacket *preAssembeldFilePack) {

	filePack = preAssembeldFilePack; // asign pack to static variable
	initLineObj(); // initalize and allocate mem for Line Obj
	filePack->AssFile = createOutputFileWithExtension(filePack, ".ob", "w");
	int wordCount = 0; // a variable holding the number of machine codes word needed so far
	int dataCount = 0; // a variable holding the number of data defined
	int externalLabelCount = 0; // a variable holding the number of external labels defined

	// First pass is to populate the labels data and instruction tree and to check for errors
	while (fgets(assemblyLine, sizeof(assemblyLine), filePack->PreAssFile)
			!= NULL) {
		// Clean LineObj variable
		resetLineObj();
		// Remove newline character if present
		size_t len = strlen(assemblyLine);
		if (len > 0 && assemblyLine[len - 1] == '\n') {
			assemblyLine[len - 1] = '\0';
			len--;
		}

		// extract the next line obj
		processAssemblyLine();

		// Check if line is a comment line
		if (lineObj.lineType == COMMENT_LINE) {

		}
		// Check if its declartive extern line and if so Store label in instruction tree
		else if (lineObj.lineType == DIRECTIVE_LINE
				&& lineObj.directiveInfo.type == EXTERN_DIRECTIVE) {
			lineObj.labelObj.labelVal = ++externalLabelCount;
			externLabelsTree = IinsertNode(externLabelsTree,
					lineObj.labelObj.labelName, lineObj.labelObj);

		}

		// Check if its declartive entry line
		else if (lineObj.lineType == DIRECTIVE_LINE
				&& lineObj.directiveInfo.type == ENTRY_DIRECTIVE) {
			entryLabelsTree = IinsertNode(entryLabelsTree,
					lineObj.labelObj.labelName, lineObj.labelObj);

		} // .string or .data directive handling
		else if (lineObj.lineType == DIRECTIVE_LINE) {
			dataCount = countElementsInDirective() + dataCount;
			lineObj.labelObj.labelVal = dataCount;
			dataLabelTree = IinsertNode(dataLabelTree,
					lineObj.labelObj.labelName, lineObj.labelObj);

		} else if (lineObj.lineType == INSTRUCTION_LINE) {
			// means there is a lable
			if (!(lineObj.labelObj.labelName[0] == '\0')) {
				lineObj.labelObj.labelVal = wordCount;
				instructionLabelTree = IinsertNode(instructionLabelTree,
						lineObj.labelObj.labelName, lineObj.labelObj);
			}
			wordCount = howManyMachineWord() + wordCount;
		}

	}

	/// PREPARTAION FOR SECOND SWEEP
	initMemoryBuffer(memBuffer,wordCount); // initalize memoryBuffer

	// add beggining machine code address to all instruction address currently stored
	IaddValueToAllNodes(instructionLabelTree, MACHINE_CODE_START_ADDRESS);
	IaddValueToAllNodes(dataLabelTree, MACHINE_CODE_START_ADDRESS + wordCount);

	// Merge the different label trees into the mainLabelTree
	mainLabelTree = mergeTrees(mainLabelTree, instructionLabelTree);
	mainLabelTree = mergeTrees(mainLabelTree, entryLabelsTree);
	mainLabelTree = mergeTrees(mainLabelTree, externLabelsTree);
	mainLabelTree = mergeTrees(mainLabelTree, dataLabelTree);

	// After the first loop has finished
	// Reset the file cursor position to the beginning of the file
	rewind(filePack->PreAssFile);

	while (fgets(assemblyLine, sizeof(assemblyLine), filePack->PreAssFile)
			!= NULL) {
		// Clean LineObj variable
		resetLineObj();

		// extract the next line obj
		processAssemblyLine();
		if (lineObj.lineType == INSTRUCTION_LINE) {
			MachineWordsForInstructionHandler();
		}
		if (lineObj.lineType == DIRECTIVE_LINE) {
			DirectiveHandler();
	    }
	}

	writeBase64ToObjectFile(memBuffer, filePack, wordCount, dataCount);
	// Clean up LineObj and its descendants
	cleanupLineObj();
}

