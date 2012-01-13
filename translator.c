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

FILE *open_write_file(const char *file){
	FILE *out_file = 0;
	out_file = fopen(file, "w");
	if(!out_file){
		fprintf(stderr, "Error: Unable to open '%s' for writing, "
				"exiting.\n", file);
	}
	return out_file;
}

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
		short ret = fgetpos(program->in, &program->str_line);
		#ifdef DEBUG
			fprintf(stderr, "LINE POS: '%p' RET VAL: '%d'\n", 
					&(program->str_line), ret);
		#endif
		tok = read_next_token(buf, program->in, MAX_LINE_LEN);
		
		#ifdef DEBUG
			fprintf(stderr, "Read Token '%s' '%d'\n", tok, strcmp(tok, "NOT"));
		#endif
		
		if(!tok){
			continue;	// just a whitespace line, move on
		}
		
		// the first token should contain our instruction
		if(!strcmp(tok, "HALT")){
			process_halt(program);
		}
		else if(!strcmp(tok, "NOT")){
			process_not(program);
		}
		
		// looks like a bad opcode
		else{
			#ifdef DEBUG
				fprintf(stderr, "BAD CODE '%s'\n", tok);
			#endif
			print_unexpected_ident(tok, program);
		}
		
		// the instruction processor should have consumed all relevant tokens,
		// check if there is garbage at the end of the line
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

void process_halt(struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "HALT instruction read...\n");
	#endif
	write_str(HALT, prog->out);
}

void process_not(struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "NOT instruction read...\n");
	#endif
	short src = read_src_reg(prog);
	short dst = read_dst_reg(prog);
	write_instruc_str(NOT, src, 0, dst, "", prog);
}

void check_garbage(struct program *prog){
	#ifdef DEBUG
		fprintf(stderr, "Checking for garbage at EOL...\n");
	#endif
	char *tok;
	if(tok = strtok(0, STR_TOK_SEP)){
		print_unexpected_ident(tok, prog);
	}
}

void write_str(char *str, FILE *out){
	fputs(str, out);
	fputs("\n", out);
}

void print_compiler_error(struct program *prog){
	fprintf(stderr, "%s: %d:\n", prog->input, prog->line_count);
	fsetpos(prog->in, &prog->str_line);
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;
	fgets(buf, MAX_LINE_LEN, prog->in);
	trimwhitespace(buf);
	fprintf(stderr, "\t'%s'\n", buf);
}

void print_unexpected_ident(char *ident, struct program *prog){
	print_compiler_error(prog);
	fprintf(stderr, "\tUnexpected Identifier '%s'.\n", ident);
	prog->error_code = GARBAGE;
}

void print_expected_ident(char *ident, char *expected, struct program *prog){
	print_compiler_error(prog);
	fprintf(stderr, "Expected '%s' but found '%s'.\n", expected, ident);
	prog->error_code = GARBAGE;
}

short check_EOF(FILE *file){
	int c = fgetc(file);
	if(feof(file))
		return 1;
	ungetc(c, file);
	return 0;
}

short read_src_reg(struct program *prog){
	char *tok = read_reg(prog);
	short reg = 0;
	if(tok[1] != 'S' || !(reg = atoi(tok+2)) || reg > MAX_REGS || 
			reg < 1){
		print_expected_ident(tok, "$Sx", prog);
	}
	return reg;
}

short read_dst_reg(struct program *prog){
	return 1;
}

char *read_reg(struct program *prog){
	char * tok = strtok(0, ", ");
	
	// should only be 3 characters (may change, but sufficient for now)
	if(strlen(tok) != 3 || tok[0] != '$'){
		print_expected_ident(tok, "$", prog);
		tok = 0;
	}
	return tok;
}

void write_instruc_str(char *str, short s1, short s2, short dest, char *misc,
		struct program *prog){
	
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

void strtoupper(char *str, int len){
	while(len){
		str[len-1] = toupper(str[len-1]);
		len--;
	}
}

char *read_next_token(char *buf, FILE *input, int buf_size){
	fgets(buf, buf_size, input);
	trimwhitespace(buf);
	strtoupper(buf, strlen(buf));
	return strtok(buf, STR_TOK_SEP);
}
