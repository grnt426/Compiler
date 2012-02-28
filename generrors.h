#ifndef _GENERRORS_H_
#define _GENERRORS_H_

// Instruction Processing Return Codes
#define GARBAGE		1
#define SYM_ERR		2
#define ALLOC_ERR	3

struct program;

void print_memory_error(struct program *prog);

#endif

