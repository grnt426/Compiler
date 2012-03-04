
void addChildTerm(struct Term *child, struct Term *parent){
	int i = 0;
	while(parent->child_terms[i] != NULL){
		++i;
	}
	parent->child_terms[i] = child;
}

struct Term* createTerm(char* term, unsigned int term_len, int children){
	struct Term * new_term = (struct Term *) calloc(1, sizeof(struct Term));
	strncpy(new_term->term, term, term_len);
	new_term->child_terms = 
			(struct Term **) malloc(children * sizeof(struct Term*));
	memset(new_term->child_terms, 0, children);
	return new_term;
}

struct Term* createSingleCharTerm(const char term){
	struct Term * new_term = (struct Term *) calloc(1, sizeof(struct Term));
	new_term->term[0] = term;
	new_term->child_terms = 
			(struct Term **) malloc(children * sizeof(struct Term*));
	memset(new_term->child_terms, 0, children);
	return new_term;
}



