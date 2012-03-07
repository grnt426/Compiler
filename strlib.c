/**
 * File:		strlib.c
 * Author:		Grant Kurtz
 * Date:		Feb 2012
 *
 * Description:	Contains several useful string processing functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "strlib.h"
#include "string.h"
#include "ctype.h"

/**
 * Removes all trailing and leading whitespace, but will leave whitespace
 * inbetween valid characters.  Note, this function will move the resulting
 * cstring to the start of the char array so that the originally provided
 * pointer can be freed.
 * Source: 	http://stackoverflow.com/a/123724
 * 
 * @param	s	The cstring to be stripped.
 */
void trimwhitespace(char *s){
	char * p = s;
	int l = strlen(p);
	while(isspace(p[l - 1])) p[--l] = 0;
	while(* p && isspace(* p)) ++p, --l;
	memmove(s, p, l + 1);
}

/**
 * A convenience function for turning an entire string into uppercase.
 */
void strtoupper(char *str, int len){
	while(len){
		str[len-1] = toupper(str[len-1]);
		len--;
	}
}

int letter_count(const char *str){
	int count = 0;
	int c = 0;
	while(str[c]){
		if( (str[c] >= 'a' && str[c] <= 'z') || 
				(str[c] >= 'A' && str[c] <= 'Z') ){
			count++;
		}
		c++;
	}
	return count;
}

/**
 * Will process the first token of the given input buffer (or return the next
 * token of the previously provided buffer if buf is 0), trim whitespace, 
 * convert the entire buffer to uppercase, and then return the token.
 */
char *read_next_token(char *buf, FILE *input, int buf_size){
	fgets(buf, buf_size, input);
	trimwhitespace(buf);
	strtoupper(buf, strlen(buf));
	return strtok(buf, STR_TOK_SEP);
}

/**
 * A convenience function for finding the end of a given file.
 */
short check_EOF(FILE *file){
	int c = fgetc(file);
	if(feof(file))
		return 1;
	ungetc(c, file);
	return 0;
}

/**
 * Converts a digit into its character equivalent.
 *
 * @param	d		The digit to convert
 * @return			The character represent of the digit
 */
char dtoc(const int d){
	static const char nums[] = "0123456789";
	return nums[d];
}

/**
 * Converts a number into a string equivalent.
 *
 * @param	num		The number to convert
 * @return			The string representation of the number.
 */
char *numtos(int num){
	int len = numd(num);
	char *number = (char *) malloc(len);
	while(len){
		number[len-1] = dtoc(num % 10);
		num /= 10;
		len--;
	}
	return number;
}

/**
 * Converts a base-10 (positive) integer to base-2. Note that the left most
 * bit is the most significant bit.
 * @param	num			The number to parse
 * @param	min_size	The minimum number of bits to show.
 * @return				The converted base-10 value.
 */
char *numtob(int num, int min_size){
	
	// vars
	char *bin = malloc(33);
	memset(bin, '0', 33);
	bin[32] = 0;
	int pos = 31;

	// reduce to base two
	do{
		if(num % 2)
			bin[pos] = '1';
		else
			bin[pos] = '0';
		pos--;
	}while(num /= 2);
	
	// make the buffer as small as possible
	min_size = min_size < 32 - pos ? 32 - pos : min_size;
	char *ret_bin = malloc(min_size + 1);
	memset(ret_bin, '0', min_size);
	ret_bin[min_size] = 0;
	memcpy(ret_bin + (min_size - (32 - pos) + 1), bin + pos + 1, min_size + 1);
	return ret_bin;
}

/**
 * Returns the number of digits in the given number.
 *
 * @param	num		The number to parse.
 * @return			The number of individual digits in the number.
 */
int numd(int num){
	return (num == 0) ? 1 : (int) log10((double)num)+1;
}

/**
 * Converts a string of numerical digits (0-9) into a signed integer.
 * @param	s		The string to convert.
 * @return			The signed integer on success, otherwise 0 is returned.
 */
int stonum(char *s){

	// vars
	int num = 0;
	int d;

	// check if we need to sign it
	int c = 0;
	if(s[0] == '-')
		c = 1;
	
	// convert
	while(s[c]){
		d = ctod(*s);
		if(d < 0)
			return 0;
		num += d;
		c++;
	}

	// sign the value
	if(s[0] == '-')
		num *= -1;
	return num;
}

/**
 * Converts the numerical character into integer form.
 * @param	c		The character t oconvert
 * @return			-1 if the character is not numerical, otherwise the
 * 					returned value is given.
 */
int ctod(char c){
	return c >= 0 && c <= 9 ? -1 : c - '0';
}

