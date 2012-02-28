#include <string.h>
#include <stdio.h>
#include "generrors.h"
#include "symbols.h"

void print_memory_error(struct program *prog){
	fprintf(stderr, "Not enough memory available to process program!\n"
			"Exiting...\n");
	prog->error_code = ALLOC_ERR;
}

