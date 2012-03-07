/**
* File: translator.c
* Author: Grant Kurtz
*
* Description: Turns Hartz assembly code into a "binary executable".
*/
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "translator.h"
#include "symbols.h"
#include "idents.h"
#include "strlib.h"
#include "generrors.h"
#include "terms.h"

// Argument data
int warnings = 0;
int print_tables = 0;
int print_comp_i = 0;
int make_fast = 0;

int main(int argc, char **argv){

	// perform sanity check on arguments
	if(argc < 3 || argc > 6){
		print_help(argv[0]);
		return 1;
	}

	// process argument options
	int c = 3;
	while(c < argc){
		if(strcmp(argv[c], WARN_FLAG) == 0)
			warnings = 1;
		else if(strcmp(argv[c], SYST_FLAG) == 0)
			print_tables = 1;
		else if(strcmp(argv[c], COMP_INFO) == 0)
			print_comp_i = 1;
		else if(strcmp(argv[c], HELP_FLAG) == 0)
			print_help(argv[0]);
		else if(strcmp(argv[c], FAST_FLAG) == 0)
			make_fast = 1;
		else{
			print_asterisk(RED_C, stderr);
			fprintf(stderr, "Unknown flag '%s'.\n\n", argv[c]);
			print_help(argv[0]);
			return 1;
		}
		c++;
	}

	// try to open/create the files the user wants us to use
	FILE *input_file = fopen(argv[1], "r");
	FILE *out_file = 0;

	if(!input_file){
		print_asterisk(RED_C, stderr);
		fprintf(stderr, "Error: Unable to open '%s' for "
		"reading, exiting.\n", argv[1]);
		return 2;
	}
	out_file = open_write_file(argv[2]);
	if(!out_file){
		print_asterisk(RED_C, stderr);
		fprintf(stderr, "Error: Unable to open '%s' for writing, exiting.\n",
				argv[2]);
		return 2;
	}

	// print banner
	if(print_comp_i){
		printf( "\t\t=== Hartz Translator ===\n"
				"Machine Constraints\n"
				"\t%d Bytes of Memory\n"
				"\t%d Registers\n"
				"\t%d Bytes of Cache\n\n"
				"Compiler Constraints\n"
				"\tMax Line Length of %d Bytes\n"
				"\tMax One Instruction Per Line\n\n",
				MAX_MEMORY, MAX_REGS, MAX_CACHE, MAX_LINE_LEN);
	}

	// setup our program struct to store some data
	struct program *program = (struct program*) malloc(sizeof(struct program));
	memset(program, 0, sizeof(struct program));
	program->out = out_file;
	program->input = argv[1];
	program->in = input_file;

	if(make_fast){
		write_instruc_str(HALT, 0, 0, 0, 0, program);
		fclose(input_file);
		fclose(out_file);
		return 0;
	}

	// create the symbol table
	struct symbol_table *tbl = (struct symbol_table*) malloc(
	sizeof(struct symbol_table));
	memset(tbl, 0, sizeof(struct symbol_table));
	program->tbl = tbl;

	// Create the table for constants
	struct symbol_table *const_tbl = (struct symbol_table*) malloc(
	sizeof(struct symbol_table));
	memset(const_tbl, 0, sizeof(struct symbol_table));
	program->const_tbl = const_tbl;

	// start processing file
	process_input_program(program);
	fclose(input_file);
	fclose(out_file);

	if(print_tables){
		printf("\n");
		print_symbols(program->tbl);
		print_symbols(program->const_tbl);
		printf("\n");
	}

	if(program->error_code){
		print_asterisk(RED_C, stderr);
		fprintf(stderr, "Stopped processing because of an error.\n");
	}
	else{
		print_asterisk(GRN_C, stdout);
		printf("Done!\n");
	}
}

/**
* Opens a file for writing.
*/
FILE *open_write_file(const char *file){
	FILE *out_file = 0;
	out_file = fopen(file, "w");
	if(!out_file){
		return 0;
	}
	return out_file;
}


/**
* Given a program struct, will process the (already opened) input file and
* begin compilation line by line.
*/
void process_input_program(struct program *program){

	print_asterisk(GRN_C, stdout);
	printf("Processing File...\n");
	char *tok;
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;

	// parse input file
	do{
		program->line_count++;
		#ifdef DEBUG
		fprintf(stderr, "*** Reading Line %d...\n", program->line_count);
		#endif
		fgetpos(program->in, &program->str_line);
		tok = read_next_token(buf, program->in, MAX_LINE_LEN);

		#ifdef DEBUG
			fprintf(stderr, "Read Token '%s'\n", tok);
		#endif

		if(!tok){
			continue; // just a whitespace line, move on
		}
		else{
			process_token(tok, program);
		}

		// the instruction processor should have consumed all relevant tokens,
		// check if there is garbage at the end of the line
		if(!program->error_code)
			check_garbage(program);
	}while(!check_EOF(program->in) && !program->error_code);

	if(program->error_code)
	return;

	// resolve constants/labels
	translate_terms(program->terms, program);

	// write terms to out file
	struct Term *t = program->terms;
	write_terms(t, program);

	// process warnings
	if(warnings){
		check_warnings(program);
	}

	#ifdef DEBUG
		// why did we quit?
		if(program->error_code){
			fprintf(stderr, "Stopped processing because of an error.\n");
		}
		else if(check_EOF(program->in)){
			fprintf(stderr, "Stopped processing because EOF reached.\n");
		}
	#endif
}

/**
* Given the first token to an input line, will attempt to parse it and
* process the related instruction.
*
* @param tok 		The first token on the input line, this is expected to be
* 					a clean (non-whitespace) token.
* @param program 	Contains all current program information gathered so far,
* 					primarily used for error reporting.
*/
void process_token(char *tok, struct program *program){

	// check if we have a label
	if(check_label_def(tok, program)){
		process_label_def(tok, program);
		return;
	}

	// check for comments
	else if(check_comment(tok, program)){
		process_comment(tok, program);
		return;
	}

	// process constant definitions
	else if(check_const_def(tok, program)){
		process_const_def(tok, program);
		return;
	}

	// the first token should contain our instruction
	else if(!strcmp(tok, "HALT")){
		process_instruction(program, HALT, HALT_F , 0);
	}
	else if(!strcmp(tok, "NOT")){
		process_instruction(program, NOT, NOT_F, 0);
	}
	else if(!strcmp(tok, "SHL")){
		process_instruction(program, SHL, SHL_F, 0);
	}
	else if(!strcmp(tok, "SHR")){
		process_instruction(program, SHR, SHR_F, 0);
	}
	else if(!strcmp(tok, "OR")){
		process_instruction(program, OR, OR_F, 0);
	}
	else if(!strcmp(tok, "AND")){
		process_instruction(program, AND, AND_F, 0);
	}
	else if(!strcmp(tok, "ADD")){
		process_instruction(program, ADD, ADD_F, 0);
	}
	else if(!strcmp(tok, "SW")){
		process_instruction(program, SW, SW_F, 0);
	}
	else if(!strcmp(tok, "LW")){
		process_instruction(program, LW, LW_F, 0);
	}
	else if(!strcmp(tok, "BEZ")){
		process_instruction(program, BEZ, BEZ_F, 0);
	}
	else if(!strcmp(tok, "ROT")){
		process_instruction(program, ROT, ROT_F, 0);
	}
	else if(!strcmp(tok, "ROT1")){
		process_instruction(program, ROT1, ROT1_F, 0);
	}
	else if(!strcmp(tok, "JMP")){
		process_instruction(program, JMP, JMP_F, 0);
	}
	else if(!strcmp(tok, "NOP")){
		process_instruction(program, NOP, JMP_F, 0);
	}

	// looks like a bad opcode
	else{
		#ifdef DEBUG
			fprintf(stderr, "BAD CODE '%s'\n", tok);
		#endif
		print_unexpected_ident(tok, program);
	}
}

/**
* Will attempt to parse the rest of the input line according to the
* restrictions given by the format string. The format string is used to
* specify, in the given order, how to parse the next token. The following
* format is accepted (strictly):
*
* 	s -> Source register
* 	d -> Destination register
* 	l -> Label
* 	c -> Constant
* 	n -> Literal number
* 	[ -> Start 'or' expression (non-recursive)
* 	] -> End 'or' expression
*
* Note that the or expression works as expected, so "[lcn]" translates to
* "take whichever successfully parses first, either a label, constant, or
* a number".
*
* If the or condition fails, or any parsing fails, an error is printed
* and all processing stops (the function does however return instead of
* calling exit()).
*
* @param prog 		Contains general program information thus far gathered,
* 					and is used for handling error reporting.
* @param opcode 	The binary operation that this line started with.
* @param fmt 		The format string for what arguments to expect.
* @param misc 		Any bits that need to be added as the last child to the
* 					opcode term.
*/
void process_instruction(struct program *prog, char *opcode, const char *fmt,
		char *misc){

	// create a term for this instruction
	struct Term *t = create_term(opcode, strlen(opcode), 0);
	if(!t){
		#ifdef DEBUG
			fprintf(stderr, "whoops! Looks like a nul-pointer...\n");
		#endif
		prog->error_code = -1;
		return;
	}
	struct Term *child;
	prog->term_count++;
	t->pos = prog->term_count;

	// add term to our program
	if(prog->terms){
		prog->end_term->next_term = t;
		prog->end_term = t;
	}
	else{
		prog->terms = t;
		prog->end_term = t;
	}

	if(!*opcode)
		return;


	#ifdef DEBUG
	fprintf(stderr, "FMT CODE: '%s'.\n", fmt);
	#endif

	// opcode argument parsing
	short reg = -1;
	char *iden = 0;
	char *label = 0;
	char *tok = 0;
	int c = 0, or = 0;
	while(fmt[c]){
		if(or && ( (reg != -1) | (iden ? 1 : 0) | (label ? 1 : 0) )
				&& fmt[c] != ']'){
			#ifdef DEBUG
				fprintf(stderr, "DOING NOTHING!\n%d\n%p\n%p\n", reg, iden,
						label);
			#endif
		}
		else if(fmt[c] == '['){
		or = 1;
		}
		else if(fmt[c] == ']'){
			or = 0;
			reg = -1;
			iden = 0;
			label = 0;
			tok = 0;

			// if all options failed, report parse error
			if(!reg && !iden && !label){
				// TODO: create standardized error reporting....
				print_compiler_error(prog, RED_C);
				print_asterisk(RED_C, stderr);
				fprintf(stderr, "\tUnexpected opcode argument.\n");
				prog->error_code = GARBAGE;
				return;
			}
		}
		else if(fmt[c] == 's'){
			reg = read_src_reg(prog, or);
			if(!or && reg == -1){
				return;
			}
		}
		else if(fmt[c] == 'd'){
			reg = read_dst_reg(prog, or);
			if(!or && reg == -1){
				return;
			}
		}
		else if(fmt[c] == 'l'){
			if(!tok && or){
				tok = strtok(0, ", \t");
				label = tok;
			}
			else if(!or){
				label = strtok(0, ", \t");
			}
			else{
				label = tok;
			}
			struct Term *nt = create_term(label, strlen(label), 0);
			prog->term_count++;
			nt->pos = prog->term_count;
			prog->end_term->next_term = nt;
			prog->end_term = nt;
			#ifdef DEBUG
			fprintf(stderr, "GOTS A LABEL!\n");
			#endif
		}
		else if(fmt[c] == 'n'){
			if(!tok && or){
				tok = strtok(0, ", \t");
				iden = tok;
			}
			else if(!or){
				iden = strtok(0, ", \t");
			}
			else{
				iden = tok;
			}
		}
		else{
			fprintf(stderr, " * Error In Compiler!!!\n\tStrange format code: "
			"'%c'.\n", fmt[c]);
		}

		if(reg != -1){
			child = create_single_char_term(dtoc(reg-1), 0);
			add_child_term(child, t, prog);
		}
		else if(iden){
			child = create_term(iden, strlen(iden), 0);
			add_child_term(child, t, prog);
		}
		c++;
	}

	// Finally, add miscellaneous bits to the end
	if(misc){
		struct Term *mt = create_term(misc, strlen(misc), 0);
		add_child_term(mt, t, prog);
	}
}

/**
* Checks for any unprocessed tokens that may have been left in the buffer.
*/
void check_garbage(struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "Checking for garbage at EOL...\n");
	#endif
	char *tok;
	if((tok = strtok(0, STR_TOK_SEP))){
		if(!check_comment(tok, prog))
			print_unexpected_ident(tok, prog);
	}
}

/**
* Simply writes the given string to the given file in a standardized format.
*/
void write_str(char *str, FILE *out){
	fprintf(out, "%s\n", str);
}

/**
* Will attempt to find a source register of the given format:
* $S<x>
* Note, that x MUST be between 1 and MAX_REGS (inclusively). The token is
* rejected if all 3 characters are not of the given format.
*
* @param prog Used for error reporting
* @param suppress suppresses error messages if 1
* @return -1 on failure, otherwise the numbered register that
* was parsed.
*/
short read_src_reg(struct program *prog, short suppress){
	char *tok = read_reg(prog, suppress);
	if(!tok)
		return -1;
	short reg = 0;
	if(tok[1] != 'S' || !(reg = atoi(tok+2)) || reg > MAX_REGS ||
			reg < 1){
		print_expected_ident(tok, "$Sx", prog);
		return -1;
	}
	return reg;
}

/**
* Like the 'read_src_reg()' function, will attempt to find a destination
* register of the given format:
* $D<x>
* Note, that x MUST be between 1 and MAX_REGS (inclusively). The token is
* rejected if all 3 characters are not of the given format.
*
* @param prog 		Used for error reporting
* @param suppress 	suppresses error messages if 1
* @return 			-1 on failure, otherwise the numbered register that
* 					was parsed.
*/
short read_dst_reg(struct program *prog, short suppress){
	char *tok = read_reg(prog, suppress);
	if(!tok)
		return -1;
	short reg = 0;
	if(tok[1] != 'D' || !(reg = atoi(tok+2)) || reg > MAX_REGS ||
			reg < 1){
		print_expected_ident(tok, "$Dx", prog);
		return -1;
	}
	return reg;
}

/**
* A general purpose function for ensuring the current token is of a valid
* register type. Note, the token is rejected if the register is not exactly 3
* characters or does not start with a '$'.
*
* @param prog Used for error reporting
* @param suppress suppresses error messages if 1
* @return 0 on failure, otherwise the register string that was
* parsed.
*/
char *read_reg(struct program *prog, short suppress){
	char * tok = strtok(0, ", \t");
	trimwhitespace(tok);

	// should only be 3 characters (may change, but sufficient for now)
	if(strlen(tok) != 3 || tok[0] != '$'){
		print_expected_ident(tok, "$", prog);
		tok = 0;
	}
	return tok;
}

/**
* For more complex instructions that will come in pieces, this is useful as it
* will print the "binary" data in the order of the parameters. Note that misc
* is a "catch all" that can be used to print an arbitrary number of
* characters.
*/
void write_instruc_str(char *str, short s1, short s2, short dest, char *misc,
		struct program *prog){
	int total = 0;
	char *buf = 0;
	total = fprintf(prog->out, "%s", str);
	if(s1){
		buf = conv_reg_to_str(buf, s1);
		total += fprintf(prog->out, "%s", buf);
	}
	if(s2){
		buf = conv_reg_to_str(buf, s2);
		total += fprintf(prog->out, "%s", buf);
	}
	if(dest){
		buf = conv_reg_to_str(buf, dest);
		total += fprintf(prog->out, "%s", buf);
	}
	if(misc)
		total += fprintf(prog->out, "%s", misc);
	free(buf);
	while(total < WORD_SIZE){
		fprintf(prog->out, "%d", 0);
		total++;
	}
	fprintf(prog->out, "\n");
}

/**
* Given a base-10 number, the provided buf will have its contents filled with
* a binary string representative of the given base-10 value. Note, if the
* provided string buffer is not of the appropriate size the buffer is
* automatically freed and reallocated.
*/
char *conv_reg_to_str(char *buf, short reg){
	int char_count = log10(MAX_REGS)/log10(2);
	#ifdef DEBUG
		fprintf(stderr, "%p\n", buf);
	#endif

	// resize the buffer
	if(!buf || strlen(buf) != char_count){
		if(buf)
			free(buf);
		buf = (char*) malloc(char_count);
	}

	// all register values start at 1, so let's fix that
	reg--;
	do{
		if(reg%2)
			buf[char_count-1] = '1';
		else
			buf[char_count-1] = '0';
		reg /= 2;
		char_count--;
	}while(char_count);
	return buf;
}

/**
* Given a linked-list of terms, will resolve identifiers and translate
* remaining values into binary for printing.
*
* @param t The root term to translate
* @param prog Contains all program information for general purpose use,
* including error reporting.
*/
void translate_terms(struct Term * t, struct program *prog){

	// vars
	struct symbol *s;
	int diff;

	// consume all terms
	while(t){

		// example code for dealing with JMP instructions
		if(strcmp(t->term, JMP) == 0){

			// extract the multiplier

			// the next term specifies an r-pointer
			t = t->next_term;
			if( check_explicit_literal(t->term, prog) ){
			t->term = numtob(stonum(t->term+1), WORD_SIZE);
			}
			else{

				// We have a symbol to parse
				if( (s = find_symbol(t->term, prog->tbl)) ){

					// mark as used
					s->used = 1;

					// we have a label to resolve
					diff = s->pos - t->pos;
					if(diff < 0){
					diff = MAX_MEMORY + diff;
					}
					t->term = numtob(diff, WORD_SIZE);
				}
				else if( (s = find_symbol(t->term, prog->const_tbl)) ){

					s->used = 1;

					// looks like a constant was used
					t->term = numtob(s->val, WORD_SIZE);
				}
				else{
					// TODO: use the standard print_compiler_error message
					print_symbol_not_found(t->term, prog);
				}
			}
		}
		t = t->next_term;
	}

}

/**
* Handles the final step in compilation of writing the terms to the output
* file.
*
* @param t 			The root term to be processed.
* @param program 	Contains all general program information gathered thus far,
* 					primarily used for error reporting.
*/
void write_terms(struct Term *t, struct program *program){

	int c = 0;
	int total_bits = 0;
	static const char *filler = "0000000";
	while(t){
		#ifdef DEBUG
			fprintf(stderr, "NEXT TERM! %d\n", t->child_count);
		#endif
		total_bits = fprintf(program->out, "%s", t->term);
		while(c < t->child_count && t->child_terms[c]){
			#ifdef DEBUG
				fprintf(stderr, "CHILD TERM %p\n", t->child_terms[c]);
			#endif
			total_bits += fprintf(program->out, "%s", t->child_terms[c]->term);
			c++;
		}
		if(total_bits != WORD_SIZE){
			fprintf(program->out, "%s", (filler + total_bits) );
		}
		fprintf(program->out, "\n");
		c = 0;
		t = t->next_term;
	}
}

void check_warnings(struct program *prog){

	// check which symbols are not used, generate warnings
	struct symbol *s = prog->tbl->r;
	while(s){
		if(!s->used){
			print_symbol_not_used(s, "Label", prog);
		}
		s = s->next;
	}
	s = prog->const_tbl->r;
	while(s){
		if(!s->used){
			print_symbol_not_used(s, "Constant", prog);
		}
		s = s->next;
	}
}

/**
* Just used to print all the accepted flags and proper program calling.
* @param prog_name The name that was used to call this program.
*/
void print_help(const char *prog_name){
	printf("usage: %s <input-file> <output-file> [flags]\n"
			"Options (make separate):\n"
			" -f\tMake Code Faster (TM)\n"
			" -h\tPrint help\n"
			" -i\tPrint system information\n"
			" -s\tPrint the symbol tables\n"
			" -w\tTurn on (all) warnings\n",
			prog_name);
}

