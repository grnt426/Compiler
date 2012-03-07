#ifndef _STRLIB_H
#define _STRLIB_H

// Text Coloring
#define RST_C	"\e[m"
#define BLK_C	"\033[22;30m"
#define RED_C	"\033[22;31m"
#define GRN_C	"\033[22;32m"
#define YLW_C	"\033[22;33m"
#define BLU_C	"\033[22;34m"
#define MAG_C	"\033[22;35m"
#define CYN_C	"\033[22;36m"
#define WHT_C	"\033[22;37m"

// String Processing Constants
#define	STR_TOK_SEP		" \t\n"

// String processing functions
void trimwhitespace(char *string);
void strtoupper(char *str, int len);
int letter_count(const char *str);

// File processing functions
short check_EOF(FILE *file);

// Number To String/Char conversion functions
char dtoc(const int d);
char *numtos(const int num);
char *numtob(int num, int min_size);

// String/Char to Number conversion functions
int stonum(char *s);
int ctod(char c);

// Number functions
int numd(int num);

// Misc.
char *read_next_token(char *buf, FILE *input, int buf_size);

#endif
