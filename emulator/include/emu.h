#include <ncurses.h>
#include <stdint.h>

#ifndef EMU_H
#define EMU_H

#define MEMORY_LOCATIONS 4096

typedef struct scroll_status {
    int y_cursor;
    int index;
} scroll_status;

typedef struct ccr_t {
    unsigned c : 1; // carry
    unsigned z : 1; // zero
    unsigned n : 1; // negative
} __attribute__ ((__packed__))ccr_t;

typedef struct registers_t{
    uint8_t a;
    uint8_t b;
    uint8_t x;
    unsigned pc : 12;
    
    ccr_t ccr;

} __attribute__ ((__packed__))registers_t;

enum {
    ADDX_OPCODE = 0,
    ADDA_OPCODE,
    ADDB_OPCODE,
    MOVA_OPCODE,
    MOVB_OPCODE,
    MOAB_OPCODE,
    MOBA_OPCODE,
    LDA_OPCODE,
    STA_OPCODE,
    JMP_OPCODE,
    JMZ_OPCODE,
    JNZ_OPCODE,
    PTA_OPCODE,
    JCS_OPCODE,
    AAX_OPCODE,
    MOVX_OPCODE,
};

int addx(int operand);

int adda(int operand);

int addb(int operand);

int mova(int operand);

int movb(int operand);

int moab(void);

int moba(void);

int lda(int operand);

int sta(int operand);

int jmp(int operand);

int jmz(int operand);

int jnz(int operand);

int jcs(int operand);

int pta(void);

int aax(int operand);

int movx(int operand);

int init_windowing(void);
int get_instruction(char *buffer, uint16_t *memory, int address);

int run_opcode(uint16_t *memory, int address);

void print_titlebar(char *right, char *left, int max_x);
void create_border(WINDOW *win1, WINDOW *win2, WINDOW *win3, char symbol_horizontal, char symbol_vertical, char* title1, char* title2, char* title3);
void print_state(WINDOW *win);
void execute_op(uint16_t address); // execute command at address
int msleep(long msec); // sleep for msec number of milliseconds
void print_help(WINDOW *win);

#endif