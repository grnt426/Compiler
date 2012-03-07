/**
 * File:		idents.c
 * Author:		Grant Kurtz
 *
 * Description:	A library for hosting common identifier processing functions.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "idents.h"
#include "generrors.h"
#include "strlib.h"
#include "symbols.h"

short check_label_def(char *tok, struct program *prog){
	if(tok[strlen(tok)-1] == LABEL_SYM)
		return 1;
	return 0;
}

void process_comment(){
	#ifdef DEBUG
		// fprintf(stderr, "Ignoring comment on line %d\n", 
				// prog->line_count);
	#endif
		
	// Just eat the rest of the string
	blind_consume();
}

void process_label_def(char *tok, struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "Procesing label on line %d\n",
				prog->line_count);
	#endif

	if(tok[0] == LABEL_SYM){
		print_compiler_error(prog, RED_C);
		fprintf(stderr, "Label definition is empty!\n");
		prog->error_code = SYM_ERR;
	}
	else{
		char *iden = (char *) malloc(sizeof(tok));
		strcpy(iden, tok);
		iden[strlen(iden)-1] = '\0';
		if(find_symbol(iden, prog->tbl)){
			print_compiler_error(prog, RED_C);
			fprintf(stderr, "Doubly defined label!\n");
		}
		else{
			add_symbol(iden, prog->line_count, prog->tbl, prog->term_count);
		}
	}
}

short check_const_def(char *tok, struct program *prog){
	if(tok[0] == CONSTANT_SYM)
		return 1;
	return 0;
}

void process_const_def(char *tok, struct program *prog){
	if(!tok[1]){
		print_compiler_error(prog, RED_C);
		fprintf(stderr, "Defined constant is empty!\n");
	}
	else{
		char *iden = (char *) malloc(sizeof(tok)-1);
		strcpy(iden, tok+1);
		if(find_symbol(iden, prog->const_tbl)){
			print_compiler_error(prog, RED_C);
			fprintf(stderr, "Doubly defined constant!\n");
		}
		else{
			tok = strtok(0, STR_TOK_SEP);
			if(!tok){
				print_compiler_error(prog, RED_C);
				fprintf(stderr, "Newly defined constant has no value!\n");
			}
			else{
				// TODO: CHECK VALUE OF ATOI() call
				add_symbol(iden, atoi(tok), prog->const_tbl, prog->line_count);
			}
		}
	}
}

int process_const(char *tok, struct program *prog){
	return find_symbol(tok, prog->const_tbl)->val;
}

short check_const(char *tok, struct program *prog){
	if(find_symbol(tok, prog->const_tbl))
		return 1;
	return 0;
}

short check_explicit_literal(char *tok, struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "CHECKING TOK: '%s'\n", tok);
	#endif
	if(tok[0] == LITERAL_SYM)
		return 1;
	return 0;
}

int process_literal(char *tok, int max_val){
	int val = stonum(tok+1);
	return val > max_val ? -1 : val;
}

void blind_consume(){
	while(strtok(0, STR_TOK_SEP));
}

short check_comment(char * tok, struct program * prog){
	if(tok[0] == COMMENT_SYM)
		return 1;
	return 0;
}

/**
 * Sets up a standard compiler error reporting message to indicate the file of
 * failure and the line the failure occured on.
 */
void print_compiler_error(struct program *prog, const char *color){
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;
	if(color)
		print_asterisk(color, stderr);
	fprintf(stderr, "%s, %d:\n", prog->input, prog->line_count);
	
	// Go back to start of line to show the user where the error occurred.
	fsetpos(prog->in, &prog->str_line);
	fgets(buf, MAX_LINE_LEN, prog->in);
	trimwhitespace(buf);
	if(color)
		print_asterisk(color, stderr);
	fprintf(stderr, "\t'%s'\n", buf);
}

/**
 * Used for reporting that an unexpected token, where one may have not been 
 * anticipated, that wasnot supposed to exist.
 */
void print_unexpected_ident(char *ident, struct program *prog){
	print_compiler_error(prog, RED_C);
	fprintf(stderr, "\tUnexpected Identifier '%s'.\n", ident);
	prog->error_code = GARBAGE;
}

/**
 * Used for reporting instances where a token was being processed, but an 
 * unexpected character was read where another was anticipated.
 */
void print_expected_ident(char *ident, char *expected, struct program *prog){
	print_compiler_error(prog, RED_C);
	fprintf(stderr, "\tExpected '%s' but found '%s'.\n", expected, ident);
	prog->error_code = GARBAGE;
}

void print_asterisk(const char *color, FILE *out){
	fprintf(out, "%s * " RST_C, color);
}

