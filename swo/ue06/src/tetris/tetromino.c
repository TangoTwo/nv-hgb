#include "tetromino.h"
#include <assert.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/**
 * This is a highly inflexible macro
 * which one should never use in larger
 * scale projects. But since this is a
 * small exercise, I use it because I
 * am lazy and I think it improves
 * readability.
 */
#define BOOL_MASK(dim, col) { \
		ROTATED_DEFAULT, \
		{0, 0}, \
        (col), \
        (dim), \
		(bool[(dim)*(dim)]) {
#define END_MASK }}

/**
 * @ @ @ @
 */
static const tetromino_t _I = BOOL_MASK(4, color_red)
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0
END_MASK;

/**
 *   @
 *   @
 * @ @
 */
static tetromino_t _J = BOOL_MASK(3, color_green)
    0, 1, 0,
    0, 1, 0,
    1, 1, 0
END_MASK;

/**
 * @
 * @
 * @ @
 */
static tetromino_t _L = BOOL_MASK(3, color_blue)
    0, 1, 0,
    0, 1, 0,
    0, 1, 1
END_MASK;

/**
 * @ @
 * @ @
 */
static tetromino_t _O = BOOL_MASK(2, color_yellow)
    1, 1,
    1, 1
END_MASK;

/**
 *   @ @
 * @ @
 */
static tetromino_t _S = BOOL_MASK(3, color_magenta)
    0, 1, 1,
    1, 1, 0,
    0, 0, 0
END_MASK;

/**
 * @ @ @
 *   @
 */
static tetromino_t _T = BOOL_MASK(3, color_cyan)
    0, 0, 0,
    1, 1, 1,
    0, 1, 0
END_MASK;

/**
 * @ @
 *   @ @
 */
static tetromino_t _Z = BOOL_MASK(3, color_white)
    1, 1, 0,
    0, 1, 1,
    0, 0, 0
END_MASK;

/**
 * The tetromino returned for invalid
 * requests.
 */
static tetromino_t _Invalid = {ROTATED_DEFAULT, {0, 0}, color_black, 0, NULL};

/**
 * A function that maps a row and a column to an index in
 * the matrix. By modifying the mapping those functions
 * can rotate indices.
 */
typedef int (*rotation_func)(int side_len, int row, int col);

/**
 * @param tetromino The tetromino for which to find
 *          a rotation function
 * @return The function representing a rotation
 *          that corresponds with the tetrominos
 *          orientation.
 */
static rotation_func get_rotation_func(const tetromino_t *tetromino);

// todo document

/**
 * Renders a tetromino tile to the current OpenGL context.
 * @param pos The position at which to draw the tile.
 * @param color The color to use for the tile.
 */
static void render_quad(const position_t * pos, const color_t * color);

/**
 * @param side_len The side length of the matrix.
 * @param row The row of the element to fetch.
 * @param col The column of the element to fetch.
 * @return A value from the matrix, reading it
 *          from the top left to the bottom right.
 */
static int rotate_none(int side_len, int row, int col);

/**
 * @param side_len The side length of the matrix.
 * @param row The row of the element to fetch.
 * @param col The column of the element to fetch.
 * @return A value from the matrix, reading it
 *          from the bottom left to the top right.
 */
static int rotate_right(int side_len, int row, int col);

/**
 * @param side_len The side length of the matrix.
 * @param row The row of the element to fetch.
 * @param col The column of the element to fetch.
 * @return A value from the matrix, reading it
 *          from the top right to the bottom left.
 *          (Basically reverse)
 */
static int rotate_half(int side_len, int row, int col);

/**
 * @param side_len The side length of the matrix.
 * @param row The row of the element to fetch.
 * @param col The column of the element to fetch.
 * @return A value from the matrix, reading it
 *          from the top left to the bottom right.
 */
static int rotate_left(int side_len, int row, int col);

/**
 * Rotates the supplied tetromino clockwise.
 * @param tetromino The tetromino to rotate.
 */
static void rotate_cw(tetromino_t *tetromino);

/**
 * Rotates the supplied tetromino counter clockwise.
 * @param tetromino The tetromino to rotate.
 */
static void rotate_ccw(tetromino_t *tetromino);

/**
 * @param tetromino The tetromino to clone.
 * @return A deep clone of the supplied tetromino.
 */
static tetromino_t clone_tetromino(const tetromino_t *tetromino);

tetromino_t create_tetromino(tetromino_shape shape)
{
    switch (shape) {
        case I: return clone_tetromino(&_I);
        case J: return clone_tetromino(&_J);
        case L: return clone_tetromino(&_L);
        case O: return clone_tetromino(&_O);
        case S: return clone_tetromino(&_S);
        case T: return clone_tetromino(&_T);
        case Z: return clone_tetromino(&_Z);
        default:
            printf("Unknown tetromino: %d", shape);
            return _Invalid;
    }
}

void render_tetromino(const tetromino_t *tetromino)
{
    if (tetromino != NULL) {
        int len = tetromino->side_len;
        for (int row = 0; row < len; ++row) {
            for (int col = 0; col < len; ++col) {
                if (get_tetromino_mask_at(tetromino, row, col)) {
                    position_t relative = {
                            tetromino->pos.x + col,
                            tetromino->pos.y + row
                    };
                    render_quad(&relative, &tetromino->color);
                }
            }
        }
    }
}

void rotate_tetromino(tetromino_t *tetromino, rotation_t rotation)
{
    if (tetromino != NULL) {
        switch (rotation) {
            case CLOCKWISE:
                rotate_cw(tetromino);
                break;
            case COUNTER_CLOCKWISE:
                rotate_ccw(tetromino);
                break;
        }
    }
}

int real_height(const tetromino_t *tetromino)
{
    int height = 0;
    if (tetromino != NULL) {
        int row = 0;
        while (row < tetromino->side_len) {
            int col = 0;
            while (col < tetromino->side_len && !get_tetromino_mask_at(tetromino, row, col)) {
                ++col;
            }
            if (col < tetromino->side_len) {
                height = row + 1;
            }
            ++row;
        }
    }
    return height;
}

bool get_tetromino_mask_at(const tetromino_t *tetromino, int row, int col)
{
    bool mask = false;
    if (tetromino != NULL) {
        rotation_func rotate = get_rotation_func(tetromino);
        assert(rotate);
        mask = tetromino->mask[rotate(tetromino->side_len, row, col)];
    }
    return mask;
}

void remove_tetromino_row(const tetromino_t *tetromino, int row)
{
    if (tetromino != NULL && row >= 0 && row < tetromino->side_len) {
        rotation_func rotate = get_rotation_func(tetromino);
        assert(rotate);
        // shift all rows below <row> up by 1
        while (row < tetromino->side_len - 1) {
            for (int col = 0; col < tetromino->side_len; ++col) {
                tetromino->mask[rotate(tetromino->side_len, row, col)] =
                        get_tetromino_mask_at(tetromino, row + 1, col);
            }
            ++row;
        }

        // clear last row
        for (int col = 0; col < tetromino->side_len; ++col) {
            tetromino->mask[rotate(tetromino->side_len, tetromino->side_len - 1, col)] = false;
        }
    }
}

int left_offset(const tetromino_t *tetromino)
{
    // delegate work to real_height
    int offset = 0;
    if (tetromino != NULL) {
        tetromino_t cpy = *tetromino;
        rotate_tetromino(&cpy, COUNTER_CLOCKWISE);
        offset = tetromino->side_len - real_height(&cpy);
    }
    return offset;
}

int right_offset(const tetromino_t *tetromino)
{
    // delegate work to real_height
    int offset = 0;
    if (tetromino != NULL) {
        tetromino_t cpy = *tetromino;
        rotate_tetromino(&cpy, CLOCKWISE);
        offset = tetromino->side_len - real_height(&cpy);
    }
    return offset;
}

void destroy_tetromino(tetromino_t *tetromino)
{
    if (tetromino != NULL) {
        free(tetromino->mask);
        tetromino->mask = NULL;
        tetromino->side_len = 0;
    }
}

// ----------- TRANSLATION-UNIT-LOCAL FUNCTIONS ----------- //

void render_quad(const position_t * pos, const color_t * color) {
    static_assert(sizeof(*color) == 4, "detected unexpected size for colors");
    glColor3ubv((unsigned char *)color);
    glBegin(GL_QUADS); {
        glVertex2i(pos->x,     pos->y);
        glVertex2i(pos->x,     pos->y + 1);
        glVertex2i(pos->x + 1, pos->y + 1);
        glVertex2i(pos->x + 1, pos->y);
    } glEnd();
}

rotation_func get_rotation_func(const tetromino_t *tetromino)
{
    assert(tetromino);
    rotation_func rotate = NULL;
    switch (tetromino->orientation) {
        case ROTATED_DEFAULT:
            rotate = rotate_none;
            break;
        case ROTATED_RIGHT:
            rotate = rotate_right;
            break;
        case ROTATED_LEFT:
            rotate = rotate_left;
            break;
        case ROTATED_HALF:
            rotate = rotate_half;
            break;
    }
    return rotate;
}

void rotate_cw(tetromino_t *tetromino)
{
    assert(tetromino);
    switch (tetromino->orientation) {
        case ROTATED_DEFAULT:
            tetromino->orientation = ROTATED_RIGHT;
            break;
        case ROTATED_RIGHT:
            tetromino->orientation = ROTATED_HALF;
            break;
        case ROTATED_HALF:
            tetromino->orientation = ROTATED_LEFT;
            break;
        case ROTATED_LEFT:
            tetromino->orientation = ROTATED_DEFAULT;
            break;
    }
}

void rotate_ccw(tetromino_t *tetromino)
{
    assert(tetromino);
    switch (tetromino->orientation) {
        case ROTATED_DEFAULT:
            tetromino->orientation = ROTATED_LEFT;
            break;
        case ROTATED_RIGHT:
            tetromino->orientation = ROTATED_DEFAULT;
            break;
        case ROTATED_LEFT:
            tetromino->orientation = ROTATED_HALF;
            break;
        case ROTATED_HALF:
            tetromino->orientation = ROTATED_RIGHT;
            break;
    }
}

// the rotate functions are black magic
// in its pures form!

int rotate_none(int side_len, int row, int col)
{
    return row * side_len + col;
}

int rotate_right(int side_len, int row, int col)
{
    return rotate_none(side_len, side_len - col - 1, row);
}

int rotate_half(int side_len, int row, int col)
{
    return side_len * side_len - rotate_none(side_len, row, col) - 1;
}

int rotate_left(int side_len, int row, int col)
{
    return rotate_none(side_len, col, side_len - row - 1);
}

tetromino_t clone_tetromino(const tetromino_t *tetromino)
{
    assert(tetromino);
    tetromino_t result;

    result.side_len = tetromino->side_len;
    result.pos = tetromino->pos;
    result.orientation = tetromino->orientation;
    result.color = tetromino->color;

    size_t arr_length = ((size_t) result.side_len * (size_t) result.side_len);
    result.mask = malloc(sizeof(bool) * arr_length);
    memcpy(result.mask, tetromino->mask, arr_length);

    return result;
}
