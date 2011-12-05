#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	char * varname;
	unsigned char value;
} Symbol;

typedef struct{
	Symbol ** sym_array;
	unsigned int total_symbols;
} SymbolTable;

typedef struct{
	char term;
	struct Term ** child_terms;
} Term;

typedef struct{
	unsigned short line_count;
} ProgramData;


// Miscellaneous Functions
void 	printEqualsSeparator();
int 	checkOpenFile(const char*);
void 	getInputFile();
char 	readNonEmptyChar(FILE *, ProgramData *);


// Symbol Table Manipulation Functions
int 	symbolExists(const char*, const SymbolTable *);
void	putSymbol(const unsigned char, const char*, const unsigned char,
			SymbolTable *);
int		tryAddSymbol(char*, const unsigned char, SymbolTable *);
int		parseFile(const char * file);
int		getNextSymbol(char *);


// Productions
void 	ifStatement(Term *, FILE *, ProgramData *);


// Error Handling
void 	reportCompilerError(char *, ProgramData *);


// Term Manipulation Functions


int main(){

	// Print banner
	printEqualsSeparator();
	printf("\tHartz Compiler\n");
	printEqualsSeparator();
	
	// grab file for compilation
	char file[64];
	getInputFile(file);
	printf("Processing '%s' for compilation...\n", file);
	
	// build Symbol Table
	SymbolTable * sym_tbl;
	sym_tbl = (SymbolTable *) calloc(1, sizeof(SymbolTable));
	sym_tbl -> sym_array = (Symbol **) calloc(10, sizeof(Symbol *));
	
	// begin parsing file
	int ret_code = parseFile(file);
	return ret_code;
}

void printEqualsSeparator(){
	printf("==============================\n");
}

void getInputFile(char * file){
	printf("Input File: ");
	scanf("%s[64]", file);
	while(!checkOpenFile(file)){
		printf("'%s' is not usable!\nInput File: ", file);
		scanf("%s[64]", file);
	}
}

int checkOpenFile(const char* filename){
	FILE * file;
	if(file = fopen(filename, "r")){
		fclose(file);
		return 1;
	}
	return 0;
}

int symbolExists(const char * symbol, const SymbolTable * sym_tbl){
	for(int i = 0; i < MAX_VARS; ++i){
		if(strcmp(symbol, sym_tbl -> sym_array[i] -> varname) == 0)
			return 1;
	}
	return 0;
}

int tryAddSymbol(char* varname, const unsigned char value, 
		SymbolTable * sym_tbl){
		
	// vars
	Symbol * symbol;
		
	if(sym_tbl -> total_symbols == MAX_VARS)
		return 0;
	else{
		for(int i = 0; i < MAX_VARS; i++){
			symbol = sym_tbl -> sym_array[i];
			if(symbol -> varname == 0){
				symbol -> varname = varname;
				symbol -> value = value;
			}
		}
		sym_tbl -> total_symbols++;
		return 1;
	}
}

int parseFile(const char * filename){
	FILE * fileh = fopen(filename, "r");
	if(!fileh){
		fprintf(stderr, "Unable to open file '%s'...what happened?", filename);
		return -1;
	}
	
	// vars
	char identifier[64];
	short chars_read;
	unsigned int lines_read = 0;
	
	// begin the look-ahead parser loop
	while(chars_read = getNextSymbol(identifier)){
		
		// we were unable to find an identifier
		if(chars_read == -1){
			fprintf(stderr, "Unable to find a useful identifier, line %d!", 
					lines_read);
			return -1;
		}
		else if(chars_read == 0){
			fprintf(stderr, "No characters read, empty file?");
			return -1;
		}
		
		// check to see what symbols we have
		
	}
}

int getNextSymbol(char * buffer){

}

/**
 * Production:
 * 		stmt -> 'IF_IDEN' 'PAREN_S' expr 'PAREN_E' 'BRACE_S' 
			stmt 'BRACE_E'
 */
void ifStatement(Term * term, FILE * input, ProgramData * prog){
	
	// vars
	char value;
	char misc[64];
	Term * new_term;
	
	// The 'IF_IDEN' terminal has already been consumed, process all else
	value = readNonEmptyChar(input, prog);
	if(value == PAREN_S){
		new_term = (Term *) calloc(1, sizeof(Term));
		
	}
}

void reportCompilerError(char * err_msg, ProgramData * prog){
	
}

char readNonEmptyChar(FILE * input, ProgramData * prog){
	
	// vars
	char c [2];
	int chars_read;
	
	while(1){
		chars_read = fscanf(input, "%c", c);
		if(chars_read == 0){
			// error handling
		}
		if(chars_read == EOF){
			// error handling
		}
		if(*c == NEWLINE)
			prog -> line_count++;
		if(*c != TAB && *c != SPACE)
			break;
	}
	return *c;
}