/*
 * A quick note to this head file:
 * Usually you would want to hide functions you don't
 * want to expose to the user. However I am pretty sure
 * whoever wants to dig into the code will be thankful to
 * find the entire documentation in the header file.
 */
#ifndef UE03_SUDOKU_H
#define UE03_SUDOKU_H

#include <stdlib.h>
#include "SudokuField.h"

/**
 * A 3D Array holding all possibilities.
 */
typedef bool * *const *const Possibilities;

/**
 * A function used to calculate an index given a fixed
 * item and an item changing every iteration.
 * @see check
 */
typedef size_t (*IndexCalculationFunction)(size_t, size_t);

/**
 * Applies the supplied solution to a specified field in the sudoku.
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The sudoku field to mark as solved.
 * @param solution The value with which to fill the sudoku field.
 */
void set_solution(Possibilities possibles, const SudokuField *const sudoku_field, size_t solution);

/**
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The field to get the value for.
 * @return The first possible value for the specified field.
 */
size_t first_possible_value(Possibilities possibles, const SudokuField *const sudoku_field);

/**
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The field to check.
 * @return True if the specified field already has a definite solution.
 */
bool field_is_solved(Possibilities possibles, const SudokuField *const sudoku_field);

/**
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The sudoku field for which to return the currently
 *          set solution.
 * @return The solution for the specified sudoku field.
 */
size_t get_solution(Possibilities possibles, const SudokuField *const sudoku_field);

/**
 * Sets all possibilities to true, meaning that for the specified
 * field, all numbers will be a possible solution.
 * @param possibles The array of possibilities for the solution.
 * @param sudoku_field The field to reset.
 */
void reset_field(Possibilities possibles, SudokuField *const sudoku_field);

/**
 * @param possibles The array of possibilities for the array.
 * @param sudoku_field The field to check.
 * @return True if the value for the specified field must not be changed.
 */
bool is_fixed(Possibilities possibles, SudokuField *const sudoku_field);

/**
 * @param arr The sudoku.
 * @return An array of possibilities for a given soduko.
 */
bool ***create_possibles(const int *const arr);

/**
 * Frees the memory allocated for the array of possible sudoku solutions.
 * @param possibles The array of possible solutions.
 */
void delete_possibles(bool ***possibles);

/**
 * A function returning the index for each column in a specified row.
 * @param row The row to inspect.
 * @param i The iterating value.
 * @see check
 */
size_t each_column_in_row(size_t row, size_t i);

/**
 * A function returning the index for each row in a specified column.
 * @param col The column to inspect.
 * @param i The iterating value.
 * @see check
 */
size_t each_row_in_column(size_t col, size_t i);

/**
 * A function returning the index for each item in a specified
 * sudoku 3*3 box.
 * @param box_index The index of the box to inspect.
 * @param i The iterating value.
 * @see check
 */
size_t each_field_in_box(size_t box_index, size_t i);

/**
 * Checks whether the supplied <i>value</i> is a solution in the area
 * covered by the indices produced by the function <i>calc_index</i>.
 * To check a whether a column x contains <i>value</i>,
 * call <code>check(possibles, value, each_row_in_column, x)</code>.
 * @param possibles The array of possibilities for the sudoku.
 * @param value The value for which to check whether it's a potential solution.
 * @param calc_index The function for calulating an index into the possibilities.
 *                      Use either <i>each_row_in_column</i>, <i>each_column_in_cow</i>
 *                      or <i>each_field_in_box</i>.
 * @param item The {item}th row, column or box will be checked.
 */
bool check(Possibilities possibles, size_t value, IndexCalculationFunction calc_index, size_t item);

/**
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The field for which to check whether the solution is valid.
 * @param solution The solution to check.
 * @return True if the supplied solution is a valid solution for the
 *          specified sudoku index using the array of possibilites to
 *          check its validity.
 */
bool is_partial_solution(Possibilities possibles, SudokuField sudoku_field, size_t solution);

/**
 * Solves the sudoku represented by the supplied possibilities using recursive backtracking.
 * @param possibles The array of possibilities for the sudoku.
 * @param sudoku_field The field to process.
 * @param final_solution The original sudoku which will be filled with the final solution.
 */
void backtrace_solution(Possibilities possibles, SudokuField sudoku_field, int *const final_solution);

/**
 * Prints the supplied sudoku to the standard output
 * in a formatted manner.
 * @param sudoku The sudoku to print.
 */
void print_sudoku(const int *const sudoku);

/**
 * The entry point to the sudoku solving module.
 * @param squares An array using a row-major layout for the sudoku.
 */
void sudoku(int squares[]);

#endif //UE03_SUDOKU_H
