#ifndef _STRLIB_H
#define _STRLIB_H

void trimwhitespace(char *string);
void strtoupper(char *str, int len);
short check_EOF(FILE *file);
char *read_next_token(char *buf, FILE *input, int buf_size);

#endif
