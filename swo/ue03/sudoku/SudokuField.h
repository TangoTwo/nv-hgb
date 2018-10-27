#ifndef UE03_SUDOKUFIELD_H
#define UE03_SUDOKUFIELD_H

#include <stdlib.h>

/**
 * A neat lil bool.
 */
typedef enum
{
    false, true
} bool;

/**
 * A 2D point representing one field in a sudoku.
 */
typedef struct
{
    size_t row;
    size_t col;
} SudokuField;

/**
 * @param f The field to project.
 * @return The 2D index (on a 9*9 basis) projected
 * onto 1D array.
 */
size_t field_to_index(const SudokuField *const f);

/**
 * @param index The index to project.
 * @return The 1D index projected onto a 9 * 9
 *          matrix.
 */
SudokuField index_to_field(size_t index);

/**
 * @param prev The predecessor to the desired field.
 * @return The field to the right of the supplied field
 *          if the column < 8 or the first column in the
 *          next row if the column = 8.
 */
SudokuField next_field(const SudokuField *const prev);

/**
 * @param field The field to check.
 * @return True if the row AND the column are equal to 8.
 */
bool is_last_field(const SudokuField *const field);

#endif //UE03_SUDOKUFIELD_H
