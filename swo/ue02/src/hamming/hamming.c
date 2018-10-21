#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define HAMMING_SEQ_ARRAY_LENGTH 1000

/**
 * @param exp_2 The exponent for the component 2.
 * @param exp_3 The exponent for the component 3.
 * @param exp_5 The exponent for the component 5.
 * @return The hamming number according to the exponents
 *          following the forumla 2<sup>exp_2</sup> *
 *          3<sup>exp_3</sup> * 5<sup>exp_5</sup>.
 */
unsigned int calc_hamming(int exp_2, int exp_3, int exp_5)
{
    return (unsigned int) (pow(2, exp_2) * pow(3, exp_3) * pow(5, exp_5));
}

/**
 * @param p The void pointer to cast.
 * @return The int value pointed to by <i>p</i>.
 */
int cast_int(const void *const p)
{
    return *(int*)p;
}

/**
 * A binary comparison "predicate" for integer numbers.
 */
int compare_int(const void *a, const void *b)
{
    if (cast_int(a) < cast_int(b)) {
        return -1;
    } else if (cast_int(a) > cast_int(b)) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    clock_t t;
    t = clock();

    if (argc != 2) {
        printf("Invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    unsigned int z = (unsigned int) atoi(argv[1]); // NOLINT(cert-err34-c)
    if (z <= 0 || z > INT_MAX) {
        printf("The upper bound must be an integer number x where 0 < x < %d.", INT_MAX);
        return EXIT_FAILURE;
    }

    // The array is large enough to
    // hold all hamming numbers up to
    // INT_MAX
    unsigned int hamming_numbers[HAMMING_SEQ_ARRAY_LENGTH];
    size_t last = 0;
    int exp_2 = 0, exp_3 = 0, exp_5 = 0;
    unsigned int current = 0;

    // calculate all numbers in the sequence
    // and add them to the values array
    while (exp_2 < 10) {
        exp_3 = 0;
        while (exp_3 < 10) {
            exp_5 = 0;
            current = 0;
            // <last> will always be less than HAMMING_SEQ.., just making sure
            // not to segfault in case I am in error.
            while (current < z &&  exp_5 < 10 && last < HAMMING_SEQ_ARRAY_LENGTH) {
                current = calc_hamming(exp_2, exp_3, exp_5++);
                if (current < z) {
                    hamming_numbers[last++] = current;
                }
            }
            ++exp_3;
        }
        ++exp_2;
    }

    qsort(hamming_numbers, last, sizeof(int), compare_int);

    for (size_t i = 0; i < last; ++i) {
        printf("%d", hamming_numbers[i]);
        if (i + 1 < last) {
            printf(", ");
        }
    }
    printf("\n");

    t = clock() - t;
    printf("Time passed: %.2fms", ((float) t) / CLOCKS_PER_SEC * 1000);

    return EXIT_SUCCESS;
}
