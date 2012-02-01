/**
 * File:		translator.c
 * Author:		Grant Kurtz
 *
 * Description:	Turns Hartz assembly code into a "binary executable".
 */
 
#include "translator.h"

int main(int argc, char **argv){
	
	// perform sanity check on arguments
	if(argc < 3 || argc > 4){
		fprintf(stderr, "Usage: translator <input_file> [target_exe]\n");
		return 1;
	}
	
	// try to open/create the files the user wants us to use
	FILE *input_file = fopen(argv[1], "r");
	FILE *out_file = 0;
	
	if(!input_file){
		fprintf(stderr, "Error: Unable to open '%s' for reading, exiting.\n",
				argv[1]);
		return 2;
	}
	out_file = open_write_file(argv[2]);
	if(!out_file)
		return 2;
	
	// print banner
	printf(	"\t\t=== Hartz Translator ===\n"
			"Machine Constraints\n"
			"\t%d Bytes of Memory\n"
			"\t%d Registers\n"
			"\t%d Bytes of Cache\n\n"
			"Compiler Contraints\n"
			"\tMax Line Length of %d Bytes\n"
			"\tMax One Instruction Per Line\n",
			MAX_MEMORY, MAX_REGS, MAX_CACHE, MAX_LINE_LEN);
	
	// setup our program struct to store some data
	struct program *program = (struct program*) malloc(sizeof(struct program));
	memset(program, 0, sizeof(struct program));
	program->line_count = 0;
	program->out = out_file;
	program->input = argv[1];
	program->in = input_file;
	
	// start processing file
	process_input_program(program);
	fclose(input_file);
	fclose(out_file);
	
	printf("\nDone.\n");
}

/**
 * Opens a file for writing.
 */
FILE *open_write_file(const char *file){
	FILE *out_file = 0;
	out_file = fopen(file, "w");
	if(!out_file){
		fprintf(stderr, "Error: Unable to open '%s' for writing, "
				"exiting.\n", file);
	}
	return out_file;
}


/**
 * Given a program struct, will process the (already opened) input file and
 * begin compilation line by line.
 */
void process_input_program(struct program *program){
	
	printf("\nProcessing File...\n");
	char *tok;
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;
	
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
			continue;	// just a whitespace line, move on
		}
		else{
			process_token(tok, program);
		}
		
		// the instruction processor should have consumed all relevant tokens,
		// check if there is garbage at the end of the line
		if(!program->error_code)
			check_garbage(program);
	}while(!check_EOF(program->in) && !program->error_code);
	
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
 *
 */
void process_token(char *tok, struct program *program){

	// check if we have a label

	

	// check for comments
	if(check_comment(tok, program))
		return;
	
	// the first token should contain our instruction
	if(!strcmp(tok, "HALT")){
		process_instruction(program, HALT, 0, 0, "");
	}
	else if(!strcmp(tok, "NOT")){
		process_instruction(program, NOT, 1, 1, "");
	}
	else if(!strcmp(tok, "SHL")){
		process_instruction(program, SHL, 1, 1, "");
	}
	else if(!strcmp(tok, "SHR")){
		process_instruction(program, SHR, 1, 1, "");
	}
	else if(!strcmp(tok, "OR")){
		process_instruction(program, OR, 2, 1, "");
	}
	else if(!strcmp(tok, "AND")){
		process_instruction(program, AND, 2, 1, "");
	}
	else if(!strcmp(tok, "ADD")){
		process_instruction(program, ADD, 2, 1, "");
	}
	else if(!strcmp(tok, "SW")){
		process_instruction(program, SW, 1, 0, "");
	}
	else if(!strcmp(tok, "LW")){
		process_instruction(program, LW, 0, 1, "");
	}
	else if(!strcmp(tok, "BEZ")){
		process_instruction(program, BEZ, 1, 0, "");
	}
	else if(!strcmp(tok, "ROT")){
		process_instruction(program, ROT, 1, 0, "");
	}
	else if(!strcmp(tok, "ROT1")){
		process_instruction(program, ROT1, 0, 0, "");
	}
	else if(!strcmp(tok, "JMP")){
		process_instruction(program, JMP, 1, 0, "");
	}
	else if(!strcmp(tok, "NOP")){
		process_instruction(program, NOP, 0, 0, "");
	}
	
	// looks like a bad opcode
	else{
		#ifdef DEBUG
			fprintf(stderr, "BAD CODE '%s'\n", tok);
		#endif
		print_unexpected_ident(tok, program);
	}
}

void process_instruction(struct program *prog, char *opcode, short src_count, 
		short dest_count, char *misc){
	short src = 0, src2 = 0, dest = 0;
	if(src_count){
		src = read_src_reg(prog);
		if(!src)
			return;
	}
	if(src_count == 2){
		src2 = read_src_reg(prog);
		if(!src2)
			return;
	}
	if(dest_count){
		dest = read_dst_reg(prog);
		if(!dest)
			return;
	}
	write_instruc_str(opcode, src, src2, dest, misc, prog);
}

/**
 * Checks for any unprocessed tokens that may have been left in the buffer.
 */
void check_garbage(struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "Checking for garbage at EOL...\n");
	#endif
	char *tok;
	if(tok = strtok(0, STR_TOK_SEP)){
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
 * Sets up a standard compiler error reporting message to indicate the file of
 * failure and the line the failure occured on.
 */
void print_compiler_error(struct program *prog){
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;
	fprintf(stderr, "%s: %d:\n", prog->input, prog->line_count);
	
	// Go back to start of line to show the user where the error occured.
	fsetpos(prog->in, &prog->str_line);
	fgets(buf, MAX_LINE_LEN, prog->in);
	trimwhitespace(buf);
	fprintf(stderr, "\t'%s'\n", buf);
}

/**
 * Used for reporting that an unexpected token, where one may have not been 
 * anticipated, that wasnot supposed to exist.
 */
void print_unexpected_ident(char *ident, struct program *prog){
	print_compiler_error(prog);
	fprintf(stderr, "\tUnexpected Identifier '%s'.\n", ident);
	prog->error_code = GARBAGE;
}

/**
 * Used for reporting instances where a token was being processed, but an 
 * unexpected character was read where another was anticipated.
 */
void print_expected_ident(char *ident, char *expected, struct program *prog){
	print_compiler_error(prog);
	fprintf(stderr, "\tExpected '%s' but found '%s'.\n", expected, ident);
	prog->error_code = GARBAGE;
}

/**
 * A convenience function for finding the end of a given file.
 */
short check_EOF(FILE *file){
	int c = fgetc(file);
	if(feof(file))
		return 1;
	ungetc(c, file);
	return 0;
}

/**
 * Will attempt to find a source register of the given format:
 *		$S<x>
 * Note, that x MUST be between 1 and MAX_REGS (inclusively). The token is 
 * rejected if all 3 characters are not of the given format.
 */
short read_src_reg(struct program *prog){
	char *tok = read_reg(prog);
	short reg = 0;
	if(tok[1] != 'S' || !(reg = atoi(tok+2)) || reg > MAX_REGS || 
			reg < 1){
		print_expected_ident(tok, "$Sx", prog);
	}
	return reg;
}

/**
 * Like the 'read_src_reg()' function, will attempt to find a destination 
 * register of the given format:
 *		$D<x>
 * Note, that x MUST be between 1 and MAX_REGS (inclusively). The token is
 * rejected if all 3 characters are not of the given format.
 */
short read_dst_reg(struct program *prog){
	char *tok = read_reg(prog);
	short reg = 0;
	if(tok[1] != 'D' || !(reg = atoi(tok+2)) || reg > MAX_REGS || 
			reg < 1){
		print_expected_ident(tok, "$Dx", prog);
	}
	return reg;
}

/**
 * A general purpose function for ensuring the current token is of a valid 
 * register type. Note, the token is rejected if the register is not exactly 3
 * characters or does not start with a '$'.
 */
char *read_reg(struct program *prog){
	char * tok = strtok(0, ", ");
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
 * will print the "binary" data in the order of the parameters.  Note that misc
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
		char_count--;
	}while(char_count);
	return buf;
}

/**
 * Removes all trailing and leading whitespace, but will leave whitespace
 * inbetween valid characters.  Note, this function will move the resulting
 * cstring to the start of the char array so that the originally provided
 * pointer can be freed.
 * Source: 	http://stackoverflow.com/a/123724
 * 
 * @param	s	The cstring to be stripped.
 */
void trimwhitespace(char *s){
	char * p = s;
	int l = strlen(p);
	while(isspace(p[l - 1])) p[--l] = 0;
	while(* p && isspace(* p)) ++p, --l;
	memmove(s, p, l + 1);
}

/**
 * A convenience function for turning an entire string into uppercase.
 */
void strtoupper(char *str, int len){
	while(len){
		str[len-1] = toupper(str[len-1]);
		len--;
	}
}

/**
 * Will process the first token of the given input buffer (or return the next
 * token of the previously provided buffer if buf is 0), trim whitespace, 
 * convert the entire buffer to uppercase, and then return the token.
 */
char *read_next_token(char *buf, FILE *input, int buf_size){
	fgets(buf, buf_size, input);
	trimwhitespace(buf);
	strtoupper(buf, strlen(buf));
	return strtok(buf, STR_TOK_SEP);
}

short check_comment(char * tok, struct program * prog){
	if(tok[0] == '#'){
		#ifdef DEBUG
			fprintf(stderr, "Ignoring comment on line %d\n", 
					prog->line_count);
		#endif
		
		// Just eat the rest of the string
		blind_consume();
		return 1;
	}
	return 0;
}

void blind_consume(){
	while(strtok(0, STR_TOK_SEP));
}

