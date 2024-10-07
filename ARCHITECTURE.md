# qb8

Toy 8-bit CPU architecture with its own assembly language

## Features:

- 8-bit `a`, `b` registers,
- 8-bit word width,
- 12-bit address width,
- `4KiB` of memory,
- 16-bit instructions:

    4-bits reserved for operator, 12-bits for operand

    `operand  - v`

    `1011 011101010101`
    
    `^ - operator`

## Instruction set:

- 16 instructions
    
| Instruction mneomonic | Binary form | Example code | Description |
| ----------- | ----------- | ----------- | ----------- |
| `addx` | `0000 0000` | `addx, 43` | Adds operand literal to value in `x` register |
| `adda` | `0000 0001` | `adda, 12` | Adds operand literal to value in `a` register |
| `addb` | `0000 0010` | `addb, 13` | Adds operand literal to value in `b` register |
| `mova` | `0000 0011` | `mova, 5` | Moves operand literal to `a` register |
| `movb` | `0000 0100` | `movb, 89` | Moves operand literal to `b` register |
| `moab` | `0000 0101` | `moab` | Moves value from `a` register to `b` register |
| `moba` | `0000 0110` | `moba` | Moves value from `b` register to `a` register |
| `lda` | `0000 0111` | `lda, $45` | Loads value from memory address in operand to `a` register |
| `sta` | `0000 1000` | `str, $12` | Stores value to memory address in operand from `a` register |
| `jmp` | `0000 1001` | `jmp, $60` | Jumps to specified memory address |
| `jmz` | `0000 1010` | `jmz, $53` | Jumps to specified memory address if `z` bit in `CCR` register is set to `1` |
| `jnz` | `0000 1011` | `jnz, $25` | Jumps to specified memory address if `z` (zero) bit in `CCR` register is set to `0` |
| `pta` | `0000 1100` | `pta` | Prints `a` register |
| `jcs` | `0000 1101` | `jcs, $23` | Jumps to specified memory address if `c` (carry) bit in `CCR` register is set to `1` |
| `aax` | `0000 1110` | `aax, $12` | Add value at the address specified by operand to `x` register |
| `movx` | `0000 1111` | `movx, $6` | Moves value in address specified by operand to `x` register |

## Registers:

### General purpose:

- `a` - 8-bit general purpose register
- `b` - 8-bit general purpose register
- `x` - 8-bit general purpose register

### Status registers:

#### CCR Register

| Flag name | Mneomonic | Bit | Description |
| ----------- | ----------- | ----------- | ----------- |
| Carry | `c` | `0` | Set to `1` if previous operation had remainder |
| Zero | `z` | `1` | Set to `1` if previous operation returned zero  |
| Negative | `n` | `2` | Set to `1` if previous operation returned a negative number  |

### Special purpose registers

| Register name | Mneomonic | Width | Description |
| ----------- | ----------- | ----------- | ----------- |
| Program counter | `pc` | `12-bit` | Holds memory address of the next instruction |











