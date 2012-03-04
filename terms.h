#ifndef TERMS_H
#define TERMS_H

struct Term{
	char* term;
	int pos;
	struct Term **child_terms;
	struct Term *next_term;
};

// Term Manipulation Functions
void 			addChildTerm(struct Term *, struct Term *);
struct Term* 	createTerm(char* term, unsigned int term_len, int children);
struct Term* 	createSingleCharTerm(const char term);



#endif
