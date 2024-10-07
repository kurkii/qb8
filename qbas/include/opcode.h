/* Everything relating to opcodes */

enum {
    ADDX_BIN    = 0b0000,
    ADDA_BIN    = 0b0001,
    ADDB_BIN    = 0b0010,
    MOVA_BIN    = 0b0011,
    MOVB_BIN    = 0b0100,
    MOAB_BIN    = 0b0101,
    MOBA_BIN    = 0b0110,
    LDA_BIN     = 0b0111,
    STA_BIN     = 0b1000,
    JMP_BIN     = 0b1001,
    JMZ_BIN     = 0b1010,
    JNZ_BIN     = 0b1011,
    PTA_BIN     = 0b1100,
    JCS_BIN     = 0b1101,
    AAX_BIN     = 0b1110,
    MOVX_BIN    = 0b1111
};