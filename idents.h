#ifndef _IDENTS_H
#define _IDENTS_H

// Label Processing Functions
short check_label_def(char *tok, struct program *prog);
void process_label_def(char *tok, struct program *prog);

// User Defined Identifiers (UDI) Processing Functions 
short check_const_def(char *tok, struct program *prog);
void process_const_def(char *tok, struct program *prog);
int process_const(char *tok, struct program *prog);
short check_const(char *tok, struct program *prog);

// Identifier Error Reporting
void print_unexpected_ident(char *ident, struct program *prog);
void print_expected_ident(char *ident, char *expected, struct program * prog);

// Comment Processing Functions
short check_comment(char * tok, struct program *prog);
void process_comment();

// function processing functions


// Miscellaneous
void blind_consume();

#endif

