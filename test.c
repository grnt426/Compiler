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
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
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
		run_test(test_num);
		compare_results(test_num);
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

void run_test(int test_num){
	
	int pid = 0, out, err;
	char *buf;
	char **prog;

	switch(pid = fork()){

		case -1:
			print_status(RED_C, 0, stderr);
			fprintf(stderr, "Unable to fork child! Tests halted!\n");
			exit(1);

		case 0:
			
			// setup file redirects
			buf = (char *) malloc(64);
			sprintf(buf, "%s%s%d", TEST_SOUT, TEST_FILE, test_num);
			out = open(buf, O_WRONLY | O_CREAT, 0770);
			dup2(out, STDOUT_FILENO);
			sprintf(buf, "%s%s%d", TEST_SERR, TEST_FILE, test_num);
			err = open(buf, O_WRONLY | O_CREAT, 0770);
			dup2(err, STDERR_FILENO);

			// build program name
			prog = (char **) malloc(4 * sizeof(char *));
			prog[0] = (char *) malloc(sizeof(TRANS_EXEC)+1);
			prog[0] = TRANS_EXEC;
			prog[0][sizeof(TRANS_EXEC)] = 0;

			// build arguments
			for(int i = 1; i < 3; i++){
				prog[i] = (char *) malloc(64);
			}
			prog[3] = '\0'; // last argument must be nul
			sprintf(prog[1], "%s%s%d", TEST_IN, TEST_FILE, test_num);
			sprintf(prog[2], "%s%s%d", TEST_RES, TEST_FILE, test_num);
			execvp(prog[0], prog);
			print_status(RED_C, 0, stderr);
			fprintf(stderr, "Unable to execute '%s'! Tests halted!\n", 
					TRANS_EXEC);
			exit(1);
	}

	// simply wait for the child to finish
	waitpid(pid, 0, 0);
}

void compare_results(int test_num){
	

}

