#include "minunit.h"
#include "gameTest.c"
#include "geneticSearchTest.c"

void test_setup(void) {
    /* Nothing */
}

void test_teardown(void) {
	/* Nothing */
}

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_initGame);
    MU_RUN_TEST(test_changeBoard);
    MU_RUN_TEST(test_getStartPlayer);
    MU_RUN_TEST(test_cleanupGame);
    MU_RUN_TEST(test_isBoardFull);
    MU_RUN_TEST(test_doMove);
    MU_RUN_TEST(test_undoMove);
    MU_RUN_TEST(test_checkWin);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
