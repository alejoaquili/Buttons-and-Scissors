COMPILER=gcc
OUTPUT_FILE1=buttonsAndScissors
OUTPUT_FILE2=buttonsAndScissorsTest
FILES1=buttonsAndScissorsBack.c buttonsAndScissorsFront.c  main.c 
FILES2=buttonsAndScissorsBack.c buttonsAndScissorsFront.c  CuTest.c AllTests.c BackTestSuite.c FrontTestSuite.c
all: game test
game:
	$(COMPILER) -Wall -pedantic -std=c99 -o $(OUTPUT_FILE1) $(FILES1)
clean1:
	rm $(OUTPUT_FILE1)
test:

	$(COMPILER) -Wall -pedantic -std=c99 -o $(OUTPUT_FILE2) $(FILES2)
clean2:
	rm $(OUTPUT_FILE2)