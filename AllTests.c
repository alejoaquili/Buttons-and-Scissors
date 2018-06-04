#include <stdio.h>
#include "CuTest.h"
#include "buttonsAndScissorsBack.h"
#include "buttonsAndScissorsFront.h"
#include "FrontTestSuite.h"
#include "BackTestSuite.h"

void
RunAllTests(void)
{
	CuString * output = CuStringNew();
	CuSuite * suite = CuSuiteNew();
	CuSuiteAddSuite(suite, frontSuite());
	CuSuiteAddSuite(suite, backSuite());
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int
main(void)
{
	RunAllTests();
}
