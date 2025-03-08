#include <chip8.h>

Error
    initChip8(OUT chip8* Chip8Obj)
{
    IN_FUNC;
    Error retVal                = FAIL;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        Chip8Obj->registerIndex     = 0;
        Chip8Obj->programCounter    = 0;
        Chip8Obj->delayTimer        = 0;
        Chip8Obj->soundTimer        = 0;
        Chip8Obj->stackPointer      = 0;

        retVal                      = SUCCESS;
    }// NULL args

    OUT_FUNC;
    return retVal;
}

Error
    emulateCycle(INOUT chip8* Chip8Obj)
{
    IN_FUNC;
    Error retVal        = FAIL;

    // Will hold the read opcode for this cycle
    uint16_t opCode     = 0;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Fetch Opcode
        // This will be located in RAM with the memory offset being the programCounter's value
        // The opcode is 2 bytes long, so read one byte into a 16bit number, shift it left by 8 and insert the next byte in the 8 LSB positions
        opCode          = ( Chip8Obj->ram[Chip8Obj->programCounter] << 8 ) | ( Chip8Obj->ram[Chip8Obj->programCounter + 1] );
        DEBUG( "Fetched opcode: 0x%04X\n", opCode );

        // Decode/Execute Opcode
        // TODO: Return code could tell us a lot
        if( PC_NO_INCREMENT != ( retVal = ( ingestOpCode( opCode, Chip8Obj ) ) ) )
        {
            Chip8Obj->programCounter += 2;
        }// PC_NO_INCREMENT != retVal

        // Update timers
    }// NULL args

    OUT_FUNC;
    return retVal;
}