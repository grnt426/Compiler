#ifndef TERMS_H
#define TERMS_H

/**
 * Term
 * char* term	Contains the string that represents this term
 * int pos		The position of this term relative to the start term
 * struct Term **	The direct children of this term
 * struct Term *	The term that follows this term
 */
struct Term{
	char* term;
	int pos;
	struct Term **child_terms;
	struct Term *next_term;
};

struct program;

// Term Manipulation Functions
void 			add_child_term(struct Term *, struct Term *, 
		struct program *prog);
struct Term* 	create_term(char* term, unsigned int term_len, int children);
struct Term* 	create_single_char_term(const char term, int children);



#endif


