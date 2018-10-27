#include <stdio.h>
#include "Sudoku.h"

void set_solution(Possibilities possibles, const SudokuField *const sudoku_field, size_t solution)
{
    for (size_t i = 0; i < 9; ++i) {
        possibles[sudoku_field->row][sudoku_field->col][i] = false;
    }
    possibles[sudoku_field->row][sudoku_field->col][solution - 1] = true;
}

size_t first_possible_value(Possibilities possibles, const SudokuField *const sudoku_field)
{
    size_t first_possible_found = false;
    size_t i = 0;
    while (i < 9 && !first_possible_found) {
        first_possible_found = possibles[sudoku_field->row][sudoku_field->col][i];
        ++i;
    }
    return i;
}

bool field_is_solved(Possibilities possibles, const SudokuField *const sudoku_field)
{
    size_t i = first_possible_value(possibles, sudoku_field);
    bool solved = i <= 9;
    while (i < 9 && solved) {
        solved = !possibles[sudoku_field->row][sudoku_field->col][i];
        ++i;
    }
    return solved;
}

size_t get_solution(Possibilities possibles, const SudokuField *const sudoku_field)
{
    return field_is_solved(possibles, sudoku_field) ?
           first_possible_value(possibles, sudoku_field) : 0;
}

void reset_field(Possibilities possibles, SudokuField *const sudoku_field)
{
    for (size_t i = 0; i < 9; ++i) {
        possibles[sudoku_field->row][sudoku_field->col][i] = true;
    }
}

bool is_fixed(Possibilities possibles, SudokuField *const sudoku_field)
{
    return possibles[sudoku_field->row][sudoku_field->col][9];
}

bool ***create_possibles(const int *const arr)
{
    // create row pointers
    bool ***possibles = (bool ***) malloc(sizeof(bool **) * 9);

    for (size_t row = 0; row < 9; ++row) {

        // create column pointers
        bool **cols = (bool **) malloc(sizeof(bool *) * 9);
        possibles[row] = cols;

        for (size_t col = 0; col < 9; ++col) {

            SudokuField current;
            current.row = row;
            current.col = col;

            // create array of possibilities + const flag
            possibles[row][col] = (bool *) malloc(sizeof(bool) * 10);

            size_t index = field_to_index(&current);
            // the 10th element in the array is a virtual const flag;
            // if it is set to true, the element must no be changed
            possibles[row][col][9] = arr[index] != 0;
            if (is_fixed(possibles, &current)) {
                set_solution(possibles, &current, (size_t) arr[index]);
            } else {
                reset_field(possibles, &current);
            }
        }
    }
    return possibles;
}

void delete_possibles(bool ***possibles)
{
    for (size_t row = 0; row < 9; ++row) {
        for (size_t col = 0; col < 9; ++col){
            free(possibles[row][col]);
        }
        free(possibles[row]);
    }
    free(possibles);
}

size_t each_column_in_row(size_t row, size_t i)
{
    return row * 9 + i;
}

size_t each_row_in_column(size_t col, size_t i)
{
    return i * 9 + col;
}

size_t each_field_in_box(size_t box_index, size_t i)
{
    // absolute box position within the sudoku
    size_t box_row = box_index / 3;
    size_t box_col = box_index % 3;

    // currently processed item within the box
    size_t row_within_box = i / 3;
    size_t col_within_box = i % 3;

    // absolute indices
    size_t row = box_row * 3 + row_within_box;
    size_t col = box_col * 3 + col_within_box;
    return row * 9 + col;
}

bool check(Possibilities possibles, size_t value, IndexCalculationFunction calc_index, size_t item)
{
    bool possible = true;
    size_t i = 0;
    while (i < 9 && possible) {
        SudokuField current = index_to_field(calc_index(item, i));
        possible =
                !(field_is_solved(possibles, &current) &&
                  (get_solution(possibles, &current) == value));
        ++i;
    }
    return possible;
}

bool is_partial_solution(Possibilities possibles, SudokuField sudoku_field, size_t solution)
{
    size_t box_row = sudoku_field.row / 3;
    size_t box_col = sudoku_field.col / 3; // intended division, NO MODULO!
    size_t box_index = box_row * 3 + box_col;
    return check(possibles, solution, each_column_in_row, sudoku_field.row) &&
           check(possibles, solution, each_row_in_column, sudoku_field.col) &&
           check(possibles, solution, each_field_in_box, box_index);
}

void solve_final(Possibilities possibles,  int *const dest)
{
    for (size_t i = 0; i < 81; ++i) {
        SudokuField current = index_to_field(i);
        dest[i] = (int) get_solution(possibles, &current);
    }
}

void backtrace_solution(Possibilities possibles, SudokuField sudoku_field, int *const final_solution)
{
    if (is_fixed(possibles, &sudoku_field)) {
        // if the field is fixed, we can skip it
        if (is_last_field(&sudoku_field)) {
            solve_final(possibles, final_solution);
        } else {
            backtrace_solution(possibles, next_field(&sudoku_field), final_solution);
        }
    } else {
        // otherwise we have to check all numbers
        for (size_t i = 1; i < 10; ++i) {

            // if we found a possible solution
            if (is_partial_solution(possibles, sudoku_field, i)) {
                // mark it as such
                set_solution(possibles, &sudoku_field, i);

                // if this was the last field, we are done
                if (is_last_field(&sudoku_field)) {
                    solve_final(possibles, final_solution);
                    return;
                }

                // otherwise we keep searching
                backtrace_solution(possibles, next_field(&sudoku_field), final_solution);
                // and backtrace this boe
                reset_field(possibles, &sudoku_field);
            }
        }
    }
}

void print_sudoku(const int *const sudoku)
{
    printf("===============================\n");
    for (size_t row = 0; row < 9; ++row) {
        for (size_t col = 0; col < 9; ++col) {
            printf("%d, ", sudoku[row * 9 + col]);
            if ((col + 1) % 3 == 0) {
                printf("\t");
            }
        }
        printf("\n");
        if ((row + 1) % 3 == 0) {
            printf("\n");
        }
    }
}

void sudoku(int *squares)
{
    bool ***possibles = create_possibles(squares);
    backtrace_solution(possibles, (SudokuField) {0, 0}, squares);
    delete_possibles(possibles);
}
