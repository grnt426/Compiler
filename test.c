/**
 * File:		test.c
 * Author:		Grant Kurtz
 * Date:		Mar 2012
 *
 * Description:	A testing suite for comparing program output against
 * 				pre-generated test results.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "strlib.h"

/**
 * Main testing driver; runs all input tests and compares to output files.
 */
int main(int argc, char **argv){

	// Print some banner for whatever reason
	printf("\t\t===== Translator Testing =====\n");

	// Check our environment first
	print_status(WHT_C, 0, stdout);
	printf("Performing clean-up from previous tests...\n");
	cleanup_older();
	print_status(WHT_C, 0, stdout);
	printf("Checking executable...\n");
	if(check_executable())
		exit(1);
	print_status(GRN_C, 0, stdout);
	printf("Ready! Running all tests!\n");

	// Run all available tests
	int test_num = 0;
	while(test_num < TEST_CNT){
		test_num++;

		printf("\t--- Test Input #%d ---\n", test_num);
		
		if(check_files(test_num))
			continue;



	}

	print_status(WHT_C, 0, stdout);
	printf("Test Complete\n");
}

short check_files(int test_num){
	return 0;
}

void print_status(const char *color, const char *indent, FILE *out){
	if(indent)
		fprintf(out, "%s", indent);
	fprintf(out, "%s * " RST_C, color);
}

void cleanup_older(){

}

short check_executable(){
	return 0;
}

