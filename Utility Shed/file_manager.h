/*
 * file_manager.h
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_


typedef enum {
	PREASSEMBLY_ERROR,
	ASSEMBLY_ERROR,
	GENERAL_ERROR

} ErrType;

typedef struct ErrorObj {
	ErrType errType;
    int lineNumber;
    char *errorMessage;
} ErrorObj;

typedef enum {
    MEM_ALLOCATION_ERR, // GENERAL
    FILE_CREATION_ERR,  // GENERAL
    REDIFINITION_OF_EXISTING_MACRO_ERR, // PREASSEMBLER
	MISSING_END_MACRO_STATEMENT_ERR, // PREASSEMBLER
	INVALID_MACRO_NAME_ERR, // PREASSEMBLER,
	FILE_OPENING_ERR, //GENERAL
	UNVALID_DIRECTIVE_NAME, // MACHINE CODE FACTORY
	UNVALID_ADDRESING_MODE_SOUCE_OPERAND, // MACHINE CODE FACTORY
	UNVALID_ADDRESING_MODE_DESTINATION_OPERAND, // MACHINE CODE FACTORY
	LABEL_REDIFINTION_ERR, // MACHINE CODE FACTORY
	DOUBLE_COMMA_ERR ,// MACHINE CODE FACTORY
	UNVALID_LABEL_NAME, // MACHINE CODE FACOTRY
	UNVALID_PROCEDURE_NAME, // MACHINE CODE FACOTRY
	GENERAL_SYNTAX_ERR, // MACHINE CODE FACOTRY
	MISSING_PROCEDURE_ARGUMENT // MACHINE CODE FACOTRY
} ErrorCode;







typedef struct FilePacket {
    FILE *InputFile; // Input file with extension .as
    FILE *PreAssFile; // Preassembled file with extension .am
    FILE *AssFile;  // Assembled file with extension .ob
    FILE *entryFile; // File representing all entry declaration .ent
    FILE *externalFile; // File representing all external declaration .ext
    ErrorObj *Errors; // Array of ErrorObj pointers
    char * filename;
    size_t ErrCount;
    size_t ErrorsCapacity; // Current capacity of the Errors array
} FilePacket;

void verifyAndOpenFiles(int numFiles, char *filenames[], struct FilePacket *fileInputs);
void errorObjInserter(FilePacket *file_manager, ErrorCode error_code, ErrType err_type, int line_number);
void writeBase64ToObjectFile(const char *buffer, FilePacket *filePack, int instructionCount, int dataCount);
#endif /* FILE_MANAGER_H_ */
