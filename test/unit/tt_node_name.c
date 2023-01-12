#include "tt_node_name.h"

#include "trivia/util.h"
#define UNIT_TAP_COMPATIBLE 1
#include "unit.h"

static void
test_tt_node_name_nil(void)
{
	header();
	plan(16);

	const char *nil_names[] = {"", NULL};
	const char *not_nil_names[] = {
		"a", "nil", "null", "false"
	};
	struct tt_node_name name;
	for (int i = 0; i < (int)lengthof(nil_names); ++i) {
		const char *str = nil_names[i];
		is(tt_node_name_set_str(&name, str), 0,
		   "set nil name %d", i);
		ok(tt_node_name_is_nil(&name), "nil name %d", i);
	}
	for (int i = 0; i < (int)lengthof(not_nil_names); ++i) {
		const char *str = not_nil_names[i];
		is(tt_node_name_set_str(&name, str), 0,
		   "set not nil name %d", i);
		ok(!tt_node_name_is_nil(&name), "not nil name %d", i);
	}
	is(tt_node_name_set_str(&name, "abc"), 0, "set abc");
	ok(!tt_node_name_is_nil(&name), "not nil abc");
	tt_node_name_set_nil(&name);
	ok(tt_node_name_is_nil(&name), "set nil");
	ok(tt_node_name_is_nil(NULL), "NULL == nil");

	check_plan();
	footer();
}

static void
test_tt_node_name_set(void)
{
	header();
	plan(44);

	/*
	 * Bad names.
	 */
	const char *bad_names[] = {
		"1",
		"1abc",
		"*",
		"a_b",
		"aBcD",
		"a~b",
		"{ab}",
	};
	struct tt_node_name name;
	for (int i = 0; i < (int)lengthof(bad_names); ++i) {
		const char *str = bad_names[i];
		is(tt_node_name_set_str_n(&name, str, strlen(str)), -1,
		   "bad name set str n %d", i);
		is(tt_node_name_set_str(&name, str), -1,
		   "bad name set str %d", i);
	}
	/*
	 * Valid names.
	 */
	const char *good_names[] = {
		"",
		"a",
		"a-b-c",
		"abc",
		"a1b2c3-d4-e5-",
	};
	struct tt_node_name copy;
	for (int i = 0; i < (int)lengthof(good_names); ++i) {
		const char *str = good_names[i];
		strlcpy(name.data, "garbage", TT_NODE_NAME_SIZE_MAX);
		is(tt_node_name_set_str_n(&name, str, strlen(str)), 0,
		   "good name set str n %d", i);
		is(strcmp(name.data, str), 0, "correct");

		strlcpy(name.data, "garbage", TT_NODE_NAME_SIZE_MAX);
		is(tt_node_name_set_str(&name, str), 0,
		   "good name set str %d", i);
		is(strcmp(name.data, str), 0, "correct");

		strlcpy(copy.data, "garbage", TT_NODE_NAME_SIZE_MAX);
		tt_node_name_set(&copy, &name);
		is(strcmp(copy.data, str), 0, "correct");
	}
	strlcpy(name.data, "garbage", TT_NODE_NAME_SIZE_MAX);
	is(tt_node_name_set_str_n(&name, NULL, 0), 0, "set str n nil");
	ok(tt_node_name_is_nil(&name), "correct");

	strlcpy(name.data, "garbage", TT_NODE_NAME_SIZE_MAX);
	is(tt_node_name_set_str(&name, NULL), 0, "set str nil");
	ok(tt_node_name_is_nil(&name), "correct");

	strlcpy(name.data, "garbage", TT_NODE_NAME_SIZE_MAX);
	tt_node_name_set(&name, NULL);
	ok(tt_node_name_is_nil(&name), "correct");

	check_plan();
	footer();
}

static void
test_tt_node_name_is_equal(void)
{
	header();
	plan(42);

	const char *eq_pairs[] = {
		NULL, NULL,
		"", NULL,
		NULL, "",
		"", "",
		"a", "a",
		"ab", "ab",
	};
	for (int i = 0; i < (int)lengthof(eq_pairs); i += 2) {
		struct tt_node_name name1, name2;
		is(tt_node_name_set_str(&name1, eq_pairs[i]), 0,
		   "set 1 in %d", i);
		is(tt_node_name_set_str(&name2, eq_pairs[i + 1]), 0,
		   "set 2 in %d", i);
		ok(tt_node_name_is_equal(&name1, &name2), "eq %d", i);
	}
	const char *neq_pairs[] = {
		NULL, "a",
		"a", NULL,
		"a", "",
		"", "a",
		"ab", "a",
		"a", "ab",
		"ab", "cd",
	};
	for (int i = 0; i < (int)lengthof(neq_pairs); i += 2) {
		struct tt_node_name name1, name2;
		is(tt_node_name_set_str(&name1, neq_pairs[i]), 0,
		   "set 1 %d", i);
		is(tt_node_name_set_str(&name2, neq_pairs[i + 1]), 0,
		   "set 2 %d", i);
		ok(!tt_node_name_is_equal(&name1, &name2), "neq %d", i);
	}
	ok(tt_node_name_is_equal(NULL, NULL), "NULL == NULL");
	struct tt_node_name name;
	tt_node_name_set_nil(&name);
	ok(tt_node_name_is_equal(NULL, &name), "NULL == nil");
	ok(tt_node_name_is_equal(&name, NULL), "nil == nil");

	check_plan();
	footer();
}

static void
test_tt_node_name_str(void)
{
	header();
	plan(5);

	const char *stub = "<no-name>";
	struct tt_node_name name;
	tt_node_name_set_nil(&name);
	is(strcmp(tt_node_name_str(&name), stub), 0, "nil");

	is(tt_node_name_set_str(&name, "abc"), 0, "set not nil");
	is(strcmp(tt_node_name_str(&name), "abc"), 0, "not nil");
	is(tt_node_name_str(&name), name.data, "not nil data");

	is(strcmp(tt_node_name_str(NULL), stub), 0, "NULL");

	check_plan();
	footer();
}

int
main(void)
{
	header();
	plan(4);

	test_tt_node_name_nil();
	test_tt_node_name_set();
	test_tt_node_name_is_equal();
	test_tt_node_name_str();

	int rc = check_plan();
	footer();
	return rc;
}
