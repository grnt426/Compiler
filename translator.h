#ifndef translator_h_
#define translator_h_

// debug print control
#define DEBUG_ // general purpose debug messages

// Machine Constraints
#define MAX_REGS 2
#define REG_ONE "$1"
#define REG_TWO "$2"
#define MAX_MEMORY 28
#define MAX_CACHE 6
#define MAX_LINE_LEN 32
#define WORD_SIZE 7

// (Real) Instructions
// Name Code Ext. Code Description
#define NOT "0000" // bitwise not of $source into $dest
#define SHL "0001" // a left bitwise shift, once
#define SHR "0010" // a right bitwise shift, once
#define OR "0011" // bitwise or of $s1 and $s2 into $dest
#define AND "0100" // bitwise and of $s1 and $s1 into $dest
#define ADD "0101" // binary addition of $s1 and $s2 into $dest
#define SW "0110" // store word to cache
#define LW "0111" // load word from cache into $dest
#define BEZ "1000" // branch if $source equal to zero
#define ROT1 "1001000" // increase cache index by one
#define ROT "10010" // increase cache index by $source
#define LROT "10011"
#define LJMP "1010" // apply scalar to next jump
#define JMP "1011" // next value is a pointer
#define HALT "1111000" // stops all processing
#define NOP "1111100" // consumes a cycle

// Instruction Format Codes
#define NOT_F "sd"
#define SHL_F "sd"
#define SHR_F "sd"
#define OR_F "ssd"
#define AND_F "ssd"
#define ADD_F "ssd"
#define SW_F "s"
#define LW_F "d"
#define BEZ_F "sl"
#define ROT_F "s"
#define ROT1_F ""
#define JMP_F "[lcn]"
#define NOP_F ""
#define HALT_F ""

// Flags
#define WARN_FLAG "-w"
#define SYST_FLAG "-s"
#define COMP_INFO "-i"
#define HELP_FLAG "-h"
#define FAST_FLAG "-f"

struct program;
struct Term;

// Compilation Functions
void process_input_program(struct program *prog);

// Program File Output Functions
void write_str(char *str, FILE *out);
void write_instruc_str(char *str, short s1, short s2, short dest, char *misc,
struct program *prog);

// Instruction Processing Fucntions
void process_token(char *tok, struct program *prog);
void process_instruction(struct program *prog, char *opcode, const char *fmt,
char *misc);

// Term translation
void translate_terms(struct Term *t, struct program *prog);
void write_terms(struct Term *t, struct program *prog);

// Register Processing Instructions
short read_src_reg(struct program *prog, short suppress);
short read_dst_reg(struct program *prog, short suppress);
char *read_reg(struct program *prog, short suppress);
char *conv_reg_to_str(char *buf, short reg);

// Error Handling Functions
void check_garbage(struct program *prog);
void check_warnings(struct program *prog);

// Miscellaneous File/String Manipulators
FILE *open_write_file(const char *file);

// Other
void print_help(const char *prog_name);

#endif