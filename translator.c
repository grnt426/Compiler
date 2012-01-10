/**
 * File:		translator.c
 * Author:		Grant Kurtz
 *
 * Description:	Turns Hartz assembly code into a "binary executable".
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Constraints
#define MAX_REGS		2
#define REG_ZERO		"$0"
#define REG_ONE			"$1"
#define MAX_MEMORY		28
#define MAX_CACHE		6
#define MAX_LINE_LEN	32

void process_file(FILE *input, FILE *out);
FILE *open_write_file(const char *file);

int main(int argc, char **argv){
	
	// perorm sanity check on arguments
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
			"\tMax Line Length of %d Bytes\n", 
			MAX_MEMORY, MAX_REGS, MAX_CACHE, MAX_LINE_LEN);
	
	// start processing file
	process_file(input_file, out_file);
	
	printf("Done.\n");
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
	
	
}