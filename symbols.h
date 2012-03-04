#ifndef SYMBOLS_H_
#define SYMBOLS_H

struct Term;

struct symbol_table{
	struct symbol *r;
	struct symbol *e;
	int sym_count;
};

typedef struct symbol{
	struct symbol *next;
	char *iden;
	struct Term * term;
	int val;
}symbol;

struct program{
	FILE *out;
	FILE *in;
	char *input;
	fpos_t str_line;
	unsigned short line_count;
	short error_code;
	char *err_str;
	struct symbol_table *tbl;
	struct symbol_table *const_tbl;
	struct Term * terms;
};

void add_symbol(char *iden, int val, struct symbol_table *tbl);
struct symbol *find_symbol(char *iden, struct symbol_table *tbl);
void print_symbol(struct symbol *sym, int c);
void print_symbols(struct symbol_table *tbl);

#endif
