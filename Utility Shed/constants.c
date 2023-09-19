#include <stdlib.h>
#include "constants.h"


// Define the procedure information array
const ProcedureInfo PROCEDURE_INFO[MAX_NUMBER_OF_PROCEDURES] = {
    /*  0 */ {"mov", "0000", "Move", 2, (AddressingMode[]){DIRECT_ADDRESSING, IMMEDIATE_ADDRESSING}, 2, (AddressingMode[]){DIRECT_ADDRESSING, REGISTER_ADDRESSING}, 2},
    /*  1 */ {"cmp", "0001", "Compare", 2, (AddressingMode[]){DIRECT_ADDRESSING, IMMEDIATE_ADDRESSING}, 2, (AddressingMode[]){DIRECT_ADDRESSING, REGISTER_ADDRESSING}, 2},
    /*  2 */ {"add", "0010", "Add", 2, (AddressingMode[]){DIRECT_ADDRESSING, IMMEDIATE_ADDRESSING, REGISTER_ADDRESSING}, 3, (AddressingMode[]){DIRECT_ADDRESSING, REGISTER_ADDRESSING}, 2},
    /*  3 */ {"sub", "0011", "Subtract", 2, (AddressingMode[]){DIRECT_ADDRESSING, IMMEDIATE_ADDRESSING, REGISTER_ADDRESSING}, 3, (AddressingMode[]){DIRECT_ADDRESSING, REGISTER_ADDRESSING}, 2},
    /*  4 */ {"not", "0100", "NOT", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /*  5 */ {"clr", "0101", "Clear", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /*  6 */ {"lea", "0110", "Load Effective Address", 2, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /*  7 */ {"inc", "0111", "Increment", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /*  8 */ {"dec", "1000", "Decrement", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /*  9 */ {"jmp", "1001", "Jump", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /* 10 */ {"bne", "1010", "Branch if Not Equal", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /* 11 */ {"red", "1011", "Read", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /* 12 */ {"prn", "1100", "Print", 1, (AddressingMode[]){DIRECT_ADDRESSING, IMMEDIATE_ADDRESSING}, 2, ( AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /* 13 */ {"jsr", "1101", "Jump to Subroutine", 1, (AddressingMode[]){DIRECT_ADDRESSING}, 1, (AddressingMode[]){REGISTER_ADDRESSING}, 1},
    /* 14 */ {"rts", "1110", "Return from Subroutine", 0, NULL, 0, NULL, 0},
    /* 15 */ {"stop", "1111", "Stop", 0, NULL, 0, NULL, 0}
};
