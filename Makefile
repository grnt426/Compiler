# Used for compiling the project.  If no make target is specified, by default
# only the object files necessary to create the Hartz translator are compiled.
CC = gcc
CFLAGS = -std=c99 -Wall -lm
COMMON_FILES = symbols.c idents.c strlib.c generrors.c terms.c
HARTZ_FILES = translator.c
CCODE_FILES = compiler.c
HARTZ_EXEC = translator
CCODE_EXEC = compiler


all: hartz ccode

# To translate Hartz assembly into a "binary executable"
hartz: $(HARTZ_FILES) $(COMMON_FILES)
	$(CC) $(CFLAGS) -o $(HARTZ_EXEC) $(HARTZ_FILES) $(COMMON_FILES)

# To compile C-Style code into Hartz Assembly
ccode: $(CCODE_FILES) $(COMMON_FILES)
	$(CC) $(CFLAGS) -o $(CCODE_EXEC) $(CCODE_FILES) $(COMMON_FILES)

# Just cleans up object files, which aren't needed after the linker creates
# the executable
clean:
	rm -f *.o 
