/*
    Plan:
        - parse the source file
        - split operator and operand
        - strcmp operator, check if operand valid...
        - convert operand to interger
        - fprintf into binary file

    Source file structure:
        adda, 43
         ^     ^--- Operand - Literal
        Mneomonic

        movx, $7
              ^--- Operand - Address ($ sign)
*/     
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include "../include/opcode.h"

#define src "bin.as"
#define bin "bin.out"
#define MAX_LINE_LENGTH 0x1000
#define MAX_OPERATOR_LENGTH 0x5

int find_opcode(char *name);

void remove_whitespace(char *buffer){ 
/*     char *string = buffer;
    size_t i = 0;
    while (string[i] == ' ' || string[i] == 9){ // remove leading white space and tab
        for(size_t j = 0; j < strlen(string); j++){ // shift the string
            string[j] = string[j+1];
        }
    }
    
    for(int j = strlen(string+1); j > 0; j--){ // remove trailing white space
        if(!isspace(string[j])){
            break;
        }
        string[j] = '\0';
    }

    return string; */

    for(size_t i = 0; i < strlen(buffer); i++){
        if(isspace(buffer[i])){
            size_t j;
            for(j = i; j < strlen(buffer); j++){ // shift the string
                buffer[j] = buffer[j+1];
            }
        }
    }
}

#include <stdio.h>
int main(void){

    FILE *fileptr = fopen(src, "r");
    if(fileptr == NULL){
        fprintf(stderr, "File not found\n");
        return 1;
    }   

    FILE *binary = fopen(bin, "w+");

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    char *operator = malloc(MAX_OPERATOR_LENGTH * sizeof(char));
    char *operand = malloc(MAX_LINE_LENGTH * sizeof(char));

    while(fgets(line, MAX_LINE_LENGTH, fileptr) != NULL){

        if(line[0] == 10){ // empty line
            continue;
        }
        
        int i = 0; // index
        memset(operator, 0 , MAX_OPERATOR_LENGTH);
        memset(operand, 0 , MAX_LINE_LENGTH);

        remove_whitespace(line);

        printf("line: %s\n", line);

        bool isoperand = true; 
        while(line[i] != ','){ // parse operator

            if(i > 256){
                printf("Invalid line %d\n", line_number);
            }

            if(line[i] == 0){
                isoperand = false;
                break;
            }

            if(i == MAX_OPERATOR_LENGTH){
                fprintf(stderr, "Invalid operator `%s` on line %d\n", operator, line_number);
                return 0;
            }

            operator[i] = line[i];

            i++;
        }



        /* everything after is the operand */

        size_t operator_length = i*sizeof(char);

        memcpy(operand, line+operator_length+1, strlen(line) - operator_length); // copy the rest of the line to the `operand` variable

        printf("operator: %s\n", operator);
        printf("operand: %s\n", operand);

        /* lowercase the operator */
        for(size_t i = 0; i < strlen(operator); i++){
            operator[i] = tolower(operator[i]);
            printf("name[%zu]: %d\n", i, operator[i]);
        }

        int opcode = find_opcode(operator);

        fwrite(&opcode, 1, 1, binary);

        if(isoperand == false){ // if there is no operand then skip last part

            switch (opcode){
                case MOBA_BIN:
                case MOAB_BIN:
                case PTA_BIN:
                    break; // opcodes dont require operand
                default: // these ones do
                    fprintf(stderr, "Missing operand for instruction %s on line %d\n", operator, line_number);
                    return -1;
            }

            continue;
        }

        long operand_value = strtol(operand, NULL, 10);

        if(operand_value == 0 && (strcmp("0", operand) != 0)){
            fprintf(stderr, "Invalid operand on line %d, operand must be an integer\n", line_number);
            return 1;
        }

        int error = errno;

        if(error != 0){
            printf("Failed to parse operand on line %d: %s\n", line_number, strerror(error));
        }

        if(operand_value > 4095){
            fprintf(stderr, "Invalid operand on line %d, operand must be less than 1.5 octets (4096)\n", line_number);
        }
        printf("operand: %ld\n", operand_value);
        fwrite(&operand_value, 2, 1, binary);

        line_number++;
    }
}

int find_opcode(char *name){
    int opcode = -1;

    if(strcmp(name, "addx") == 0){
        opcode = ADDX_BIN;
    }else if(strcmp(name, "adda") == 0){
        opcode = ADDA_BIN;
    }else if(strcmp(name, "addb") == 0){
        opcode = ADDB_BIN;
    }else if(strcmp(name, "mova") == 0){
        opcode = MOVA_BIN;
    }else if(strcmp(name, "movb") == 0){
        opcode = MOVB_BIN;
    }else if(strcmp(name, "moab") == 0){
        opcode = MOAB_BIN;
    }else if(strcmp(name, "moba") == 0){
        opcode = MOBA_BIN;
    }else if(strcmp(name, "lda") == 0){
        opcode = LDA_BIN;
    }else if(strcmp(name, "sta") == 0){
        opcode = STA_BIN;
    }else if(strcmp(name, "jmp") == 0){
       opcode = JMP_BIN; 
    }else if(strcmp(name, "jmz") == 0){
        opcode = JMZ_BIN;
    }else if(strcmp(name, "jnz") == 0){
        opcode = JNZ_BIN;
    }else if(strcmp(name, "pta") == 0){
        opcode = PTA_BIN;
    }else if(strcmp(name, "jcs") == 0){
        opcode = JCS_BIN;
    }else if(strcmp(name, "aax") == 0){
        opcode = AAX_BIN;
    }else if(strcmp(name, "movx") == 0){
        opcode = MOVX_BIN;
    }else{
        fprintf(stderr, "Opcode %s doesn't exist\n", name);
        exit(EXIT_FAILURE);
    }

    return opcode;

}