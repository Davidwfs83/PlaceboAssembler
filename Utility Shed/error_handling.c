/*
 * error_handling.c
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */




#include <stdio.h>
#include <stdlib.h>

#include "file_manager.h"

const char* get_error_message(ErrorCode error_code);

#define ERROR_MESSAGE(code) get_error_message(code);

const char* error_messages[] = {
    "Memory Allocation Failed!" // 0 - General Error
	"File Creation Failed!" // 1 - General Error
	"Cannot Redefine a Macro already defined" // 2 - PreAssembler Error
	"Missing End Macro Statement!" // 3 - PreAssembler Error
    "The macro name you provided is invalid!" // 4 - PreAssembler Error
	"Unable to open specified file!" // 5 - General Error
	"Unvalid directive name provided, only .string .data .entry .extern allowed" // 6 -MACHINE CODE FACTORY
	"Unvalid addresing at source operand!" // 7 -MACHINE CODE FACTORY
	"unvalid addresing at destination operand!" // 8 - MACHINE CODE FACTORY
	"Redifinition of defined Label!" // 9 - MACHINE CODE FACTORY
	"Double comma not allowed!" // 10 - MACHINE CODE FACTORY
    "label name should be alpha for first char and alphanum for rest" // 11 - MACHINE CODE FACOTRY
	"Unvalid Procedure Name" // 11 - MACHINE CODE FACOTRY
	"General Syntax Error" // 12 - MACHINE CODE FACOTRY
	"Missing procedure arguemnt" // 13 -  MACHINE CODE FACOTRY
};

ErrorObj errObjFactory(ErrorCode error_code, ErrType err_type, int line_number) {
    ErrorObj new_error;
    new_error.errType = err_type;
    new_error.lineNumber = line_number;
    new_error.errorMessage = get_error_message(error_code);
    return new_error;
}

const char* get_error_message(ErrorCode error_code) {
    if (error_code >= 0 && error_code < sizeof(error_messages) / sizeof(error_messages[0])) {
        return error_messages[error_code];
    }
    return "Unknown error";
}


void errorObjInserter(FilePacket *file_packet, ErrorCode error_code, ErrType err_type, int line_number) {
    if (file_packet->ErrCount >= file_packet->ErrorsCapacity) {
        // Double the capacity to reduce reallocation frequency
        size_t new_capacity = file_packet->ErrorsCapacity * 2;
        ErrorObj *new_errors = realloc(file_packet->Errors, new_capacity * sizeof(ErrorObj));

        if (new_errors == NULL) {
            perror("Memory allocation error");
            return;
        }

        file_packet->Errors = new_errors;
        file_packet->ErrorsCapacity = new_capacity;
    }

    struct ErrorObj new_error = errObjFactory(error_code, err_type, line_number);

    file_packet->Errors[file_packet->ErrCount++] = new_error;
}
