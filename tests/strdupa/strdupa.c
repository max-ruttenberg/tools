#include <stdio.h>
#include <string.h>
#include <tools/arrayops.h>
#include <tools/strdupa.h>

struct strdupa_test {
	const char *test_name;
	const char *test_target;
	const char *test_expect;
};

#define DEFINE_STRDUPA_TEST(t_name, t_target, t_expect)	\
	{__FILE__ " " t_name, t_target, t_expect},

static struct strdupa_test tests [] = {
#include "tests.inc"
};

#undef DEFINE_STRDUPA_TEST

#define test_failure(test, result, reason)				\
	printf("%s: input=%s, expected=%s, result=%s: failure: %s\n",	\
	       test->test_name,						\
	       test->test_target ? test->test_target : "(nullptr)",	\
	       test->test_expect ? test->test_expect : "(nullptr)",	\
	       result ? result : "(nullptr)",				\
	       reason)							
	
#define test_success(test, result)					\
	printf("%s: input=%s, expected=%s, result=%s: success\n",	\
	       test->test_name,						\
	       test->test_target ? test->test_target : "(nullptr)",	\
	       test->test_expect ? test->test_expect : "(nullptr)",	\
	       result ? result : "(nullptr)")							

static int leaf_routine(struct strdupa_test *test)
{
	const char *result = strdupa(test->test_target);

	if (test->test_target == NULL) {
		if (result == NULL) {
			test_success(test, result);
			return 0;
		} else {
			test_failure(test, result, "null input pointer not handled");
			return 1;
		}			
	} else if (result == test->test_target) {
		test_failure(test, result, "result points to input");
		return 1;
	} else if (result == NULL) {
		test_failure(test, result, "result is NULL");
		return 1;
	} else if (strcmp(result, test->test_target)) {
		test_failure(test, result, "result does not match input");
		return 1;
	} else {
		test_success(test, result);
		return 0;
	}
}



static int run_leaf_routine_tests(void)
{
	int i, ret = 0;
	char *tmp;
	
	for (i = 0; i < ARRAY_SIZE(tests); i++) {
		ret = ret || leaf_routine(&tests[i]);
	}
	return ret;
}



int main(int argc, char *argv[])
{
	char *test;
	int ret;
	
	if (argc > 1) {
		test = argv[1];
		if (strcmp(test, "leaf-routine")==0) {
			ret = run_leaf_routine_tests();
		} 
	}
	return ret;
}
