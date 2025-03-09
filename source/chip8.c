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
        Chip8Obj->indexRegister     = 0;

        // PC should start at 0x200
        Chip8Obj->programCounter    = 0x200;
        Chip8Obj->delayTimer        = 0;
        Chip8Obj->soundTimer        = 0;
        Chip8Obj->stackPointer      = 0;
        // To draw the background at least
        Chip8Obj->updateScreen      = true;

        for( unsigned int i = 0; i < 64 * 32; i++ )
        {
            Chip8Obj->graphics[i]   = 0;
        }// for graphics

        for( unsigned int i = 0; i < 16; i++ )
        {
            Chip8Obj->stack[i]      = 0;
        }// for stack

        for( unsigned int i = 0; i < 16; i++ )
        {
            Chip8Obj->keys[i]       = false;
        }// for keys

        for( unsigned int i = 0; i < 4096; i++ )
        {
            Chip8Obj->ram[i]   = 0;
        }// for ram

        // Load the fontset starting at memory location 0x50
        BYTE fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

        /*

            DEC   HEX    BIN         RESULT    DEC   HEX    BIN         RESULT
            240   0xF0   1111 0000    ****     240   0xF0   1111 0000    ****
            144   0x90   1001 0000    *  *      16   0x10   0001 0000       *
            144   0x90   1001 0000    *  *      32   0x20   0010 0000      *
            144   0x90   1001 0000    *  *      64   0x40   0100 0000     *
            240   0xF0   1111 0000    ****      64   0x40   0100 0000     *

        */

        for( uint16_t i = 0; i < 80; i++ )
        {
            Chip8Obj->ram[i] = fontset[i];
        }// for

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
        if( PC_NO_INCREMENT != ( retVal = ( ingestOpCode( opCode, Chip8Obj ) ) ) )
        {
            Chip8Obj->programCounter += 2;
        }
        else
        {
            // Set retVal to success if it was PC_NO_INCREMENT
            retVal      = SUCCESS;
        }// PC_NO_INCREMENT != retVal

        // Update timers - but we do it outside this function since we want a different HZ value than the instructions

        // retVal already set

    }// NULL args

    OUT_FUNC;
    return retVal;
}

Error
    loadGame(OUT chip8* Chip8Obj, IN char* GameFilePath)
{
    IN_FUNC;
    Error retVal    = FAIL;

    FILE* fptr      = NULL;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal      = NULL_ARG;
    }
    else if( NULL == GameFilePath )
    {
        DEBUG( "GameFilePath is NULL!\n" );
        retVal      = NULL_ARG;
    }
    else
    {
        if ( NULL == ( fptr = fopen( GameFilePath, "rb" ) ) )
        {
            retVal  = GAME_FILE_NOT_FOUND;
        }
        else
        {
            // Game file now in fptr
            unsigned int    fileSize        = 0;

            // Calculate fileSize
            fseek( fptr, 0, SEEK_END );
            fileSize                        = ftell( fptr );
            fseek( fptr, 0, SEEK_SET );

            printf( "Game's fileSize: %d bytes\nPC should not go out of these bounds: 0 to 79 (fonts) and %d to %d (game)\n", fileSize, 0x200, 0x200 + fileSize - 1 );

            BYTE            buffer[fileSize];

            // Read in the data to our buffer
            fread( buffer, sizeof(BYTE), fileSize, fptr );
            for( unsigned int i = 0; i < fileSize; i++ )
            {
                // Remember chip8's program starts at memory address 0x200
                Chip8Obj->ram[i + 0x200] = buffer[i];
            }// for

            fclose( fptr );
            retVal  = SUCCESS;
        }// fopen

    }// NULL args

    OUT_FUNC;
    return retVal;
}

Error
    playBeep(IN uint8_t NumCycles)
{
    IN_FUNC;
    Error retVal    = FAIL;

    if( 0 == NumCycles )
    {
        DEBUG( "NumCycles is 0!\n" );
        retVal      = NULL_ARG;
    }
    else
    {
        // Beep at 750Hz, and then calculate the milliseconds to beep for
        double msBeeping = ( ( (float)NumCycles / (float)TIMERS_HZ ) * 1000 );
        Beep(750, msBeeping );
        retVal      = SUCCESS;
    }// NULL args

    OUT_FUNC;
    return retVal;
}