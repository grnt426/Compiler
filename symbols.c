/*
 * File:		symbols.c
 * Author:		Grant Kurtz
 *
 * Description:	Symbol manipulation...I'll come up with a better description
 * 				when it's written.
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"
#include "idents.h"
#include "strlib.h"

void add_symbol(char *iden, int val, struct symbol_table *tbl, int pos){
	if(!tbl)
		return;
	
	struct symbol *sym = (struct symbol *) malloc(sizeof(struct symbol));
	sym->iden = iden;
	sym->val = val;
	sym->next = 0;
	sym->pos = pos;
	sym->used = 0;


	if(tbl->r){
		tbl->e->next = sym;
		tbl->e = sym;
	}
	else{
		tbl->r = sym;
		tbl->e = sym;
	}
}

struct symbol *find_symbol(char *iden, struct symbol_table *tbl){
	if(!iden)
		return 0;
	if(!tbl)
		return 0;
	
	struct symbol *sym = tbl->r;
	while(sym && strcmp(sym->iden, iden)){
		sym = sym->next;
	}
	return sym;
}

void print_symbol(struct symbol *sym, int c){

	if(c > -1)
		printf(	"\t== Symbol %d ==\n", c);
	else
		printf(	"\t== Symbol ==\n");

	if(!sym){
		printf("** NULL **\n");
	}
	else{
		printf(	"Next:\t%p\n"
				"Iden:\t%s\n"
				"Val:\t%d\n"
				"Used:\t%d\n", sym->next, sym->iden, sym->val, sym->used);
	}
}

void print_symbols(struct symbol_table *tbl){
	
	int i = 0;

	printf(	"\t\t==== Symbol Table ====\n");
	if(!tbl){
		printf("** NULL **\n");
	}
	else{
		struct symbol *sym = tbl->r;
		while(sym){
			print_symbol(sym, i++);
			sym = sym->next;
		}
	}
}

void print_symbol_not_found(const char *bad_sym, struct program *prog){
	print_asterisk(RED_C, stderr);
	fprintf(stderr, "%s:\n", prog->input);
	print_asterisk(RED_C, stderr);
	fprintf(stderr, "\tUnknown Symbol '%s'.\n", bad_sym);
	prog->error_code = -1; // TODO: create actual error_code 

}


void print_symbol_not_used(const struct symbol *sym, const char *sym_type, 
		const struct program *prog){
	print_asterisk(YLW_C, stderr);
	fprintf(stderr, "%s, %d:\n", prog->input, sym->pos);
	print_asterisk(YLW_C, stderr);
	fprintf(stderr, "\tWarning: %s '%s' is not used.\n", sym_type, sym->iden);
}

