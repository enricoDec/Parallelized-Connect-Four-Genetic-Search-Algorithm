#include <stdio.h>
#include "minunit.h"

int add(int a, int b) {
  return a + b;
}

void test_setup(void) {
	/* Nothing */
}

void test_teardown(void) {
	/* Nothing */
}

MU_TEST(test_add) {
  int result = add(2, 3);
  mu_assert(result == 5, "add test failed");
}

MU_TEST(test_add_2) {
  int result = add(1, 1);
  mu_assert(result == 2, "add test failed");
}

MU_TEST_SUITE(test_suite) {
  MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
  MU_RUN_TEST(test_add);
  MU_RUN_TEST(test_add_2);
}

int main() {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return 0;
}
