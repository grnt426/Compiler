/**
 * File:		compiler.c
 * Author:		Grant Kurtz
 *
 * Description:	Processes a C-Style program and translates it into Hartz
 * 				Assembly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "compiler.h"
#include "generrors.h"
#include "strlib.h"
#include "idents.h"
#include "terms.h"

int main(){

	// Print banner
	printEqualsSeparator();
	printf("\tHartz Compiler\n");
	printEqualsSeparator();
	
	// grab file for compilation
	char file[64];
	getInputFile(file);
	printf("Processing '%s' for compilation...\n", file);
	
	// build Symbol Table
	struct symbol_table *vars;
	vars = (struct symbol_table *) malloc(sizeof(struct symbol_table));
	memset(vars, 0, sizeof(struct symbol_table));

	// Build program metadata
	struct program *prog;
	prog = (struct program *) malloc(sizeof(struct program));
	memset(prog, 0, sizeof(struct program));
	prog->input = file;
	prog->in = fopen(file, "r");
	prog->tbl = vars;

	// begin parsing file
	int ret_code = parseFile(prog);
	return ret_code;
}

void printEqualsSeparator(){
	printf("==============================\n");
}

void getInputFile(char *file){
	printf("Input File: ");
	scanf("%s[64]", file);
	while(!checkOpenFile(file)){
		printf("'%s' is not usable!\nInput File: ", file);
		scanf("%s[64]", file);
	}
}

int checkOpenFile(const char *filename){
	FILE *file;
	if( (file = fopen(filename, "r")) ){
		fclose(file);
		return 1;
	}
	return 0;
}

int parseFile(struct program *prog){

	// vars
	short chars_read;
	char *tok;

	do{
		prog->line_count++;
		#ifdef DEBUG
			fprintf(stderr, "*** Reading Line %d...\n", prog->line_count);
		#endif
		// fgetpos(prog->in, &prog->str_line);
		// tok = read_next_token(buf, prog->in, 64);
		break;
		if(!tok){
			continue;
		}
		else{
			// process_token(tok);
		}
	}while(!prog->error_code && !check_EOF(prog->in));

	return 0;
}

void process_token(char *tok, struct program *prog){
	
	if(check_comment(tok, prog)){
		process_comment(tok);
	}
	else{
		
		// must be a variable
		process_definition(tok, prog);
	}
}

void process_definition(char *tok, struct program *prog){
	
	struct symbol *s;

	// previously declared
	if( (s = find_symbol(tok, prog->tbl)) ){
		// do stuff
	}

	// new term
	else{
		s = (struct symbol *) malloc(sizeof(struct symbol));
		if(!s){
			print_memory_error(prog);
			return;
		}
		memset(s, 0, sizeof(struct symbol));
		s->iden = (char *) malloc(strlen(tok));
		strncpy(s->iden, tok, strlen(tok));
	}
	
	struct Term *t = (struct Term *) malloc(sizeof(struct Term));
	if(!t){
		print_memory_error(prog);
		return;
	}
	strncpy(t->term, tok, strlen(tok));

}

/**
 * Production:
 * 		stmt -> 'IF_IDEN' 'PAREN_S' expr 'PAREN_E' 'BRACE_S' stmt 'BRACE_E'
 */
void ifStatement(struct Term *term, FILE *input, ProgramData *prog){
	
	// vars
	char value;
	const unsigned int buf_size = 64;
	char misc_buf[buf_size];
	struct Term * new_term;
	
	// The 'IF_IDEN' terminal has already been consumed, process all else
	value = readNonEmptyChar(input, prog);
	if(value == PAREN_S){
		addChildTerm(createSingleCharTerm(PAREN_S), term);
	}
	else{
		// produce error
	}
	
	// consume the expression until the 'PAREN_E' is found
	if(consumeUntil(input, misc_buf, buf_size, PAREN_E, prog) == -1){
		// produce error
	}
	
	// produce a Term for the expression, then attempt to parse it
	addChildTerm(createTerm(misc_buf, buf_size), term);
	
	// parse statement within the if-statement
	// STUB
	
	// check parsed statement
	if(prog->has_error)
		return; // someone beneath us failed
	
	// add the closing parenthesis
	addChildTerm(createSingleCharTerm(PAREN_S), term);
	
	// next, consume the 'BRACE_S' identifier
	value = readNonEmptyChar(input, prog);
	if(value == PAREN_S){
		addChildTerm(createSingleCharTerm(BRACE_S), term);
	}
	else{
		// produce error
	}
	
	// consume the following statement
	
	
	// consume the final identifier, 'BRACE_E'
	
	
}

void reportCompilerError(char * err_msg, ProgramData * prog){
	
}

char readNonEmptyChar(FILE *input, ProgramData *prog){
	
	// vars
	char c [2];
	int chars_read;
	
	while(1){
		chars_read = fscanf(input, "%c", c);
		if(chars_read == 0){
			// error handling
		}
		if(chars_read == EOF){
			// error handling
		}
		if(*c == NEWLINE)
			prog -> line_count++;
		if(*c != TAB && *c != SPACE)
			break;
	}
	return *c;
}

int consumeUntil(FILE *input, char *buf, const unsigned int buf_size, 
		const char term_char, ProgramData *prog){
	int chars_read = 0;
	while(chars_read < buf_size){
		buf[chars_read] = readNonEmptyChar(input, prog);
		chars_read++;
		if(buf[chars_read-1] == term_char)
			return chars_read;
	}
	return -1;
}

