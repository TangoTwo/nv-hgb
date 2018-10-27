#include "SudokuField.h"

size_t field_to_index(const SudokuField *const f)
{
    return f->row * 9 + f->col;
}

SudokuField index_to_field(size_t index)
{
    SudokuField f;
    f.row = index / 9;
    f.col = index % 9;
    return f;
}

SudokuField next_field(const SudokuField *const prev)
{
    SudokuField next;
    bool wraps = prev->col == 8;
    next.row = wraps ? prev->row + 1 : prev->row;
    next.col = wraps ? 0 : prev->col + 1;
    return next;
}

bool is_last_field(const SudokuField *const field)
{
    return field->row == 8 && field->col == 8;
}
