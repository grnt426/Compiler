#include "compiler.h"

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
	SymbolTable *sym_tbl;
	sym_tbl = (SymbolTable *) calloc(1, sizeof(SymbolTable));
	sym_tbl -> sym_array = (Symbol **) calloc(10, sizeof(Symbol *));
	
	// begin parsing file
	int ret_code = parseFile(file);
	return ret_code;
}

void printEqualsSeparator(){
	printf("==============================\n");
}

void getInputFile(char *file){
	printf("Input File: ");
	scanf("%s[64]", file);
	while(!checkOpenFile(file)){
		printf("'%s' is not usable!\nInput File: ", file);
		scanf("%s[64]", file);
	}
}

int checkOpenFile(const char *filename){
	FILE *file;
	if(file = fopen(filename, "r")){
		fclose(file);
		return 1;
	}
	return 0;
	presents
}

int symbolExists(const char *symbol, const SymbolTable *sym_tbl){
	for(int i = 0; i < MAX_VARS; ++i){
		if(strcmp(symbol, sym_tbl -> sym_array[i] -> varname) == 0)
			return 1;
	}
	return 0;
}

int tryAddSymbol(char *varname, const unsigned char value, 
		SymbolTable *sym_tbl){
		
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

int parseFile(const char *filename){
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

int getNextSymbol(char *buffer){

}

/**
 * Production:
 * 		stmt -> 'IF_IDEN' 'PAREN_S' expr 'PAREN_E' 'BRACE_S' stmt 'BRACE_E'
 */
void ifStatement(struct Term *term, FILE *input, ProgramData *prog){
	
	// vars
	char value;
	const unsigned int buf_size = 64;
	char misc_buf[buf_size];
	struct Term * new_term;
	
	// The 'IF_IDEN' terminal has already been consumed, process all else
	value = readNonEmptyChar(input, prog);
	if(value == PAREN_S){
		addChildTerm(createSingleCharTerm(PAREN_S), term);
	}
	else{
		// produce error
	}
	
	// consume the expression until the 'PAREN_E' is found
	if(consumeUntil(input, misc_buf, buf_size, PAREN_E, prog) == -1){
		// produce error
	}
	
	// produce a Term for the expression, then attempt to parse it
	addChildTerm(createTerm(misc_buf, buf_size), term);
	
	// parse statement within the if-statement
	// STUB
	
	// check parsed statement
	if(prog->has_error)
		return; // someone beneath us failed
	
	// add the closing parenthesis
	addChildTerm(createSingleCharTerm(PAREN_S), term);
	
	// next, consume the 'BRACE_S' indentifier
	value = readNonEmptyChar(input, prog);
	if(value == PAREN_S){
		addChildTerm(createSingleCharTerm(BRACE_S), term);
	}
	else{
		// produce error
	}
	
	// consume the following statement
	
	
	// consume the final indentifier, 'BRACE_E'
	
	
}

void addChildTerm(struct Term *child, struct Term *parent){
	int i = 0;
	while(parent->child_terms[i] != NULL){
		++i;
	}
	parent->child_terms[i] = child; // the compiler throws a warning?
}

void reportCompilerError(char * err_msg, ProgramData * prog){
	
}

char readNonEmptyChar(FILE *input, ProgramData *prog){
	
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

int consumeUntil(FILE *input, char *buf, const unsigned int buf_size, 
		const char term_char, ProgramData *prog){
	int chars_read = 0;
	while(chars_read < buf_size){
		buf[chars_read] = readNonEmptyChar(input, prog);
		chars_read++;
		if(buf[chars_read-1] == term_char)
			return chars_read;
	}
	return -1;
}

struct Term* createTerm(char* term, unsigned int term_len){
	struct Term * new_term = (struct Term *) calloc(1, sizeof(struct Term));
	strncpy(new_term->term, term, term_len);
	return new_term;
}

struct Term* createSingleCharTerm(const char term){
	struct Term * new_term = (struct Term *) calloc(1, sizeof(struct Term));
	new_term->term[0] = term;
	return new_term;
}
