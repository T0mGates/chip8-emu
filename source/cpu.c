#include <cpu.h>

Error
    ingestOpCode(IN uint16_t OpCode, INOUT chip8* Chip8Obj)
{
    IN_FUNC;
    Error retVal        = FAIL;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Look at only the first hex digit
        DEBUG( "Masked OpCode: 0x%04X\n", OpCode & 0xF000 );
        switch( OpCode & 0xF000 )
                {
                    case 0x0000:
                        // 00E0 - CLS... Saw some code that 0000 was clear as well idk...
                        if( 0x00E0 == OpCode )//|| 0x0000 == OpCode )
                        {
                            // Clear the display
                            DEBUG( "00E0 - CLS\n" );

                            for( unsigned int i = 0; i < 64 * 32; i++ )
                            {
                                Chip8Obj->graphics[i]   = 0;
                            }

                            Chip8Obj->updateScreen      = true;

                            retVal                      = SUCCESS;
                        }// 0x00E0

                        // 00EE - RET
                        else if( 0x00EE == OpCode )
                        {
                            // Return from a subroutine
                            DEBUG( "00EE - RET\nNew stack pointer: %d, new PC: %d\n",  Chip8Obj->stackPointer-1, Chip8Obj->stack[Chip8Obj->stackPointer-1] );
                            Chip8Obj->stackPointer      -= 1;
                            Chip8Obj->programCounter    = Chip8Obj->stack[Chip8Obj->stackPointer];
                            retVal                      = SUCCESS;
                        }// 0x00EE
                        else
                        {
                            fprintf( stderr, "UNKNOWN OpCode: 0x%04X, PC = %d\n", OpCode, Chip8Obj->programCounter );
                        }// 0x00EE == OpCode
                        break;

                    case 0x1000:
                        // 1nnn - JP addr
                        // Jump to location nnn
                        DEBUG( "1nnn - JP addr\n" );
                        Chip8Obj->programCounter        = ( OpCode & 0x0FFF );
                        retVal                          = PC_NO_INCREMENT;
                        break;

                    case 0x2000:
                        // 2nnn - CALL addr
                        // Call subroutine at nnn
                        DEBUG( "2nnn - CALL addr\nStack at sp value: %d is %d\n", Chip8Obj->stackPointer, Chip8Obj->programCounter );
                        Chip8Obj->stack[Chip8Obj->stackPointer] = Chip8Obj->programCounter;
                        Chip8Obj->stackPointer++;
                        Chip8Obj->programCounter                = ( OpCode & 0x0FFF );
                        retVal                                  = PC_NO_INCREMENT;
                        break;

                    case 0x3000:
                        // 3xkk - SE Vx, byte
                        // Skip next instruction if Vx = kk
                        DEBUG( "3xkk - SE Vx, byte\n" );
                        if( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] == ( OpCode & 0x00FF ) )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x4000:
                        // 4xkk - SNE Vx, byte
                        // Skip next instruction if Vx != kk
                        DEBUG( "4xkk - SNE Vx, byte\n" );
                        if( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] != ( OpCode & 0x00FF ) )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x5000:
                        // 5xy0 - SE Vx, Vy
                        // Skip next instruction if Vx = Vy
                        DEBUG( "5xy0 - SE Vx, Vy\n" );
                        if( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] == Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4] )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x6000:
                        // 6xkk - LD Vx, byte
                        // Set Vx = kk
                        DEBUG( "6xkk - LD Vx, byte\n" );
                        Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   =  ( OpCode & 0x00FF );

                        retVal                                          = SUCCESS;
                        break;

                    case 0x7000:
                        // 7xkk - ADD Vx, byte
                        // Set Vx = Vx + kk
                        DEBUG( "7xkk - ADD Vx, byte\n" );
                        Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   += ( OpCode & 0x00FF );

                        retVal                                      = SUCCESS;
                        break;

                    case 0x8000:
                        if( 0x0000 == ( OpCode & 0x000F ) )
                        {
                            // 8xy0 - LD Vx, Vy
                            // Set Vx = Vy
                            DEBUG( "8xy0 - LD Vx, Vy\n" );
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   =  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0001 == ( OpCode & 0x000F ) )
                        {
                            // 8xy1 - OR Vx, Vy
                            // Set Vx = Vx OR Vy
                            DEBUG( "8xy1 - OR Vx, Vy\n" );
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   |=  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0002 == ( OpCode & 0x000F ) )
                        {
                            // 8xy2 - AND Vx, Vy
                            // Set Vx = Vx AND Vy
                            DEBUG( "8xy2 - AND Vx, Vy\n" );
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   &=  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0003 == ( OpCode & 0x000F ) )
                        {
                            // 8xy3 - XOR Vx, Vy
                            // Set Vx = Vx XOR Vy
                            DEBUG( "8xy3 - XOR Vx, Vy\n" );
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   ^=  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0004 == ( OpCode & 0x000F ) )
                        {
                            // 8xy4 - ADD Vx, Vy
                            // Set Vx = Vx + Vy, set VF = carry
                            DEBUG( "8xy4 - ADD Vx, Vy\n" );

                            uint16_t sum                                = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] + Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];
                            // If result is higher than 8 bits, set flag
                            if( sum > 0xFF )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// sum > 0xFF

                            // Store lowest 8 bits from sum into Vx
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   = ( sum & 0x00FF );

                            retVal                                          = SUCCESS;
                        }
                        else if( 0x0005 == ( OpCode & 0x000F ) )
                        {
                            // 8xy5 - SUB Vx, Vy
                            DEBUG( "8xy5 - SUB Vx, Vy\n" );

                            // If Vx > Vy, set VF to 1, else 0
                            if( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] > Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4] )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// Vx > Vy

                            // Set Vx to Vx - Vy
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   -=  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0006 == ( OpCode & 0x000F ) )
                        {
                            // 8xy6 - SHR Vx {, Vy}
                            DEBUG( "8xy6 - SHR Vx {, Vy}\n" );

                            // If LSB of Vx == 1, set VF to 1, else 0
                            if( ( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] ) & 0x0001 )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// LSB of Vx == 1

                            // Divide Vx by 2
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   /=  2;
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0007 == ( OpCode & 0x000F ) )
                        {
                            // 8xy7 - SUBN Vx, Vy
                            DEBUG( "8xy7 - SUBN Vx, Vy\n" );

                            // If Vy > Vx, set VF to 1, else 0
                            if( Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4] > Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// Vy > Vx

                            // Vx = Vy - Vx

                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   =  Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4] - Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x000E == ( OpCode & 0x000F ) )
                        {
                            // 8xyE - SHL Vx {, Vy}
                            DEBUG( "8xyE - SHL Vx {, Vy}\n" );

                            // If MSB of Vx == 1, set VF to 1, else 0
                            if( ( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] ) & 0x0080 )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// LSB of Vx == 1

                            // Multiply Vx by 2
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   *=  2;
                            retVal                                      = SUCCESS;

                        }
                        else
                        {
                            fprintf( stderr, "UNKNOWN OpCode: 0x%04X, PC = %d\n", OpCode, Chip8Obj->programCounter );
                        }// OpCode & 0x000F
                        break;

                    case 0x9000:
                        // 9xy0 - SNE Vx, Vy
                        DEBUG( "9xy0 - SNE Vx, Vy\n" );

                        // Skip next instruction if Vx != Vy
                        if( Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] != Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4] )
                        {
                            Chip8Obj->programCounter                += 2;
                        }// Vx != Vy

                        retVal                                      = SUCCESS;
                        break;

                    case 0xA000:
                        // Annn - LD I, addr
                        DEBUG( "Annn - LD I, addr\n" );

                        // Set I = nnn
                        Chip8Obj->indexRegister                     = ( OpCode & 0x0FFF );

                        retVal                                      = SUCCESS;
                        break;

                    case 0xB000:
                        // Bnnn - JP V0, addr
                        DEBUG( "Bnnn - JP V0, addr\n" );

                        // Jump to location nnn + V0
                        Chip8Obj->programCounter                    = ( OpCode & 0x0FFF ) + Chip8Obj->registers[0];

                        retVal                                      = PC_NO_INCREMENT;
                        break;

                    case 0xC000:
                        // Cxkk - RND Vx, byte
                        DEBUG( "Cxkk - RND Vx, byte\n" );

                        // Set Vx = random byte AND kk

                        // Random number must be between 0 and 255
                        uint8_t randNum                                 = rand() % 256;
                        Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   = ( ( OpCode & 0x00FF ) & randNum );
                        retVal                                          = SUCCESS;
                        break;

                    case 0xD000:
                        // Dxyn - DRW Vx, Vy, nibble
                        DEBUG( "Dxyn - DRW Vx, Vy, nibble\n" );

                        // Draws a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a height of N pixels
                        // Each row of 8 pixels is read as bit-coded starting from the indexRegister (indexRegister doesn't change after this instruction)
                        // Vf is set to 1 if any pixels are flipped from set to unset when the sprite is drawn (collision), and 0 if it doesn't happen
                        BYTE xCoordinate                            = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];
                        BYTE yCoordinate                            = Chip8Obj->registers[( OpCode & 0x00F0 ) >> 4];
                        BYTE width                                  = 8;
                        BYTE height                                 = ( OpCode & 0x000F );

                        BYTE curPixel                               = 0;
                        BYTE ramPixel                               = 0;

                        Chip8Obj->registers[0xF]                    = 0;

                        for( unsigned int y = 0; y < height; y++ )
                        {
                            ramPixel                                = Chip8Obj->ram[Chip8Obj->indexRegister + y];

                            for( unsigned int x = 0; x < width; x++ )
                            {
                                // Check if this pixel is set to 1 (pixel is a bit), going left to right
                                if( 0 != ( ramPixel & ( 0x80 >> x ) ) )
                                {
                                    // Means this bit/pixel is 'set' (this is the pixel 'to be drawn')

                                    // Now look at our current pixel
                                    if( 1 == Chip8Obj->graphics[( xCoordinate + x + ( ( yCoordinate + y ) * 64 ) )] )
                                    {
                                        Chip8Obj->registers[0xF]    = 1;
                                    }// if curPixel is set

                                    // State of each pixel is set by using XOR. So, XOR the current pixel state with current value in memory, if ==, set bit to 0, else 1
                                    // Basically, if pixel is not the same, set to 1, else 0
                                    // Don't forget to take int account for wrap around
                                    Chip8Obj->graphics[( xCoordinate + x + ( ( yCoordinate + y ) * 64 ) ) % (64 * 32)] ^= 1;
                                }// ramPixel is set
                            }// for x
                        }// for y

                        // We changed our graphics array so we will need to update the screen
                        Chip8Obj->updateScreen                      = true;

                        retVal                                      = SUCCESS;
                        break;

                    case 0xE000:
                        if( 0x009E == ( OpCode & 0x00FF ) )
                        {
                            // Ex9E - SKP Vx
                            DEBUG( "Ex9E - SKP Vx\n" );

                            // Skip next instruction if key with the value of Vx is pressed
                            if( Chip8Obj->keys[Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]] )
                            {
                                Chip8Obj->programCounter            += 2;
                            }// key Vx is pressed

                            retVal                                  = SUCCESS;
                        }
                        else if( 0x00A1 == ( OpCode & 0x00FF ) )
                        {
                            // ExA1 - SKNP Vx
                            DEBUG( "ExA1 - SKNP Vx\n" );

                            // Skip next instruction if key with the value of Vx is not pressed
                            if( !Chip8Obj->keys[Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]] )
                            {
                                Chip8Obj->programCounter            += 2;
                            }// key Vx is not pressed

                            retVal                                  = SUCCESS;
                        }
                        else
                        {
                            fprintf( stderr, "UNKNOWN OpCode: 0x%04X, PC = %d\n", OpCode, Chip8Obj->programCounter );
                        }// OpCode & 0x00FF

                        break;

                    case 0xF000:
                        if( 0x0007 == ( OpCode & 0x00FF ) )
                        {
                            // Fx07 - LD Vx, DT
                            DEBUG( "Fx07 - LD Vx, DT\n" );

                            // Set Vx = delay timer value
                            Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   = Chip8Obj->delayTimer;

                            retVal                                          = SUCCESS;
                        }
                        else if( 0x000A == ( OpCode & 0x00FF ) )
                        {
                            // Fx0A - LD Vx, K
                            DEBUG( "Fx0A - LD Vx, K\n" );
                            bool keyWasPressed  = false;

                            // Wait for a key press, store the value of the key in Vx
                            for( unsigned int i = 0; i < 16; i++ )
                            {
                                if( Chip8Obj->keys[i] )
                                {
                                    Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8]   = i;
                                    keyWasPressed   = true;
                                }// if keys[i]
                            }// for

                            // If a key was not pressed, try again next cycle
                            if( keyWasPressed )
                            {
                                retVal      = SUCCESS;
                            }
                            else
                            {
                                retVal      = PC_NO_INCREMENT;
                            }// keyWasPressed
                        }
                        else if( 0x0015 == ( OpCode & 0x00FF ) )
                        {
                            // Fx15 - LD DT, Vx
                            DEBUG( "Fx15 - LD DT, Vx\n" );

                            // Set delay timer = Vx
                            Chip8Obj->delayTimer                        = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0018 == ( OpCode & 0x00FF ) )
                        {
                            // Fx18 - LD ST, Vx
                            DEBUG( "Fx18 - LD ST, Vx\n" );

                            // Set sound timer = Vx
                            Chip8Obj->soundTimer                        = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];
                            // Start beeping
                            playBeep( Chip8Obj->soundTimer );
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x001E == ( OpCode & 0x00FF ) )
                        {
                            // Fx1E - ADD I, Vx
                            DEBUG( "Fx1E - ADD I, Vx\n" );

                            // Set I = I + Vx
                            Chip8Obj->indexRegister                     += Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0029 == ( OpCode & 0x00FF ) )
                        {
                            // Fx29 - LD F, Vx
                            DEBUG( "Fx29 - LD F, Vx\n" );

                            // Set I = location of sprite for digit Vx
                            // Characters 0-F (in hexadecimal) are represented by a 4x5 font
                            // Fonts are stored at 0x0000 and are 5 bytes each, so multiply by 5 to skip through
                            Chip8Obj->indexRegister                     = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8] * 5;

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0033 == ( OpCode & 0x00FF ) )
                        {
                            // Fx33 - LD B, Vx
                            DEBUG( "Fx33 - LD B, Vx\n" );

                            // Store BCD representation of Vx in memory locations I, I+1, and I+2
                            uint8_t vx                                  = Chip8Obj->registers[( OpCode & 0x0F00 ) >> 8];

                            // Place hundreds digit in memory at location I
                            // Tens digit at location I+1
                            // Ones digit at location I+2
                            Chip8Obj->ram[Chip8Obj->indexRegister]      = ( vx / 100 ) % 10;
                            Chip8Obj->ram[Chip8Obj->indexRegister + 1]  = ( vx / 10 ) % 10;
                            Chip8Obj->ram[Chip8Obj->indexRegister + 2]  = vx % 10;

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0055 == ( OpCode & 0x00FF ) )
                        {
                            // Fx55 - LD [I], Vx
                            DEBUG( "Fx55 - LD [I], Vx\n" );

                            // Store registers V0 through Vx in memory starting at location I
                            for( unsigned int i = 0; i <= ( ( OpCode & 0x0F00 ) >> 8 ); i++ )
                            {
                                Chip8Obj->ram[Chip8Obj->indexRegister + i]  = Chip8Obj->registers[i];
                            }// for

                            retVal                                          = SUCCESS;
                        }
                        else if( 0x0065 == ( OpCode & 0x00FF ) )
                        {
                            // Fx65 - LD Vx, [I]
                            DEBUG( "Fx65 - LD Vx, [I]\n" );

                            // Read registers V0 through Vx from memory starting at location I
                            for( unsigned int i = 0; i <= ( ( OpCode & 0x0F00 ) >> 8 ); i++ )
                            {
                                Chip8Obj->registers[i]                  = Chip8Obj->ram[Chip8Obj->indexRegister + i];
                            }// for

                            retVal                                      = SUCCESS;
                        }
                        else
                        {
                            fprintf( stderr, "UNKNOWN OpCode: 0x%04X, PC = %d\n", OpCode, Chip8Obj->programCounter );
                        }// OpCode & 0x00FF

                        break;

                    default:
                    fprintf( stderr, "UNKNOWN OpCode: 0x%04X, PC = %d\n", OpCode, Chip8Obj->programCounter );
                        break;

                }// switch OpCode
    }// NULL args

    OUT_FUNC;
    return retVal;
}