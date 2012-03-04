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

void add_symbol(char *iden, int val, struct symbol_table *tbl){
	if(!tbl)
		return;
	
	struct symbol *sym = (struct symbol *) malloc(sizeof(struct symbol));
	sym->iden = iden;
	sym->val = val;
	sym->next = 0;

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
				"Val:\t%d\n", sym->next, sym->iden, sym->val);
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

