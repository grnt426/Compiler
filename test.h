#ifndef TEST_H
#define TEST_H

// Folder locations for output comparison, input files
#define TEST_IN		"test_input/"
#define TEST_OUT	"test_output/"
#define TEST_SOUT	"test_stdout/"
#define TEST_SERR	"test_stderr/"

// The file name for all tests
#define TEST_FILE	"test."

// The highest test number (generally the range is the set of natural numbers)
#define TEST_CNT	6

// The program we are testing
#define	TRANS_EXEC	"./translator"

short check_files(int test_num);
void print_status(const char *color, const char *indent, FILE *out);
void cleanup_older();
short check_executable();


#endif

