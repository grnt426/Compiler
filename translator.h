#ifndef translator_h
#define translator_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// debug print control
#define DEBUG	// general purpose debug messages

// Machine Constraints
#define MAX_REGS		2
#define REG_ZERO		"$0"
#define REG_ONE			"$1"
#define MAX_MEMORY		28
#define MAX_CACHE		6
#define MAX_LINE_LEN	32

// (Real) Instructions
//		Name	Code	Ext. Code	Description
#define	NOT		"0000"				// bitwise not of $source into $dest
#define	SHL		"0001"				// a left bitwise shift, once
#define	SHR		"0010"				// a right bitwise shift, once
#define OR		"0011"				// bitwise or of $s1 and $s2 into $dest
#define	AND		"0100"				// bitwise and of $s1 and $s1 into $dest
#define	ADD		"0101"				// binary addition of $s1 and $s2 into $dest
#define	SW		"0110"				// store word to cache
#define	LW		"0111"				// load word from cache into $dest
#define	BEZ		"1000"				// branch if $source equal to zero
#define ROT1	"1001	000"		// increase cache index by one
#define	ROT		"1001	1"			// increase cache index by $source
#define	JMP		"1010"				// move memory pointer by $source
#define HALT	"1111	000"		// stops all processing
#define NOP		"1111	001"		// consumes a cycle

// String Processing Constants
#define	STR_TOK_SEP		" \n"

// Instruction Processing Return Codes
#define GARBAGE		1

struct program{
	FILE *out;
	unsigned short line_count;
	short error_code;
	char *err_str;
};

// Compilation Functions
void process_file(FILE *input, FILE *out);
void print_compiler_error(struct program *prog);

// Program Output Functions
void write_str(char *str, FILE *out);

// Instruction Processing Fucntions
short process_halt(struct program *prog);
void check_garbage(struct program *prog);

// Miscellaneous File/String Manipulators
FILE *open_write_file(const char *file);
void trimwhitespace(char *string);
void strtolower(char *str, int len);
short check_EOF(FILE *file);

#endif