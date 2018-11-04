#include <stdio.h>
#include "unic.h"

#define MAX 100

int max(int a, int b)
{
    return a > b ? a : b;
}

int longest_increasing_run(int const s[], int const n)
{
    int last_max_run = 0;
    int curr_run = 1;
    int last_num = s[0];

    for (int i = 0; i < n; ++i) {
        if (last_num < s[i]) {
            // run still going
            ++curr_run;
        } else {
            // run ended
            if (last_max_run < curr_run) {
                // run was longer than previous max run
                last_max_run = curr_run;
            }
            curr_run = 1;
        }
        last_num = s[i];
    }

    // in case the sequence ended with a run
    if (curr_run > last_max_run) {
        last_max_run = curr_run;
    }

    return last_max_run;
}

int longest_increasing_subsequence(int const s[], int const n)
{
    int l[MAX] = {0};
    int p[MAX] = {0};
    for (int i = 0; i < n; ++i) {
        p[i] = -1;
    }
    int max_seq = 0;
    for (int i = 0; i < n; ++i) {
        max_seq = 0;
        for (int j = 1; j < i; ++j) {
            if (s[j] < s[i]) {
                // find the max value of l[0,i - 1]
                max_seq = max(l[j], l[j - 1]);
                if (p[i] == -1) {
                    // if the new p is still unset, set it
                    p[i] = j;
                } else if (p[j] > p[p[i]]) {
                    // else check if the current (j) p is larger
                    // than the p previously stored in p[i]
                    p[i] = j;
                }
            }
        }
        // add + 1 according to formula
        l[i] = max_seq + 1;
    }

    // stores the index of the end of the max sequence
    int seq_end = 0;
    for (int i = 0; i < n; ++i) {
        if (l[i] > max_seq) {
            // also find the longest sequence itself and store its length
            max_seq = l[i];
            seq_end = i;
        }
    }

    // now insert it by traversing p in reverse order
    int *ordered_max_seq = (int *) malloc(sizeof(int) * (size_t) max_seq);
    int reverse_index = max_seq - 1;
    while (seq_end != -1) {
        ordered_max_seq[reverse_index] = s[seq_end];
        --reverse_index;
        seq_end = p[seq_end];
    }

    // print dat sequence
    printf("Max sequence: ");
    for (int i = 0; i < max_seq; ++i) {
        printf("%d,", ordered_max_seq[i]);
    }
    printf("\n");
    free(ordered_max_seq);

    return max_seq;
}

#define INT_ARR(x) (int[x])

int main()
{
    unic_init();
    unic_ass_eq_i(3, longest_increasing_run(INT_ARR(6) {6, 7, 8, 3, 2, 1}, 6), "Sequence starting with one LIR");
    unic_ass_eq_i(3, longest_increasing_run(INT_ARR(5) {3, 2, 1, 4, 8}, 5), "Sequence ending with one LIR");
    unic_ass_eq_i(4, longest_increasing_run(INT_ARR(7) {10, 1, 2, 3, 4, 2}, 6), "Sequence with one LIR in the middle");
    unic_ass_eq_i(7, longest_increasing_run(INT_ARR(7) {1, 2, 3, 4, 5, 6, 7}, 7), "Sequence with one LIR = n");
    unic_ass_eq_i(2, longest_increasing_run(INT_ARR(5) {5, 6, 2, 1, 2}, 5), "Sequence with two equally long runs");
    unic_ass_eq_i(3, longest_increasing_run(INT_ARR(5) {5, 6, 7, 1, 2}, 5), "Sequence with two runs of diff. sizes");

    unic_ass_eq_i(3, longest_increasing_subsequence(INT_ARR(9) {9, 5, 2, 8, 7, 3, 1, 6, 4}, 9), "Exercise");
    unic_ass_eq_i(3, longest_increasing_subsequence(INT_ARR(7) {4, 7, 3, 1, 5, 9, 6 }, 7), "Sequence with three LISs");
    unic_ass_eq_i(3, longest_increasing_subsequence(INT_ARR(5) {4, 1, 7, 2, 5}, 5), "Sequence with one LISs");
    unic_ass_eq_i(3, longest_increasing_subsequence(INT_ARR(5) {8, 1, 3, 6, 2}, 5), "Sequence with one LISs (adjacent elements)");
    return unic_get_results();
}
