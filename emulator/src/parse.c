
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/emu.h"

extern registers_t registers;

int run_opcode(uint16_t *memory, int address){ /* fix ccr shit */
    int ret = 0;

    switch (memory[address]) {
        case ADDX_OPCODE:
            addx((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case ADDA_OPCODE:
            adda((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case ADDB_OPCODE:
            addb((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case MOVA_OPCODE:
            mova((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case MOVB_OPCODE:
            movb((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case MOAB_OPCODE:
            moab();
            return address+1;
            break;
        case MOBA_OPCODE:
            moba();
            return address+1;
            break;
        case LDA_OPCODE:
            lda((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case STA_OPCODE:
            sta((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case JMP_OPCODE:
            return jmp((memory[address+2] << 8) | memory[address+1]);;
            break;
        case JMZ_OPCODE:
            ret = jmz((memory[address+2] << 8) | memory[address+1]);

            if(ret == -1){
                return address+3;
            }
            return ret;

            break;
        case JNZ_OPCODE:
            ret = jnz((memory[address+2] << 8) | memory[address+1]);

            if(ret == -1){
                return address+3;
            }

            return ret;
            break;
        case JCS_OPCODE:
            ret = jcs((memory[address+2] << 8) | memory[address+1]);
            fprintf(stderr, "ret: %d\n", ret);
            if(ret == -1){
                return address+3;
            }

            return ret;
            break;
        case PTA_OPCODE:
            pta();
            return address+1;
            break;
        case AAX_OPCODE:
            aax((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        case MOVX_OPCODE:
            movx((memory[address+2] << 8) | memory[address+1]);
            return address+3;
            break;
        default:
            return -1; 
    }
    return 0;
}

int get_instruction(char *buffer, uint16_t *memory, int address){

    int ret = 0;

    if(memory[address] == 0x1000){
        return -2; // end of program
    }

    uint16_t operand = 0;
    switch (memory[address]) {
        case ADDX_OPCODE:
            sprintf(buffer, "addx, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case ADDA_OPCODE:
            sprintf(buffer, "adda, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case ADDB_OPCODE:
            sprintf(buffer, "addb, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case MOVA_OPCODE:
            sprintf(buffer, "mova, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case MOVB_OPCODE:
            sprintf(buffer, "movb, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case MOAB_OPCODE:
            sprintf(buffer, "moab");
            ret++;
            return ret;
        case MOBA_OPCODE:
            sprintf(buffer, "moba");
            ret++;
            return ret;
        case LDA_OPCODE:
            sprintf(buffer, "lda, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case STA_OPCODE:
            sprintf(buffer, "sta, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case JMP_OPCODE:
            sprintf(buffer, "jmp, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case JMZ_OPCODE:
            sprintf(buffer, "jmz, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case JNZ_OPCODE:
            sprintf(buffer, "jnz, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case JCS_OPCODE:
            sprintf(buffer, "jcs, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case PTA_OPCODE:
            sprintf(buffer, "pta");
            ret++;
            return ret;
        case AAX_OPCODE:
            sprintf(buffer, "aax, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        case MOVX_OPCODE:
            sprintf(buffer, "movx, ");
            operand = (memory[address+2] << 8) | memory[address+1];
            sprintf(buffer + strlen(buffer), "%d", operand);
            ret += 3;
            return ret;
        default:
            return -1;
    }
    return 0;
}