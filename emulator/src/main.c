
/*
    Plan:
        - parse through qb8 binary file
        - global variables acting as registers
        - easy

*/

#include "../include/emu.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define CONTINUE_DELAY_MS 10
#define MAX_PROGRAM_SIZE 512

enum FOCUS {
    NONE = 0,
    MEMORY,
    PROGRAM,
    STATUS
};

#define bin "bin.out"

uint8_t focus_state = NONE; // which window is in focus
int current_line_program = 0; // current selected line in the program window
int current_line_memory = 0; // current selected line in the memory window

char *program_buffer[MAX_PROGRAM_SIZE] = {0};

registers_t registers = {0};

uint16_t *memory = {0};

uint16_t program_counter = 0;

int main(void){
    memory = malloc(sizeof(uint8_t) * MEMORY_LOCATIONS);

    FILE *fileptr = fopen(bin, "r");

    if(fileptr == NULL){
        fprintf(stderr, "File not found");
    }

    int buffer = 0;

    int program_size = 0; // index and size of file in bytes

    /* read binary file and put it in memory */
    while((buffer = fgetc(fileptr)) != EOF){

        if(program_size >= MEMORY_LOCATIONS){
            fprintf(stderr, "Binary file too big! Over %d bytes\n", MEMORY_LOCATIONS);
            return -1;
        }
        memory[program_size] = buffer;
        program_size++;
    }

    memory[program_size] = 4096; // indicate end of program

    int max_y,max_x = 0;
    

    for(int j; j <= program_size; j++){
        printf("memory[%d]: %d\n", j, memory[j]);
    }

    initscr();			/* Start curses mode 		  */

    if(has_colors() == FALSE){
        endwin();
        printf("Your terminal has no support for colors :(\n");
        return 0;
    }

    start_color();

    raw();

    keypad(stdscr, TRUE); // enable F1, F2... keys

    noecho();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    getmaxyx(stdscr, max_y, max_x);

    if(max_x < 40 || max_y < 25){
        endwin();
        fprintf(stderr, "Terminal size too small! Requires atleast 40x25 while you have %dx%d\n", max_x, max_y);
        return 1;
    }

    /* Create windows */
    int win1_maxy = (max_y-1)/2;
    int win1_maxx = max_x/2;

    int win2_maxy = max_y-1;
    int win2_maxx = max_x/2;

    int win3_maxy = (max_y-1)/2;
    int win3_maxx = max_x/2;


    WINDOW *win1 = newwin(win1_maxy, win1_maxx, 1, 0);
    WINDOW *win2 = newwin(win2_maxy, win2_maxx, 1, win2_maxx);
    WINDOW *win3 = newwin(win3_maxy, win3_maxx, win3_maxy+1, 0);

    keypad(win1, TRUE);
    keypad(win2, TRUE);

    print_titlebar("Press F5 for help", "qb8 emulator", max_x);
    
    create_border(win1, win2, win3, '-', '|', "Memory", "Program", "Status");
    
    wmove(win1, 1, 1);
    wmove(win2, 1, 1);

    /* parse memory and write instructions to the program window */

    int i = 0; // index for parsing program_buffer array
    int j = 0; // index for parsing memory
    int y = 1; // index for y position of the cursor
    bool end_of_program = false;

    while(1) {

        if(i > MAX_PROGRAM_SIZE){
            endwin();
            fprintf(stderr, "File too big, over %d instructions\n", MAX_PROGRAM_SIZE);
            return 1;
        }

        char *instruction = malloc(sizeof(char) * 10);
        memset(instruction, 0, 10);

        printf("memory[i]: %d\n", memory[j]);
        int ret;
        if((ret = get_instruction(instruction, memory, j)) == -1){
            fprintf(stderr, "Error parsing on line %d\n", j);
            endwin();
            return -1;
        }

        if(ret == -2){
            sprintf(instruction, "==FIN==");
            fprintf(stderr, "End of program\n");
            end_of_program = true;
        }

        program_buffer[i] = instruction;
        if(y < win2_maxy-1){
            mvwprintw(win2, y, 1, "%s", instruction);
        }

        if(end_of_program == true){
            break;
        }

        j += ret;
        y++;
        i++;
    }

    int program_buffer_size = i;

    /* parse memory and print it to the memory window */
    for(int i = 1; i < MEMORY_LOCATIONS; i++){
        if(i == win1_maxy-1){
            /* reached the end of the window */
            break;
        }
        mvwprintw(win1, i, 1, "addr %d: %d", i-1, memory[i-1]); // i-1 as memory locations start at memory[0] but y = 0 is part of the window border
    }

    print_state(win3);

    refresh();
    wrefresh(win1);
    wrefresh(win2);
    wrefresh(win3);

    int y_program = 0; // current y coordinate of the cursor in the program window
    int y_memory = 0; // current y coordinate of the cursor in the memory window
    int memory_index = 0; // memory index for parsing when scrolling
    int program_index = 0; // program index for parsing when scrolling

    char *instruction = malloc(sizeof(char) * 10); // for 'm' memory command to find location of program
    int ret; // return code for get_instruction() for memory command
    int instruction_addr = 0; // address of instruction for memory command

    for(;;){
        int c = getch();

        switch (c) {
            case KEY_F(1):
                focus_state = MEMORY;
                break;
            case KEY_F(2):
                focus_state = PROGRAM;
                break;
            case KEY_F(3):
                focus_state = STATUS;
                break;
            case KEY_F(5):
                print_help(win3);
                while (1) {
                    int c = getch();

                    if(c == KEY_F(5)){
                        print_state(win3);
                        break;
                    }
                }
                break;
            case 'x':
            case 'X': // exit
                endwin();
                exit(0);
            case 's':
            case 'S': // step
                if(program_counter >= program_size){
                    break;
                }

                program_counter = run_opcode(memory, program_counter);
                registers.pc = program_counter;
                print_state(win3);
                break;
            case 'r':
            case 'R': // reset
                program_counter = 0;
                memset(&registers, 0, sizeof(registers_t)); // set everything to zero
                print_state(win3);
                break;
            case 'c':
            case 'C': // continue
                nodelay(stdscr, TRUE);
                y_memory = 2;
                while (1) {
                    msleep(CONTINUE_DELAY_MS);
                    if(program_counter >= program_size){
                        break;
                    }

                    int d = getch();
                    if(d != ERR){ // ERR means no input in nodelay mode
                        break;
                    }


                    program_counter = run_opcode(memory, program_counter);
                    registers.pc = program_counter;
                    print_state(win3);

                    /* highlight current instruction */
                    mvwchgat(win1, program_counter, 1, 16, A_NORMAL, 0, NULL);


                }
                nodelay(stdscr, FALSE);
                break;
            case 'M': // memory
            case 'm':
                switch (focus_state) {
                    case MEMORY:
                        memset(instruction, 0, 10);
                        ret = get_instruction(instruction, memory, y_memory+2); // dont ask me why its +2

                        if(ret < 0){
                            break;
                        }

                        for(int i = 0; i < program_buffer_size; i++){
                            if(strcmp(program_buffer[i], instruction) == 0){
                                instruction_addr = i;
                                break;
                            }
                        }

                        if(instruction_addr > 0){
                            mvwchgat(win2, y_program, 1, 10, A_NORMAL, 0, NULL); // normalize the previous line
                            y_program = instruction_addr;
                            mvwchgat(win2, y_program, 1, 10, A_NORMAL, 1, NULL);
                        }
                    break;
                }

                break;
            case KEY_DOWN: // scroll down on focused window

                switch (focus_state) {
                    case PROGRAM:
                        if(program_index >= program_buffer_size){
                            break;
                        }

                        if(y_program > win2_maxy-3){ // if cursor is bigger than last line before border
                            /* cursor is bigger than window size - scroll down */


                            wclear(win2); // clear the window
                            create_border(NULL, win2, NULL, '-', '|', NULL, "Program", NULL); // create border box

                            for(int i = 1; i < win2_maxy-1; i++){
                                if(program_buffer[i+program_index] == NULL){
                                    continue; // find better solution
                                }

                                mvwprintw(win2, i, 1, "%s", program_buffer[i+program_index]);
                            }

                            program_index++;
                            y_program = win2_maxy-3; // puts cursor on the last line before border
                        }
                        mvwchgat(win2, y_program, 1, 10, A_NORMAL, 0, NULL); // normalize the previous line
                        mvwchgat(win2, ++y_program, 1, 10, A_NORMAL, 1, NULL); // color the current line
                        break;
                    case MEMORY:
                        if(y_memory > win1_maxy-3){ // if cursor is bigger than last line before border
                            /* cursor is bigger than window size - scroll down */

                            wclear(win1); // clear the window
                            create_border(win1, NULL, NULL, '-', '|', "Memory", NULL, NULL); // create border box

                            for(int i = 1; i < win1_maxy-1; i++){

                                if(i + memory_index >= MEMORY_LOCATIONS){
                                    /* reached the end */
                                    break;
                                }

                                mvwprintw(win1, i, 1, "addr %d: %d", i+memory_index, memory[i+memory_index]);
                            }
                            memory_index++;
                            y_memory = win1_maxy-3; // puts cursor on the last line before border
                        }

                        mvwchgat(win1, y_memory, 1, 16, A_NORMAL, 0, NULL);
                        mvwchgat(win1, ++y_memory, 1, 16, A_NORMAL, 1, NULL);
                        break;     
                    default:
                        break;
                }

                break;

            case KEY_UP:
                switch (focus_state) {
                    case PROGRAM:
                        /* prevent user from going out of bounds of array */
                        if((program_index < 0) && (y_program <= 2)){
                            program_index = 0;
                            break;
                        }

                        if(y_program <= 2){ // if cursor is on the first line
                            /* cursor is on the first line - scroll up */
                            wclear(win2); // clear the window
                            create_border(NULL, win2, NULL, '-', '|', NULL, "Program", NULL); // create border box
                        
                            for(int i = 1; i < win2_maxy-1; i++){
                                if(program_buffer[i+program_index-1] == NULL){
                                    continue; // find better solution
                                } 

/*                                 if((i+program_index-1) < 0){ // find better solution
                                    for(int i = 1; i < win2_maxy-1; i++){
                                        mvwprintw(win2, i, 1, "%s", program_buffer[i+program_index]);
                                    }

                                    program_index = 0;
                                    break;
                                } */

                                mvwprintw(win2, i, 1, "%s", program_buffer[i+program_index-1]); // i - 1 because array indexing starts at 0, while i is 1 so it doesnt print to the border
                            }


                            program_index--;
                            y_program = 2; // puts cursor on the first line
                        }

                        mvwchgat(win2, y_program, 1, 10, A_NORMAL, 0, NULL); // normalize the previous line
                        mvwchgat(win2, --y_program, 1, 10, A_NORMAL, 1, NULL); // color the current line
                        break;
                    case MEMORY:\

                        /* prevent user from going out of bounds of array */
                        if((memory_index < 0) && (y_memory <= 2)){
                            memory_index = 0;
                            break;
                        }

                        if(y_memory <= 2){ // if cursor is on the first line
                            /* cursor is on the first line - scroll up */
                            wclear(win1); // clear the window
                            create_border(win1, NULL, NULL, '-', '|', "Memory", NULL, NULL); // create border box
                        
                            for(int i = 1; i < win1_maxy-1; i++){

                                /* prevent user from going out of bounds of array */
/*                                 if((i+memory_index-1) < 0){
                                    for(int i = 1; i < win2_maxy-1; i++){
                                        mvwprintw(win1, i, 1, "addr %d: %d", i, memory[i+memory_index-1]);
                                    }
                                    memory_index = 0;
                                    break;
                                } */

                                mvwprintw(win1, i, 1, "addr %d: %d", i+memory_index-1, memory[i+memory_index-1]); // i - 1 because array indexing starts at 0, while i is 1 so it doesnt print to the border
                            }


                            memory_index--;
                            y_memory = 2; // puts cursor on the first line
                        }

                        mvwchgat(win1, y_memory, 1, 16, A_NORMAL, 0, NULL); // normalize the previous line
                        mvwchgat(win1, --y_memory, 1, 16, A_NORMAL, 1, NULL); // color the current line
                        break;
                }

            default:
                break;
        }

        /* change window focus */
        switch (focus_state) {
            case MEMORY: // win1
                mvwchgat(win1, 0, 0, 6, A_BOLD, 1, NULL);
                mvwchgat(win2, 0, 0, 7, A_NORMAL, 0, NULL);
                mvwchgat(win3, 0, 0, 6, A_NORMAL, 0, NULL);
                break;
            case PROGRAM:
                mvwchgat(win1, 0, 0, 6, A_NORMAL, 0, NULL);
                mvwchgat(win2, 0, 0, 7, A_BOLD, 1, NULL);
                mvwchgat(win3, 0, 0, 6, A_NORMAL, 0, NULL);
                break;
            case STATUS:
                mvwchgat(win1, 0, 0, 6, A_NORMAL, 0, NULL);
                mvwchgat(win2, 0, 0, 7, A_NORMAL, 0, NULL);
                mvwchgat(win3, 0, 0, 6, A_BOLD, 1, NULL);
                break;
            default:
                break;
        }
        refresh();
        wrefresh(win1);
        wrefresh(win2);
        wrefresh(win3);

    }

    endwin();


    return 0;
}

void print_titlebar(char *right, char *left, int max_x){
    mvprintw(0, 0, "%s", left);

    size_t length = strlen(left);
    for(int i = 0; i < max_x; i++){
        mvaddch(0, length+1+i, '^');
    }

    mvprintw(0, max_x-strlen(right), "%s", right);
}

void create_border(WINDOW *win1, WINDOW *win2, WINDOW *win3, char symbol_horizontal, char symbol_vertical, char* title1, char* title2, char* title3){

    if(win1 != NULL){
        box(win1, symbol_vertical, symbol_horizontal);
        wprintw(win1, "%s", title1);
    }

    if (win2 != NULL) {
        box(win2, symbol_vertical, symbol_horizontal);
        wprintw(win2, "%s", title2);
    }
    
    if (win3 != NULL) {
        box(win3, symbol_vertical, symbol_horizontal);
        wprintw(win3, "%s", title3);
    }

    refresh();
    wrefresh(win1);
    wrefresh(win2);
    wrefresh(win3);

}

void print_state(WINDOW *win){
    wclear(win);

    create_border(NULL, NULL, win, '-', '|', NULL, NULL, "Status");
    int y = 2;
    int x = 1;
    mvwprintw(win, y, x, "a: %d           b: %d", registers.a, registers.b);
    y++;
    mvwprintw(win, y, x, "x: %d           pc: %d\n", registers.x, registers.pc);
    y++;
    mvwprintw(win, y, x, "ccr: c:%d, z:%d, n:%d", registers.ccr.c, registers.ccr.z, registers.ccr.n);

    refresh();
    wrefresh(win);

}

int msleep(long msec){
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void print_help(WINDOW *win){
    wclear(win);
    box(win, '|', '-');
    mvwprintw(win, 0, 0, "Help");

    mvwprintw(win, 2, 1, "qb8 emulator version 1.0");
    mvwprintw(win, 5, 1, "F1, F2, F3 - Focus on memory window, program window and status window respectively");
    mvwprintw(win, 7, 1, "Use up and down arrow keys to move up and down the windows");
    mvwprintw(win, 9, 1, "c - Continue executing the program till the end. If a key is pressed it will stop");
    mvwprintw(win, 11, 1, "s - Step through exeucting the program instruction at a time");
    mvwprintw(win, 13, 1, "m - Find instruction which correlates on selected memory address (very broken)");
    mvwprintw(win, 15, 1, "r - Reset, clear the registers and put pc back to 0");
    mvwprintw(win, 17, 1, "x - Exit the program");

     mvwprintw(win, 19, 1, "Press F5 to exit this menu");

    wrefresh(win);
    refresh();
}

