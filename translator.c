/**
 * File:		translator.c
 * Author:		Grant Kurtz
 *
 * Description:	Turns Hartz assembly code into a "binary executable".
 */
 
#include "translator.h"

int main(int argc, char **argv){
	
	// perform sanity check on arguments
	if(argc < 2 || argc > 3){
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
	if(argc == 3)
		out_file = open_write_file(argv[2]);
	else
		out_file = open_write_file(strcat(argv[1], ".out"));
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
	
	// start processing file
	process_file(input_file, out_file);
	
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

void process_file(FILE *input, FILE *out){
	
	printf("\nProcessing File...\n");
	
	char buf[MAX_LINE_LEN+1];
	buf[MAX_LINE_LEN] = 0;
	char *tok;
	struct program *program = (struct program*) malloc(sizeof(struct program));
	memset(program, 0, sizeof(struct program));
	program->line_count = 0;
	program->out = out;
	
	do{
		program->line_count++;
		#ifdef DEBUG
			fprintf(stderr, "\tReading Line %d...\n", program->line_count);
		#endif
		
		fgets(buf, MAX_LINE_LEN, input);
		trimwhitespace(buf);
		tok = strtok(buf, STR_TOK_SEP);
		strtolower(tok, strlen(tok));
		#ifdef DEBUG
			printf("Read Token '%s'\n", tok);
		#endif
		
		// the first token should contain our instruction
		if(!strcmp(tok, "HALT")){
			process_halt(program);
		}
		
		// first, see if the instruction generated any errors
		if(program->error_code == GARBAGE){
			// something bad?
		}
		
		// the instruction processor should have consumed all relevant tokens,
		// check if there is garbage at the end of the line
		check_garbage(program);
		
	}while(!check_EOF(input) && !program->error_code);
	
	#ifdef DEBUG
		// why did we quit?
		if(program->error_code){
			fprintf(stderr, "Stopped processing because of an error.\n");
		}
		else if(check_EOF(input)){
			fprintf(stderr, "Stopped processing because EOF reached.\n");
		}
	#endif
}

short process_halt(struct program *prog){
	#ifdef DEBUG
		printf("HALT instruction read...\n");
	#endif
	write_str(HALT, prog->out);
}

void check_garbage(struct program *prog){
	char *tok;
	if(tok = strtok(0, STR_TOK_SEP)){
		print_compiler_error(prog);
		fprintf(stderr, "Unexpected Identifier '%s'.\n", tok, 
			prog->line_count);
		prog->error_code = GARBAGE;
	}
}

void write_str(char *str, FILE *out){
	fputs(str, out);
	fputs("\n", out);
}

void print_compiler_error(struct program *prog){
	fprintf(stderr, "Error (line %d): ", prog->line_count);
}

// void print_unexpected(char *token

short check_EOF(FILE *file){
	int c = fgetc(file);
	if(feof(file))
		return 1;
	ungetc(c, file);
	return 0;
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

void strtolower(char *str, int len){
	static int diff = 'a' - 'A';
	while(len){
		tolower(str[len-1]);
		len--;
	}
}
