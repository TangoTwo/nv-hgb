#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef enum
{
    false, true
} bool;
typedef int *const CountsArray;
typedef const int *const LengthsArray;
typedef bool (*SolutionFunction)(int, LengthsArray, CountsArray, int, int);

/**
 * @see possible
 */
bool possible_it(int x, const int *const lengths, const int *const counts, int n)
{
    // I am eagerly awaiting the first time I see code like this at work...

    if (n > 3) {
        printf("The iterative version will only consider the first 3 pipe lengths.\n");
    }
    bool possible = false;
    int sum = 0;
    int first = 0;
    while (first < counts[0] + 1 && !possible) {
        sum = lengths[0] * first;
        possible = sum == x;

        if (n > 1) {
            int second = 0;
            while (second < counts[1] + 1 && !possible) {
                sum = lengths[0] * first + lengths[1] * second;
                possible = sum == x;

                if (n > 2) {
                    int third = 0;
                    while (third < counts[2] + 1 && !possible) {
                        sum = lengths[0] * first + lengths[1] * second + lengths[2] * third; // look ma! I'm on top of the pyramid!!
                        possible = sum == x;

                        ++third;
                    }
                }

                ++second;
            }
        }

        ++first;
    }
    return possible;
}

/**
 * @see possible
 * @param current_length The sum of all pipe lengths tested in this branch
 *                      of the recursion tree.
 */
bool possible_rec(int x, LengthsArray lengths, CountsArray counts, int n, int current_length)
{
    // For a note on the recursive version, have
    // a look at the documentation.
    if (current_length == x) {
        return true;
    } else if (current_length > x) {
        return false;
    }

    int i = 0;
    bool possible = false;
    while (i < n && !possible) {
        // if there are more pipes with
        // the "current" length (lengths[i])
        if (counts[i] > 0) {
            // use it
            --counts[i];
            possible = possible_rec(x, lengths, counts, n, current_length + lengths[i]);
            // if that pipe didn't work,
            if (!possible) {
                // "un-use" (/ backtrack) it
                ++counts[i];
            }
        }
        ++i;
    }
    return possible;
}

/**
 * @see possible
 * @param current_length The sum of all pipe lengths tested in this branch
 *                      of the recursion tree.
 */
bool possible_bt(int x, LengthsArray lengths, CountsArray counts, int n, int current_length)
{
    int i = 0;
    bool possible = false;
    while (i < n && !possible) {
        // if there are more pipes with
        // the "current" length (lengths[i])

        if (current_length == x) {
            return true;
        }

        if (counts[i] > 0) {
            // use it
            --counts[i];
            possible = possible_bt(x, lengths, counts, n, current_length + lengths[i]);
            // if that pipe didn't work,
            if (!possible) {
                // "un-use" (/ backtrack) it
                ++counts[i];
            }
        }
        ++i;
    }

    return possible;
}

/**
 * @param x The total length the pipes should add up to.
 * @param lengths The array of available pipe lengths.
 * @param counts The amount of pipes available for a specific length.
 * @param n The number of pipe-lengths. (Also used to index counts to
 *          retrieve the number of available pipes per length.)
 * @param f The function that should be used for checking the possibilities.
 * @return True if the function f found a way to combine the available (counts)
 *          pipelengths (lenghts) so that the total length is x.
 */
bool possible(int x, LengthsArray lengths, int *const counts, int n, SolutionFunction f)
{
    // Implementation note: I am not using the original counts array
    // since the solution function f mutates it. To avoid a potential
    // source of errors, I decided to use a copy. That way users will
    // not have to take the mutation into account when using this function.

    // Also using possible_it is potentially dangerous since its interface differs
    // from the one typedef'd as SolutionFunction. I would not do this (and also not
    // recommend doing so) in real world applications. Because, however, the only
    // goal of this exercise is to demonstrate different implementations, I'd say
    // it's fine.
    int *counts_cpy = (int *) malloc(sizeof(int) * (size_t) n);
    for (int i = 0; i < n; ++i) {
        counts_cpy[i] = counts[i];
    }
    bool possible = f(x, lengths, counts, n, 0);
    free(counts_cpy);
    return possible;
}

/*
 *
 * Tests
 *
 */

void assertBool(bool expected, bool got, const char *name)
{
    bool success = expected == got;
    printf("@\tTest \"%s\" %s.", name, success ? "succeeded" : "failed");
    if (!success) {
        printf(" Expected %d, got %d.", expected, got);
    }
    printf("\n");
}

#define INT_ARR(x) (int[x])

static time_t timer;

void peak()
{
    time_t diff = clock() - timer;
    printf("Time passed: %.2fms\n", ((float) diff) / CLOCKS_PER_SEC * 1000);
    timer = clock();
}

void test_it()
{
    timer = clock();
    /* Possible */
    // ugly (and unsafe!) typecasts, see implementation note in function "possible"!
    assertBool(true, possible(4,
                              INT_ARR(1) {1},
                              INT_ARR(1) {5}, 1, (SolutionFunction) possible_it),
               "Possible for 1 using iteration");
    peak();
    assertBool(true, possible(3,
                              INT_ARR(2) {1, 2},
                              INT_ARR(2) {2, 1}, 2, (SolutionFunction) possible_it),
               "Possible for 2 using iteration");
    peak();
    assertBool(true, possible(6,
                              INT_ARR(3) {1, 2, 3},
                              INT_ARR(3) {1, 1, 1}, 3, (SolutionFunction) possible_it),
               "Possible for 3 using iteration");
    peak();

    /* n > 3 */
    assertBool(true, possible(10,
                              INT_ARR(4) {1, 2, 3, 99},
                              INT_ARR(4) {1, 1, 3, 99}, 4, (SolutionFunction) possible_it),
               "Possible for 4 using iteration");
    peak();

    /* Impossible */
    assertBool(false, possible(10,
                               INT_ARR(1) {1},
                               INT_ARR(1) {5}, 1, (SolutionFunction) possible_it),
               "Impossible for 1 using iteration");
    peak();
    assertBool(false, possible(10,
                               INT_ARR(2) {1, 2},
                               INT_ARR(2) {2, 1}, 2, (SolutionFunction) possible_it),
               "Impossible for 2 using iteration");
    peak();
    assertBool(false, possible(10,
                               INT_ARR(3) {1, 2, 3},
                               INT_ARR(3) {1, 1, 1}, 3, (SolutionFunction) possible_it),
               "Impossible for 3 using iteration");
    peak();

    /* n > 4 */
    assertBool(false, possible(7,
                               INT_ARR(5) {1, 2, 3, 4, 5},
                               INT_ARR(5) {2, 0, 1, 1, 0}, 5, (SolutionFunction) possible_it),
               "Impossible for 4 using iteration");
    peak();
}

void test_rec()
{
    timer = clock();
    /* Possible */
    assertBool(true, possible(7,
                              INT_ARR(5) {1, 2, 3, 4, 5},
                              INT_ARR(5) {2, 0, 1, 1, 0}, 5, possible_rec),
               "Possible for 5 using recursion");
    peak();
    assertBool(true, possible(35,
                              INT_ARR(7) {1, 2, 3, 4, 5, 10, 15},
                              INT_ARR(7) {2, 0, 1, 1, 0, 1, 2}, 7, possible_rec),
               "Possible for 7 using recursion");
    peak();
    assertBool(true, possible(55,
                              INT_ARR(10) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                              INT_ARR(10) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, possible_rec),
               "Possible for 10 using recursion");
    peak();

    /* Impossible */
    assertBool(false, possible(10,
                               INT_ARR(5) {1, 2, 3, 4, 5},
                               INT_ARR(5) {2, 0, 1, 1, 0}, 5, possible_rec),
               "Impossible for 5 using recursion");
    peak();
    assertBool(false, possible(100,
                               INT_ARR(7) {1, 2, 3, 4, 5, 10, 15},
                               INT_ARR(7) {2, 0, 1, 1, 0, 1, 2}, 7, possible_rec),
               "Impossible for 7 using recursion");
    peak();
    assertBool(false, possible(56,
                               INT_ARR(10) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                               INT_ARR(10) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, possible_rec),
               "Impossible for 10 using recursion");
    peak();
}

void test_bt()
{
    timer = clock();
    /* Possible */
    assertBool(true, possible(7,
                              INT_ARR(5) {1, 2, 3, 4, 5},
                              INT_ARR(5) {2, 0, 1, 1, 0}, 5, possible_bt),
               "Possible for 5 using backtracking");
    peak();
    assertBool(true, possible(35,
                              INT_ARR(7) {1, 2, 3, 4, 5, 10, 15},
                              INT_ARR(7) {2, 0, 1, 1, 0, 1, 2}, 7, possible_bt),
               "Possible for 7 using backtracking");
    peak();
    assertBool(true, possible(55,
                              INT_ARR(10) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                              INT_ARR(10) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, possible_bt),
               "Possible for 10 using backtracking");
    peak();

    /* Impossible */
    assertBool(false, possible(10,
                               INT_ARR(5) {1, 2, 3, 4, 5},
                               INT_ARR(5) {2, 0, 1, 1, 0}, 5, possible_bt),
               "Impossible for 5 using backtracking");
    peak();
    assertBool(false, possible(100,
                               INT_ARR(7) {1, 2, 3, 4, 5, 10, 15},
                               INT_ARR(7) {2, 0, 1, 1, 0, 1, 2}, 7, possible_bt),
               "Impossible for 7 using backtracking");
    peak();
    assertBool(false, possible(56,
                               INT_ARR(10) {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                               INT_ARR(10) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, possible_bt),
               "Impossible for 10 using backtracking");
    peak();
}

int main()
{
    test_it();
    test_rec();
    test_bt();
    return EXIT_SUCCESS;
}
