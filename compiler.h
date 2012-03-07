#ifndef compiler_h
#define compiler_h


// Machine Capabilities
#define MAX_VARS 	6
#define MAX_TEXT 	28

// Language Identifiers
#define LINE_TERM 	';'
#define IF_IDEN		'if'
#define PAREN_S		'('
#define PAREN_E		')'
#define EQUALS		'='
#define BRACE_S		'{'
#define BRACE_E		'}'
#define PLUS		'+'
#define SUB			'-'
#define HALT		'HALT'
#define NEWLINE		'\n'
#define TAB			'\t'
#define SPACE		' '

typedef struct{
	unsigned short line_count;
	unsigned short has_error;
	char *err_msg;
	FILE *input;
	FILE *output;
} ProgramData;


// Miscellaneous Functions
void 	printEqualsSeparator();
int 	checkOpenFile(const char*);
void 	getInputFile();
char 	readNonEmptyChar(FILE *, ProgramData *);
int 	consumeUntil(FILE * input, char * buf, const unsigned int buf_size, 
		const char term_char, ProgramData *prog);

// Symbol Table Manipulation Functions
/*int 	symbolExists(const char*, const SymbolTable *);
void	putSymbol(const unsigned char, const char*, const unsigned char,
			SymbolTable *);
int		tryAddSymbol(char*, const unsigned char, SymbolTable *);
*/
int		parseFile(struct program *prog);
int		getNextSymbol(char *);


// Productions
void 	ifStatement(struct Term *, FILE *, ProgramData *);

// Pre-Production Identifiers
void	process_definition(char *tok, struct program *prog);

// Error Handling
void 	reportCompilerError(char *, ProgramData *);


#endif
