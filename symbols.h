#ifndef SYMBOLS_H_
#define SYMBOLS_H

// Error Codes
#define BAD_SYM		40	

// Type values
#define CONST_TYPE	0
#define LABEL_TYPE	1
#define FUNC_TYPE	2

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
	int pos;
	short used;
	short type;
}symbol;

struct program{
	FILE *out;
	FILE *in;
	char *input;
	fpos_t str_line;
	unsigned short line_count;
	unsigned int term_count;
	short error_code;
	char *err_str;
	struct symbol_table *tbl;
	struct symbol_table *const_tbl;
	struct Term * terms;
	struct Term *end_term;
};

// Symbol manipulation
void add_symbol(char *iden, int val, struct symbol_table *tbl, int pos, 
		short type);

// Symbol searching
struct symbol *find_symbol(char *iden, struct symbol_table *tbl);
struct symbol *find_symbol_at(int pos, struct symbol_table *tbl);

// Printing of symbols
void print_symbol(struct symbol *sym, int c);
void print_symbols(struct symbol_table *tbl);

// Error handling
void print_symbol_not_found(const char *bad_sym, struct program *prog);
void print_symbol_not_used(const struct symbol *sym, const char *sym_type, 
		const struct program *prog);
void print_non_func_call(const struct symbol *sym, struct program *prog,
		int err_line);
void print_return_from_non_func(int abs_pos, struct program *prog);

#endif
