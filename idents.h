#ifndef _IDENTS_H
#define _IDENTS_H

// Special Symbols
#define COMMENT_SYM		'*'
#define LABEL_SYM		':'
#define CONSTANT_SYM	'#'
#define LITERAL_SYM		'!'

// Soft Contraints
#define MAX_LINE_LEN	32

struct program;

// Label Processing Functions
short check_label_def(char *tok, struct program *prog);
void process_label_def(char *tok, struct program *prog);

// User Defined Identifiers (UDI) Processing Functions 
short check_const_def(char *tok, struct program *prog);
void process_const_def(char *tok, struct program *prog);
int process_const(char *tok, struct program *prog);
short check_const(char *tok, struct program *prog);

// literals
short check_explicit_literal(char *tok, struct program *prog);

// Comment Processing Functions
short check_comment(char * tok, struct program *prog);
void process_comment();

// Error Reporting
void print_compiler_error(struct program *prog);
void print_asterisk(const char *color, FILE *out);

// Identifier Error Reporting
void print_unexpected_ident(char *ident, struct program *prog);
void print_expected_ident(char *ident, char *expected, struct program * prog);

// Miscellaneous
void blind_consume();

#endif

