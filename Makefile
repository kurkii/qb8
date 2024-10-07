BASE_EMU = emulator/src/instructions.c emulator/src/main.c emulator/src/parse.c
BASE_AS = qbas/src/main.c
BUILD = target
CC = gcc
CFLAGS = -Wall -Wno-unused-parameter -Wextra
CDEBUG = -ggdb3 -DDEBUG

.PHONY: emulator debug_emulator assembler debug_assembler

emulator:
	mkdir $(BUILD) || true
	$(CC) $(BASE_EMU) $(CFLAGS) -lncurses -o $(BUILD)/qb8_emulator

debug_emulator:
	$(CC) $(BASE_EMU) $(CFLAGS) $(CDEBUG) -lncurses -o $(BUILD)/qb8_emulator

assembler:
	mkdir $(BUILD) || true
	$(CC) $(BASE_AS) $(CFLAGS) -o $(BUILD)/qbas

debug_assembler:
	$(CC) $(BASE_AS) $(CFLAGS) $(CDEBUG) -o $(BUILD)/qbas

clean:
	rm target/*
