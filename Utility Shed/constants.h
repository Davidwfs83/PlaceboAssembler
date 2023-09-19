/*
 * constants.h
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_



#define MAX_FILENAME_LENGTH 100
#define MAX_INPUTFILE_LINE_LENGTH MAX_WORDS_PER_LINE * MAX_WORD_LENGTH
#define MAX_NUMBER_OF_FILES 10
#define MAX_WORDS_PER_LINE  10
#define MAX_WORD_LENGTH  20
#define MAX_MACRO_NAME_LENGTH 10
#define MAX_MACRO_CONTENT_LENGTH 100
#define MAX_NUMBER_OF_PROCEDURES 16
#define MAX_DIRECTIVE_NAME_LENGTH 10
#define MAX_STRING_DIRECTIVE_VALUE 10
#define MEMORY_SIZE 1024
#define BIT_GROUP_SIZE 12
#define MAX_LABEL_NAME_LENGTH 20
#define MACHINE_CODE_START_ADDRESS 100




typedef enum {
	IMMEDIATE_ADDRESSING, // 321321 , 3213
	DIRECT_ADDRESSING, // LENGTH, MAIN
	REGISTER_ADDRESSING // @r3, @r1
} AddressingMode;

typedef enum {
	COMMENT_LINE,
	INSTRUCTION_LINE,
	DIRECTIVE_LINE
} LineType;

// Define the structure for procedure information
typedef struct {
    char *name;                // Procedure name
    const char *machineCode;         // Machine code (4-bit string)
    const char *comment;             // Comment for the procedure
    const int numberOfAllowedArgs;   // Number of allowed arguments
     AddressingMode *srcModes;   // Allowed addressing modes for source operand
    int numSrcModes;                 // Number of allowed source modes
    AddressingMode *destModes;  // Allowed addressing modes for destination operand
    int numDestModes;                // Number of allowed destination modes
} ProcedureInfo;


// Enum to represent different directive types
typedef enum  {
	UNKNOWN_DIRECTIVE,
    STRING_DIRECTIVE,
    DATA_DIRECTIVE,
    ENTRY_DIRECTIVE,
    EXTERN_DIRECTIVE
} DirectiveType;

// Define the structure for directive information
typedef struct {
    char *name;          // Directive name
    DirectiveType type;   // Directive type
    char *stringValue;   // String value (for STRING_DIRECTIVE)
    int *dataValues;           // Data values (for DATA_DIRECTIVE)
    int dataValuesCount;       // Number of data values
} DirectiveInfo;

// Only for Instruction Lines
typedef struct {
	AddressingMode  addressMode; // the address mode for the said arguement
	char * val;
} LineArgument;


typedef struct {
    int procedureDecimalIndex;
    LineArgument firstArg;
    LineArgument secondArg;
} ProcedureObj;

typedef enum {
	REGULAR_LABEL,
	EXTERNAL_LABEL,
	RELOCATABLE_LABEL
} LabelType;

typedef struct {
	char* labelName;
	LabelType labelType;
	int labelVal;
} LabelObj;


typedef struct {
    LineType lineType;
    ProcedureObj procedureInfo;
    DirectiveInfo directiveInfo;
    LabelObj labelObj;
} LineObj;


extern const ProcedureInfo PROCEDURE_INFO[];

#endif /* CONSTANTS_H_ */
