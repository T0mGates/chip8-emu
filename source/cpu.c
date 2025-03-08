#include <cpu.h>

Error
    ingestOpCode(IN uint16_t OpCode, INOUT chip8* Chip8Obj)
{
    IN_FUNC;
    Error retVal        = FAIL;

    if( NULL == OpCode )
    {
        DEBUG( "OpCode is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Look at only the first hex digit
        DEBUG( "Masked OpCode: 0x%08X\n", OpCode & 0xF000 );
        switch( OpCode & 0xF000 )
                {
                    case 0x0000:
                        // 00E0 - CLS
                        if( 0x00E0 == OpCode )
                        {
                            DEBUG( "00E0 - CLS\n" );
                            retVal                      = SUCCESS;
                        }// 0x00E0

                        // 00EE - RET
                        else if( 0x00EE == OpCode )
                        {
                            DEBUG( "00EE - RET\n" );
                            Chip8Obj->programCounter    = Chip8Obj->stack[Chip8Obj->stackPointer];
                            Chip8Obj->stackPointer      -= 1;
                            retVal                      = SUCCESS;
                        }// 0x00EE
                        break;

                    case 0x1000:
                        // 1nnn - JP addr
                        DEBUG( "1nnn - JP addr\n" );
                        Chip8Obj->programCounter        = OpCode & 0x0FFF;
                        retVal                          = PC_NO_INCREMENT;
                        break;

                    case 0x2000:
                        // 2nnn - CALL addr
                        DEBUG( "2nnn - CALL addr\n" );
                        Chip8Obj->stackPointer++;
                        Chip8Obj->stack[Chip8Obj->stackPointer] = Chip8Obj->programCounter;
                        Chip8Obj->programCounter                = OpCode & 0x0FFF;
                        retVal                                  = PC_NO_INCREMENT;
                        break;

                    case 0x3000:
                        // 3xkk - SE Vx, byte
                        DEBUG( "3xkk - SE Vx, byte\n" );
                        if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] == OpCode & 0x00FF )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x4000:
                        // 4xkk - SNE Vx, byte
                        DEBUG( "4xkk - SNE Vx, byte\n" );
                        if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] != OpCode & 0x00FF )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x5000:
                        // 5xy0 - SE Vx, Vy
                        DEBUG( "5xy0 - SE Vx, Vy\n" );
                        if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] != Chip8Obj->registers[OpCode & 0x00F0 >> 4] )
                        {
                            Chip8Obj->programCounter    += 2;
                        }// registers

                        retVal                          = SUCCESS;
                        break;

                    case 0x6000:
                        // 6xkk - LD Vx, byte
                        DEBUG( "6xkk - LD Vx, byte\n" );
                        Chip8Obj->registers[OpCode & 0x0F00 >> 8]   =  OpCode & 0x00FF;

                        retVal                                      = SUCCESS;
                        break;

                    case 0x7000:
                        // 7xkk - ADD Vx, byte
                        DEBUG( "7xkk - ADD Vx, byte\n" );
                        Chip8Obj->registers[OpCode & 0x0F00 >> 8]   +=  OpCode & 0x00FF;

                        retVal                                      = SUCCESS;
                        break;

                    case 0x8000:
                        if( 0x0000 == OpCode & 0x000F )
                        {
                            // 8xy0 - LD Vx, Vy
                            DEBUG( "8xy0 - LD Vx, Vy\n" );
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   =  Chip8Obj->registers[OpCode & 0x00F0 >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0001 == OpCode & 0x000F )
                        {
                            // 8xy1 - OR Vx, Vy
                            DEBUG( "8xy1 - OR Vx, Vy\n" );
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   |=  Chip8Obj->registers[OpCode & 0x00F0 >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0002 == OpCode & 0x000F )
                        {
                            // 8xy2 - AND Vx, Vy
                            DEBUG( "8xy2 - AND Vx, Vy\n" );
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   &=  Chip8Obj->registers[OpCode & 0x00F0 >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0003 == OpCode & 0x000F )
                        {
                            // 8xy3 - XOR Vx, Vy
                            DEBUG( "8xy3 - XOR Vx, Vy\n" );
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   ^=  Chip8Obj->registers[OpCode & 0x00F0 >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0004 == OpCode & 0x000F )
                        {
                            // 8xy4 - ADD Vx, Vy
                            DEBUG( "8xy4 - ADD Vx, Vy\n" );

                            uint16_t sum                                = Chip8Obj->registers[OpCode & 0x0F00 >> 8] + Chip8Obj->registers[OpCode & 0x00F0 >> 4];
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
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   = sum & 0x00FF;

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0005 == OpCode & 0x000F )
                        {
                            // 8xy5 - SUB Vx, Vy
                            DEBUG( "8xy5 - SUB Vx, Vy\n" );

                            // If Vx > Vy, set VF to 1, else 0
                            if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] > Chip8Obj->registers[OpCode & 0x00F0 >> 4] )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// Vx > Vy

                            // Set Vx to Vx - Vy
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   -=  Chip8Obj->registers[OpCode & 0x00F0 >> 4];

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0006 == OpCode & 0x000F )
                        {
                            // 8xy6 - SHR Vx {, Vy}
                            DEBUG( "8xy6 - SHR Vx {, Vy}\n" );

                            // If LSB of Vx == 1, set VF to 1, else 0
                            if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] & 0x0001 )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// LSB of Vx == 1

                            // Divide Vx by 2
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   /=  2;
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0007 == OpCode & 0x000F )
                        {
                            // 8xy7 - SUBN Vx, Vy
                            DEBUG( "8xy7 - SUBN Vx, Vy\n" );

                            // If Vy > Vx, set VF to 1, else 0
                            if( Chip8Obj->registers[OpCode & 0x00F0 >> 4] > Chip8Obj->registers[OpCode & 0x0F00 >> 8] )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// Vy > Vx

                            // Vx = Vy - Vx

                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   =  Chip8Obj->registers[OpCode & 0x00F0 >> 4] - Chip8Obj->registers[OpCode & 0x0F00 >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x000E == OpCode & 0x000F )
                        {
                            // 8xyE - SHL Vx {, Vy}
                            DEBUG( "8xyE - SHL Vx {, Vy}\n" );

                            // If MSB of Vx == 1, set VF to 1, else 0
                            if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] & 0x0080 )
                            {
                                Chip8Obj->registers[0xF]                = 1;
                            }
                            else
                            {
                                Chip8Obj->registers[0xF]                = 0;
                            }// LSB of Vx == 1

                            // Multiply Vx by 2
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   *=  2;
                            retVal                                      = SUCCESS;

                        }// OpCode & 0x000F
                        break;

                    case 0x9000:
                        // 9xy0 - SNE Vx, Vy
                        DEBUG( "9xy0 - SNE Vx, Vy\n" );

                        // Skip next instruction if Vx != Vy
                        if( Chip8Obj->registers[OpCode & 0x0F00 >> 8] != Chip8Obj->registers[OpCode & 0x00F0 >> 4] )
                        {
                            Chip8Obj->programCounter                += 2;
                        }// Vx != Vy

                        retVal                                      = SUCCESS;
                        break;

                    case 0xA000:
                        // Annn - LD I, addr
                        DEBUG( "Annn - LD I, addr\n" );

                        // Set I = nnn
                        Chip8Obj->registerIndex                     = OpCode & 0x0FFF;

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
                        uint8_t randNum                             = rand() % 256;
                        Chip8Obj->registers[OpCode & 0x0F00 >> 8]   = ( OpCode & 0x00FF ) & randNum;
                        retVal                                      = SUCCESS;
                        break;

                    case 0xD000:
                        // Dxyn - DRW Vx, Vy, nibble
                        DEBUG( "Dxyn - DRW Vx, Vy, nibble\n" );

                        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
                        // TODO
                        retVal                                      = SUCCESS;
                        break;

                    case 0xE000:
                        if( 0x009E == OpCode & 0x00FF )
                        {
                            // Ex9E - SKP Vx
                            DEBUG( "Ex9E - SKP Vx\n" );

                            // Skip next instruction if key with the value of Vx is pressed
                            // TODO
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x00A1 == OpCode & 0x00FF )
                        {
                            // ExA1 - SKNP Vx
                            DEBUG( "ExA1 - SKNP Vx\n" );

                            // Skip next instruction if key with the value of Vx is not pressed
                            // TODO
                            retVal                                      = SUCCESS;
                        }// OpCode & 0x00FF

                        break;

                    case 0xF000:
                        if( 0x0007 == OpCode & 0x00FF )
                        {
                            // Fx07 - LD Vx, DT
                            DEBUG( "Fx07 - LD Vx, DT\n" );

                            // Set Vx = delay timer value
                            Chip8Obj->registers[OpCode & 0x0F00 >> 8]   = Chip8Obj->delayTimer;

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x000A == OpCode & 0x00FF )
                        {
                            // Fx0A - LD Vx, K
                            DEBUG( "Fx0A - LD Vx, K\n" );

                            // Wait for a key press, store the value of the key in Vx

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0015 == OpCode & 0x00FF )
                        {
                            // Fx15 - LD DT, Vx
                            DEBUG( "Fx15 - LD DT, Vx\n" );

                            // Set delay timer = Vx
                            Chip8Obj->delayTimer                        = Chip8Obj->registers[OpCode & 0x0F00 >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0018 == OpCode & 0x00FF )
                        {
                            // Fx18 - LD ST, Vx
                            DEBUG( "Fx18 - LD ST, Vx\n" );

                            // Set sound timer = Vx
                            Chip8Obj->soundTimer                        = Chip8Obj->registers[OpCode & 0x0F00 >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x001E == OpCode & 0x00FF )
                        {
                            // Fx1E - ADD I, Vx
                            DEBUG( "Fx1E - ADD I, Vx\n" );

                            // Set I = I + Vx
                            Chip8Obj->registerIndex                     += Chip8Obj->registers[OpCode & 0x0F00 >> 8];
                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0029 == OpCode & 0x00FF )
                        {
                            // Fx29 - LD F, Vx
                            DEBUG( "Fx29 - LD F, Vx\n" );

                            // Set I = location of sprite for digit Vx
                            // TODO

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0033 == OpCode & 0x00FF )
                        {
                            // Fx33 - LD B, Vx
                            DEBUG( "Fx33 - LD B, Vx\n" );

                            // Store BCD representation of Vx in memory locations I, I+1, and I+2
                            uint8_t vx                                  = Chip8Obj->registers[OpCode & 0x0F00 >> 8];

                            // Place hundreds digit in memory at location I
                            // Tens digit at location I+1
                            // Ones digit at location I+2
                            Chip8Obj->ram[Chip8Obj->registerIndex]      = ( vx / 100 ) % 10;
                            Chip8Obj->ram[Chip8Obj->registerIndex + 1]  = ( vx / 10 ) % 10;
                            Chip8Obj->ram[Chip8Obj->registerIndex + 2]  = vx % 10;

                            retVal                                      = SUCCESS;
                        }
                        else if( 0x0055 == OpCode & 0x00FF )
                        {
                            // Fx55 - LD [I], Vx
                            DEBUG( "Fx55 - LD [I], Vx\n" );

                            // Store registers V0 through Vx in memory starting at location I
                            for( unsigned int i = 0; i <= ( OpCode & 0x0F00 >> 8 ); i++ )
                            {
                                Chip8Obj->ram[Chip8Obj->registerIndex + i]  = Chip8Obj->registers[i];
                            }// for

                            retVal                                          = SUCCESS;
                        }
                        else if( 0x0065 == OpCode & 0x00FF )
                        {
                            // Fx65 - LD Vx, [I]
                            DEBUG( "Fx65 - LD Vx, [I]\n" );

                            // Read registers V0 through Vx from memory starting at location I
                            for( unsigned int i = 0; i <= ( OpCode & 0x0F00 >> 8 ); i++ )
                            {
                                Chip8Obj->registers[i]                  = Chip8Obj->ram[Chip8Obj->registerIndex + i];
                            }// for

                            retVal                                      = SUCCESS;
                        }// OpCode & 0x00FF

                        break;

                    default:
                        fprintf( stderr, "UNKNOWN OpCode: 0x%04X\n", OpCode );
                        break;

                }// switch OpCode
    }// NULL args

    OUT_FUNC;
    return retVal;
}