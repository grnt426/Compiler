#include <string.h>
#include <stdio.h>
#include "generrors.h"
#include "symbols.h"

void print_memory_error(struct program *prog){
	fprintf(stderr, "Not enough memory available to process program!\n"
			"Exiting...\n");
	prog->error_code = ALLOC_ERR;
}

void print_fault(const char* reason, struct program *prog){
	fprintf(stderr, "Whoops! The compiler has a bug! Failure Reason:\n"
			"\t%s", reason);
	prog->error_code = 4;
}

