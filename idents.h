#ifndef _IDENTS_H
#define _IDENTS_H

// Special Symbols
#define COMMENT_SYM		'*'
#define LABEL_SYM		':'
#define CONSTANT_SYM	'#'
#define LITERAL_SYM		'!'
#define FUNC_DEF		'.'

// Soft Contraints
#define MAX_LINE_LEN	32

// Error Reporting
#define	DOUBLE_DEF		30
#define	NO_DEF_VAL		31
#define	EMPTY_DEF		32
#define FUNC_DOUBLE		33
#define	CALL_NON_F		34
#define RET_NON_F		35
#define LIT_TOO_BIG		36
#define UNEXPECTED		37

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
int process_literal(char *tok, int max_val);

// Comment Processing Functions
short check_comment(char * tok, struct program *prog);
void process_comment(struct program *prog);

// Error Reporting
void print_compiler_error(struct program *prog, const char *color);
void print_asterisk(const char *color, FILE *out);
void print_expected_const(char *iden, struct program *prog);
void print_expected_literal(char *iden, struct program *prog);
void print_literal_too_large(char *iden, struct program *prog);

// Identifier Error Reporting
void print_unexpected_ident(char *ident, struct program *prog);
void print_expected_ident(char *ident, char *expected, struct program * prog);

// Function Definition Processing
short check_func_def(char *tok, struct program *prog);
void process_func_def(char *tok, struct program *prog);

// Miscellaneous
void blind_consume();

#endif

