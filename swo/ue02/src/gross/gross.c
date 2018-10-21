#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// region merge sort

void merge(int *const a, int a_n, const int *const b, int b_n)
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

// endregion

// region quickselect

int partition(int *const a, int left, int right, int pivot_index)
{
    int pivot_value = a[pivot_index];
    int buff = a[pivot_index];
    a[pivot_index] = a[right];
    a[right] = buff;
    int store_index = left;
    for (int i = left; i < right; ++i) {
        if (a[i] < pivot_value) {
            buff = a[store_index];
            a[store_index] = a[i];
            a[i] = buff;
            ++store_index;
        }
    }
    buff = a[right];
    a[right] = a[store_index];
    a[store_index] = buff;
    return store_index;
}

int select(int *const a, int left, int right, int k)
{
    if (left == right) {
        return a[left];
    }
    int pivot_index = left + (int) floor(rand() % (right - left + 1));
    pivot_index = partition(a, left, right, pivot_index);
    if (k == pivot_index) {
        return a[k];
    } else if (k < pivot_index) {
        return select(a, left, pivot_index - 1, k);
    } else {
        return select(a, pivot_index + 1, right, k);
    }
}

// endregion

// region exercises

/**
 * Finds the second largest number using
 * linear search.
 */
int second_largest(const int *const a , int n)
{
    int max = INT_MIN;
    int second_to_max = INT_MIN;
    for (int i = 0; i < n; ++i) {
        int current = a[i];
        if (current > second_to_max) {
            if (current > max) {
                second_to_max = max;
                max = current;
            } else {
                second_to_max = current;
            }
        }
    }
    return second_to_max;
}

/**
 * Finds the <i>i</i>th-largest number
 * by sorting the array using the merge
 * sort alrorithm first, then indexes
 * using <i>i</i>.
 */
int ith_largest_1(int *const a, int n, int i)
{
    merge_sort(a, n);
    return a[n - i];
}

/**
 * Finds the <i>i</i>th-largest number
 * using the quickselect algorithm.
 */
int ith_largest_2(int *const a, int n, int i)
{
    return select(a, 0, n-1, n - i);
}

// endregion

int main()
{
    int arr[10] = {98, 4, 3, 80, 2, 9, -3, 10, 6, 99};
    printf("Second largest: %d\n", second_largest(arr, 10));
    printf("4th largest: %d\n", ith_largest_1(arr, 10, 4));
    printf("3rd largest: %d\n", ith_largest_2(arr, 10, 3));
}
