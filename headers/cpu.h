#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <chip8.h>

// =================================
// FUNCTIONS
// =================================

/*
 * @brief           Ingests a given opcode, decoding it and doing whatever the instruction is set to do
 *
 * @param[in]       OpCode              - OpCode to decode and execute an instruction based off its' value
 * @param[inout]    Chip8Obj            - the chip8 struct to manipulate based off the given opcode
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    ingestOpCode(IN uint16_t OpCode, INOUT chip8* Chip8Obj);


#endif