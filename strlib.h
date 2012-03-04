#ifndef _STRLIB_H
#define _STRLIB_H

// String Processing Constants
#define	STR_TOK_SEP		" \t\n"

void trimwhitespace(char *string);
void strtoupper(char *str, int len);
short check_EOF(FILE *file);
char *read_next_token(char *buf, FILE *input, int buf_size);
char dtoc(const int d);
char *numtos(const int num);
int numd(int num);

#endif
