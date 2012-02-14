#ifndef symbols_h_
#define symbols_h_


struct symbol_table{
	struct symbol *r;
	struct symbol *e;
	int sym_count;
};

typedef struct symbol{
	struct symbol *next;
	char *iden;
	int val;
	short used;
}symbol;

void add_symbol(char *iden, int val, struct symbol_table *tbl);
struct symbol *find_symbol(char *iden, struct symbol_table *tbl);
void print_symbol(struct symbol *sym, int c);
void print_symbols(struct symbol_table *tbl);

#endif
