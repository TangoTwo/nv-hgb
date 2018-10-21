#include <stdlib.h>
#include <stdio.h>

#define MAX 100

void merge(int *const a, int a_n, int *const b, int b_n)
{
    int *merged = (int *) malloc(sizeof(int) * (size_t) (a_n + b_n));
    size_t merged_ind = 0;
    int i = 0;
    int j = 0;
    while (i < a_n && j < b_n) {
        if (a[i] > b[j]) {
            merged[merged_ind] = b[j++];
        } else {
            merged[merged_ind] = a[i++];
        }
        ++merged_ind;
    }

    while (i < a_n) {
        merged[merged_ind++] = a[i++];
    }

    while (j < b_n) {
        merged[merged_ind++] = b[j++];
    }

    for (size_t override = 0; override < merged_ind; ++override) {
        a[override] = merged[override];
    }

    free(merged);
}

void merge_sort(int a[], int n)
{
    if (n != 1) {
        int *const a_left = a;
        int left = n / 2;

        int *const a_right = a + (n / 2);
        int right = n - left;

        merge_sort(a_left, left);
        merge_sort(a_right, right);

        merge(a_left, left, a_right, right);
    }
}

int main(int argc, char *argv[])
{
    int n = 0;
    int a[MAX] = {0};

    if (argc < 2) {
        printf("What am I supposed to sort?\n");
        return EXIT_FAILURE;
    }

    if (argc > MAX) {
        printf("Cannot sort so many numbers. Enter a max of %d values.\n", MAX);
        return EXIT_FAILURE;
    }

    int success = 1;
    while (n < argc - 1 && n < MAX && success) {
        success = sscanf(argv[n + 1], "%d", a + n); // NOLINT(cert-err34-c)
        ++n;
    }

    if (!success) {
        printf("\"%s\" is not an integer number.\n", argv[n]);
        return EXIT_FAILURE;
    }

    printf("Unsorted: ");
    for (int i = 0; i < n; ++i) {
        printf("%d", a[i]);
        if (i + 1 < n) {
            printf(", ");
        }
    }
    printf("\n");

    merge_sort(a, n);

    printf("Sorted: ");
    for (int i = 0; i < n; ++i) {
        printf("%d", a[i]);
        if (i + 1 < n) {
            printf(", ");
        }
    }

    return EXIT_SUCCESS;
}