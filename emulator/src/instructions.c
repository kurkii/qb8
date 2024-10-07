#include "../include/emu.h"
#include <stdio.h>

extern registers_t registers;
extern uint8_t *memory;
extern uint32_t program_counter;

int addx(int operand){
    registers.x += operand;

    if((registers.x + operand) > 255){ // overflow
        registers.ccr.c = 1;
    }else{
        registers.ccr.c = 0;
    }

    if((registers.x + operand) == 0){
        registers.ccr.z = 1;
    }else{
        registers.ccr.z = 0;
    }

    return 0;
}

int adda(int operand){

    if((registers.a + operand) > 255){ // overflow
        registers.ccr.c = 1;
        return 0;
    }else{
        registers.ccr.c = 0;
    }

    if((registers.a + operand) == 0){
        registers.ccr.z = 1;
    }else{
        registers.ccr.z = 0;
    }

    registers.a += operand;

    return 0;
}

int addb(int operand){
    registers.b += operand;

    if((registers.b + operand) > 255){ // overflow
        registers.ccr.c = 1;
    }else{
        registers.ccr.c = 0;
    }

    if((registers.b + operand) == 0){
        registers.ccr.z = 1;
    }else{
        registers.ccr.z = 0;
    }

    return 0;
}

int mova(int operand){
    registers.a = operand;
    return 0;
}

int movb(int operand){
    registers.b = operand;
    return 0;
}

int moab(void){
    registers.b = registers.a;
    return 0;
}

int moba(void){
    registers.a = registers.b;
    return 0;
}

int lda(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    registers.a = memory[operand];
    return 0;
}

int sta(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    memory[operand] = registers.a;
    return 0;
}

int jmp(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    return operand;
}

int jmz(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    if(registers.ccr.z == 1){
        return operand;
    }

    return -1;
}

int jnz(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    if(registers.ccr.z == 0){
        return operand;
    }

    return -1;
}

int jcs(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    if(registers.ccr.c == 1){
        return operand;
    }

    return -1;
}

int pta(void){

    printf("%d\n", registers.a);
    return 0;
}

int aax(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    registers.x += memory[operand];

    if((registers.x + memory[operand]) > 255){ // overflow
        registers.ccr.c = 1;
    }else{
        registers.ccr.c = 0;
    }

    if((registers.x + memory[operand]) == 0){
        registers.ccr.z = 1;
    }else{
        registers.ccr.z = 0;
    }
    

    return 0;

}

int movx(int operand){

    if(operand > MEMORY_LOCATIONS){
        return -1;
    }

    registers.x = memory[operand];
    return 0;

}



