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
	int total_failed = 0;
	while(test_num < TEST_CNT){
		test_num++;
		printf("\n\t--- Test Input #%d ---\n", test_num);
		if(check_files(test_num))
			continue;
		run_test(TRANS_EXEC, test_num);
		total_failed += compare_results(test_num);
	}

	printf("\n\t\t===== Summary =====\n");
	if(total_failed >= TEST_CNT *.5)
		print_status(RED_C, 0, stdout);
	else if(total_failed)
		print_status(YLW_C, 0, stdout);
	else
		print_status(GRN_C, 0, stdout);
	printf("%d of %d tests failed.\n", total_failed, TEST_CNT);

	print_status(WHT_C, 0, stdout);
	printf("Test Complete.\n");
}

/**
 * Checks to make sure all relevant files exist.
 *
 * @param test_num		Which tests to check exist.
 * @return				0 if the files exist and are readable, otherwise
 * 						1;
 */
short check_files(int test_num){
	return 0;
}

/**
 * A convenience function for printing a colored asterisk in front of messages.
 *
 * @param	color	The color to make the asterisk.
 * @param	indent	Any extra indentation (or anything really) to add before
 * 					the asterisk.
 * @param	out		Where to make the output go.
 */
void print_status(const char *color, const char *indent, FILE *out){
	if(indent)
		fprintf(out, "%s", indent);
	fprintf(out, "%s * " RST_C, color);
}

/**
 * Deletes older test result data so as not to confuse an older test result
 * with a new one.
 */
void cleanup_older(){
	
}

/**
 * Makes sure the executable we are testing against exists and is executable.
 *
 * @return		0 on success, otherwise 1
 */
short check_executable(){
	return 0;
}

/**
 * Performs the test on the given exceutable and will wait until the test
 * completes before returning.
 *
 * @param	exec		The program to test against
 * @param	test_num	The numbered test to run against the executable.
 */
void run_test(char *exec, int test_num){
	
	int pid = 0, out, err;
	char *outbuf, *errbuf, *inbuf, *resbuf;
	char **prog;
	pid = fork();
	
	switch(pid){

		// fork failed!
		case -1:
			print_status(RED_C, 0, stderr);
			fprintf(stderr, "Unable to fork child!\n");
			exit(1);

		case 0:
			
			// setup file redirects
			outbuf = (char *) malloc(64);
			memset(outbuf, 0, 64);
			sprintf(outbuf, "%s%s%d.out", TEST_RES, TEST_FILE, test_num);
			out = open(outbuf, O_WRONLY | O_CREAT, 0770);
			dup2(out, STDOUT_FILENO);
			errbuf = (char *) malloc(64);
			memset(errbuf, 0, 64);
			sprintf(errbuf, "%s%s%d.err", TEST_RES, TEST_FILE, test_num);
			err = open(errbuf, O_WRONLY | O_CREAT, 0770);
			dup2(err, STDERR_FILENO);
			
			// build arguments
			prog = (char **) malloc(4 * sizeof(char *));
			for(int i = 1; i < 3; i++){
				prog[i] = (char *) malloc(4 * 4);
			}
			prog[0] = exec;
			inbuf = (char *) malloc(64);
			memset(inbuf, 0, 64);
			sprintf(inbuf, "%s%s%d", TEST_IN, TEST_FILE, test_num);
			prog[1] = inbuf;
			resbuf = (char *) malloc(64);
			memset(resbuf, 0, 64);
			sprintf(resbuf, "%s%s%d.b", TEST_RES, TEST_FILE, test_num);
			prog[2] = resbuf;
			prog[3] = '\0'; // last argument must be nul
			
			// actually execute the test
			execvp(prog[0], prog);
			print_status(RED_C, 0, stderr);
			fprintf(stderr, "Unable to execute '%s'!\n", 
					TRANS_EXEC);
			_exit(1);
	}

	// simply wait for the child to finish
	waitpid(pid, 0, 0);
}

/**
 * Does a line-by-line comparison of the expected output against the actual
 * output the program generated.
 *
 * @param	test_num	The numbered test that we will compare.
 * @return				0 if the comparison found errors, otherwise 1.
 */
int compare_results(int test_num){

	/*
	char *outbuf = (char *) malloc(64);
	memset(outbuf, 0, 64);
	sprintf(outbuf, "%s%s%d.out", TEST_RES, TEST_FILE, test_num);
	char *errbuf = (char *) malloc(64);
	memset(errbuf, 0, 64);
	sprintf(errbuf, "%s%s%d.err", TEST_RES, TEST_FILE, test_num);
	*/

	// Read from Example out
	char *bbuf = (char *) malloc(64);
	memset(bbuf, 0, 64);
	sprintf(bbuf, "%s%s%d", TEST_OUT, TEST_FILE, test_num);
	FILE *tres = fopen(bbuf, "r");

	// Read from results of test
	sprintf(bbuf, "%s%s%d.b", TEST_RES, TEST_FILE, test_num);
	FILE *res = fopen(bbuf, "r");
	if(!res){
		print_status(RED_C, 0, stderr);
		fprintf(stderr, "Unable to open '%s'!\n", bbuf);
		print_test_failed();
		return 1;
	}

	// setup some buffers for comparison
	char *tline, *oline;
	tline = (char *) malloc(64);
	oline = (char *) malloc(64);
	int line = 0;
	int failure = 0;

	// check each line
	while(fgets(tline, 64, tres)){
		fgets(oline, 64, res);
		
		// make sure the read went smoothly
		if(feof(res)){
			print_status(RED_C, 0, stdout);
			printf("Compiled file has fewer lines than expected!\n");
			print_test_failed();
			return 1;
		}
		else if(ferror(res)){
			print_status(RED_C, 0, stdout);
			printf("Compiled file, '%s', had an error during "
					"reading!\n", bbuf);
			print_test_failed();
			return 1;
		}

		// clean up the lines first
		trimwhitespace(tline);
		trimwhitespace(oline);
		
		// equate lines
		if(strcmp(tline, oline)){
			print_status(RED_C, 0, stdout);
			printf("Line %d:\texpected '%s', but read '%s'!\n", 
					line+1, tline, oline);
			failure = 1;
		}
		line++;
	}

	// summary of this test
	if(failure)
		print_test_failed();
	else
		print_test_success();
	return failure;
}

/**
 * A convenience function for indicating the test failed.
 */
void print_test_failed(){
	print_status(RED_C, 0, stdout);
	printf("Test Failed!\n");
}

/**
 * A convenience function for indicating a test was successful.
 */
void print_test_success(){
	print_status(GRN_C, 0, stdout);
	fprintf(stdout, "Test Successful!\n");
}

