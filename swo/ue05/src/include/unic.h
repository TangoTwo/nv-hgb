/**
 * The header is from my public gist:
 * https://gist.github.com/YourPsychiatrist/ab56d2879d51289a4170196a3b57b5d9
 * I am the rightful owner of this code
 */
#ifndef UNIC_H
#define UNIC_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * PUBLIC INTERFACE
 */

/**
 * Initializes the unic library
 */
void unic_init();

/**
 * Prints test results. (Only relevant if
 * unic_init was called before running tests!)
 * @return EXIT_SUCCESS if all tests succeeded,
 *          EXIT_FAILURE otherwise.
 */
int unic_get_results();

/**
 * Succeeds if the supplied bool evaluates to true.
 * @param got The value to check.
 * @param name The name of the test.
 */
void unic_ass_true(bool got, const char *name);

/**
 * Succeeds if the supplied bool evaluates to false.
 * @param got The value to check.
 * @param name The name of the test.
 */
void unic_ass_false(bool got, const char *name);

/**
 * Succeeds if the supplied values are equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_eq_b(bool a, bool b, const char *name);

/**
 * Succeeds if the supplied values are not equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_not_eq_b(bool a, bool b, const char *name);

/**
 * Succeeds if the supplied values are equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_eq_i(int a, int b, const char *name);

/**
 * Succeeds if the supplied values are not equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_not_eq_i(int a, int b, const char *name);

/**
 * Succeeds if the supplied values are equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_eq_str(const char *a, const char *b, const char *name);

/**
 * Succeeds if the supplied values are not equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_not_eq_str(const char *a, const char *b, const char *name);

/**
 * Succeeds if the supplied values are equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_eq_f(double a, double b, const char *name);

/**
 * Succeeds if the supplied values are not equal.
 * @param a One of the values to compare.
 * @param b The other value.
 * @param name The name of the test.
 */
void unic_ass_not_eq_f(double a, double b, const char *name);

/*
 * "PRIVATE INTERFACE"
 */

// A list of colors for the terminal.

const char * UNIC_COLOR_RED = "\x1b[31m";
const char * UNIC_COLOR_GREEN = "\x1b[32m";
const char * UNIC_COLOR_CYAN = "\x1b[36m";
const char * UNIC_COLOR_RESET = "\x1b[0m";

// Formatting strings

const char *UNIC_EXPECTED_INDENT = "\t|-";
const char *UNIC_GOT_INDENT = "\t'-";

/**
 * The compound holding test results.
 */
struct
{
    unsigned long long succeeded_tests;
    unsigned long long failed_tests;
} global_results;

/**
 * A generic message printing whether a test
 * succeeded.
 * @param success The success flag.
 * @param name The name of the test.
 */
void unic_print_success(bool success, const char * name);

/**
 * Prints the difference between the two
 * arguments a and b.
 */
void unic_print_diff_b(bool a, bool b);

/**
 * Prints the difference between the two
 * arguments a and b.
 */
void unic_print_diff_i(int a, int b);

/**
 * Prints the difference between the two
 * arguments a and b.
 */
void unic_print_diff_str(const char *a, const char *b);

/**
 * Prints the difference between the two
 * arguments a and b.
 */
void unic_print_diff_f(double a, double b);

/**
 * Prints a mismatch message for index i.
 * @param i The index at which a mismatch
 *          happened.
 */
void unic_print_diff_array(size_t i);

/**
 * Prints a message saying that the
 * arrays differ in size.
 */
void unic_print_array_diff_size();

/**
 * Prints a message saying that the
 * arrays are equal.
 */
void unic_print_array_eq();

/*
 * IMPLEMENTATION
 */

void unic_init()
{
    global_results.succeeded_tests = 0;
    global_results.failed_tests = 0;
}

int unic_get_results()
{
    printf("==================\n");
    printf("UNIC ran %llu tests.\n", global_results.succeeded_tests + global_results.failed_tests);
    if (global_results.succeeded_tests != 0) {
        printf("%llu tests %ssucceeded%s.\n", global_results.succeeded_tests, UNIC_COLOR_GREEN, UNIC_COLOR_RESET);
    }
    if (global_results.failed_tests != 0) {
        printf("%llu tests %sfailed%s.\n", global_results.failed_tests, UNIC_COLOR_RED, UNIC_COLOR_RESET);
    }
    printf("==================\n");
    return global_results.failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

void unic_print_success(bool success, const char * name)
{
    if (success) {
        ++global_results.succeeded_tests;
    } else {
        ++global_results.failed_tests;
    }
    const char *color = success ? UNIC_COLOR_GREEN : UNIC_COLOR_RED;
    printf("@ Test \"%s%s%s\" ", UNIC_COLOR_CYAN, name, UNIC_COLOR_RESET); // print name
    printf("%s%s%s.\n", color, success ? "succeeded" : "FAILED", UNIC_COLOR_RESET); // print status
}

void unic_ass_true(bool got, const char *name)
{
    bool success = got;
    unic_print_success(success, name);
    if (!success) {
        printf("%s Assertion failed, argument was \"false\".\n", UNIC_GOT_INDENT);
    }
}

void unic_ass_false(bool got, const char *name)
{
    bool success = !got;
    unic_print_success(success, name);
    if (!success) {
        printf("%s Assertion failed, argument was \"true\".\n", UNIC_GOT_INDENT);
    }
}

void unic_print_diff_b(bool a, bool b)
{
    printf("%s Arg 1: %d\n%s Arg 2: %d\n", UNIC_EXPECTED_INDENT, a, UNIC_GOT_INDENT, b);
}

void unic_ass_eq_b(bool a, bool b, const char *name)
{
    bool success = a == b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_b(a, b);
    }
}

void unic_ass_not_eq_b(bool a, bool b, const char *name)
{
    bool success = a != b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_b(a, b);
    }
}

void unic_print_diff_i(int a, int b)
{
    printf("%s Arg 1: %d\n%s Arg 2: %d\n", UNIC_EXPECTED_INDENT, a, UNIC_GOT_INDENT, b);
}

void unic_ass_eq_i(int a, int b, const char *name)
{
    bool success = a == b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_i(a, b);
    }
}

void unic_ass_not_eq_i(int a, int b, const char *name)
{
    bool success = a != b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_i(a, b);
    }
}

void unic_print_diff_str(const char *a, const char *b)
{
    printf("%s Arg 1: %s\n%s Arg 2: %s\n", UNIC_EXPECTED_INDENT, a, UNIC_GOT_INDENT, b);
}

void unic_ass_eq_str(const char *a, const char *b, const char *name)
{
    bool success = strcmp(a, b) == 0;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_str(a, b);
    }
}

void unic_ass_not_eq_str(const char *a, const char *b, const char *name)
{
    bool success = strcmp(a, b) != 0;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_str(a, b);
    }
}

void unic_print_diff_f(double a, double b)
{
    printf("%s Arg 1: %f\n%s Arg 2: %f\n", UNIC_EXPECTED_INDENT, a, UNIC_GOT_INDENT, b);
}

void unic_ass_eq_f(double a, double b, const char *name)
{
    bool success = a == b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_f(a, b);
    }
}

void unic_ass_not_eq_f(double a, double b, const char *name)
{
    bool success = a != b;
    unic_print_success(success, name);
    if (!success) {
        unic_print_diff_f(a, b);
    }
}

/*
 * Array assertions are implemented as macros
 * because C doesn't support generic functions
 * and I am too lazy to write array functions
 * for all data types covered by unic.
 */

void unic_print_diff_array(size_t i)
{
    printf("%s Arrays differ at index %lu\n", UNIC_GOT_INDENT, i);
}

void unic_print_array_diff_size()
{
    printf("%s The arrays have different lengths.\n", UNIC_GOT_INDENT);
}

void unic_print_array_eq()
{
    printf("%s Arrays are equal.\n", UNIC_GOT_INDENT);
}

#define unic_array_size(x) \
    (sizeof(x) / sizeof((x)[0]))

#define unic_ass_array_eq(a1, a2, name)                             \
    if (unic_array_size(a1) != unic_array_size(a2)) {               \
        unic_print_success(false, name);                            \
        unic_print_array_diff_size();                               \
    } else {                                                        \
        size_t i = 0;                                               \
        while (i < unic_array_size(a1) && (a1)[i] == (a2)[i]) ++i;  \
        unic_print_success(i == unic_array_size(a1), name);         \
        if (i != unic_array_size(a1)) { unic_print_diff_array(i); } \
    }

#define unic_ass_array_not_eq(a1, a2, name)                         \
    if (unic_array_size(a1) != unic_array_size(a2)) {               \
        unic_print_success(true, name);                             \
    } else {                                                        \
        size_t i = 0;                                               \
        while (i < unic_array_size(a1) && (a1)[i] == (a2)[i]) ++i;  \
        unic_print_success(i != unic_array_size(a1), name);         \
        if (i == unic_array_size(a1)) { unic_print_array_eq(); }    \
    }

#endif // !UNIC_H