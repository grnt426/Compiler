/**
 * File:		terms.c
 * Author:		Grant Kurtz
 *
 * Description:	Handles holding terms so that post-processing of consumed
 * 				literals can be made.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "terms.h"
#include "generrors.h"


/**
 * Automatically adds the child to the parent term, adding it to the next
 * available slot.  Note that this function already expects
 */
void add_child_term(struct Term *child, struct Term *parent, 
		struct program *prog){

	// just in case...
	if(!parent->child_terms){
		parent->child_terms = 
				(struct Term **) malloc(3 * sizeof(struct Term*));
		memset(parent->child_terms, 0, 3);
		parent->child_count = 3;
	}

	int i = 0;
	while(i < parent->child_count && parent->child_terms[i] != 0){
		++i;
	}

	// just a small protection measure
	if(i < parent->child_count)
		parent->child_terms[i] = child;
	else{
		// resize, add to end
		parent->child_terms = (struct Term **) realloc(parent->child_terms,
				(parent->child_count * .75) * sizeof(struct Term*));
		parent->child_terms[parent->child_count] = child;
		parent->child_count = parent->child_count * .75;
	}
}

struct Term* create_term(char* term, unsigned int term_len, int children){
	struct Term * new_term = (struct Term *) malloc(sizeof(struct Term));
	new_term->child_count = children;
	if(!new_term){
		#ifdef DEBUG
			fprintf(stderr, "NEW TERM IS NUL.\n");
		#endif
	}
	new_term->term = (char *) malloc(term_len+1);
	strncpy(new_term->term, term, term_len);
	new_term->term[term_len] = '\0';
	if(children){
		new_term->child_terms = 
				(struct Term **) malloc(children * sizeof(struct Term*));
		memset(new_term->child_terms, 0, children);
	}
	else{
		new_term->child_terms = 0;
	}
	return new_term;
}

struct Term* create_single_char_term(const char term, int children){
	struct Term * new_term = (struct Term *) malloc(sizeof(struct Term));
	new_term->child_count = children;
	new_term->term = (char *) malloc(2);
	new_term->term[1] = '\0';
	new_term->term[0] = term;
	if(children){	
		new_term->child_terms = 
				(struct Term **) malloc(children * sizeof(struct Term*));
		memset(new_term->child_terms, 0, children);
	}
	else{
		new_term->child_terms = 0;
	}
	return new_term;
}



