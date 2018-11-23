#ifndef UE06_TETROMINO_H
#define UE06_TETROMINO_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Handy color typedefs for drawing c:
 */
typedef enum {
    color_black,
    color_red     = 0x0000FFU,
    color_green   = 0x00FF00U,
    color_blue    = 0xFF0000U,
    color_yellow  = color_red   | color_green,
    color_magenta = color_red   | color_blue,
    color_cyan    = color_green | color_blue,
    color_white   = color_red   | color_green | color_blue,
} color_t;

/**
 * A 2D position on the game board.
 */
typedef struct {
    int x, y;
} position_t;

/**
 * An array holding a mask which represents
 * a tetromino.
 */
typedef bool* tetromino_mask_t;

typedef enum {
    ROTATED_DEFAULT, ROTATED_RIGHT, ROTATED_LEFT, ROTATED_HALF
} orientation_t;

typedef enum {
    CLOCKWISE, COUNTER_CLOCKWISE
} rotation_t;

/**
 * A compound representing a tetris object,
 * called a tetromino.
 */
typedef struct {
    /**
     * The current orientation (/rotation)
     * of the tetromino.
     */
    orientation_t orientation;

    /**
     * The current position of the
     * tetromino.
     */
    position_t pos;

    /**
     * The color of the tetromino.
     */
    color_t color;

    /**
     * The side length of the matrix representing
     * the tetrominos tile-occupation-mask.
     */
    int side_len;

    /**
     * The tetrominos
     * tile-occupation-mask.
     */
    tetromino_mask_t mask;
} tetromino_t;

/**
 * An enumeration containing all Tetrominoes.
 */
typedef enum {
    I, J, L, O, S, T, Z
} tetromino_shape;

/**
 * @param shape The shape of the tetromino to create.
 * @return A copy of the tetromino prototype with the
 * 			specified shape.
 */
tetromino_t create_tetromino(tetromino_shape shape);

/**
 * Renders a tetromino to the current OpenGL Context.
 * @param tetromino The tetromino to render.
 */
void render_tetromino(const tetromino_t *tetromino);

/**
 * Rotates the supplied tetromino according to the
 * specified rotation direction.
 * @param tetromino The tetromino to rotate.
 * @param rotation The direction in which to rotate
 *                  dat boi.
 */
void rotate_tetromino(tetromino_t *tetromino, rotation_t rotation);

/**
 * @param tetromino The tetromino to query.
 * @param row
 * @param col
 * @return The mask value (the occupation) at the
 *          specified row and column.
 */
bool get_tetromino_mask_at(const tetromino_t *tetromino, int row, int col);

/**
 * @param tetromino The tetromino to query.
 * @return The height of the tetromino without
 *          empty mask rows at the bottom.
 */
int real_height(const tetromino_t *tetromino);

/**
 * Cuts through the supplied tetromino horizontally
 * and removes the specified row, shifting all rows
 * below that one up by 1.
 * @param tetromino The tetromino to edit.
 * @param row The row to remove.
 */
void remove_tetromino_row(const tetromino_t *tetromino, int row);

/**
 * @param tetromino The tetromino from which to fetch the offset.
 * @return The left offset of the actual tetromino
 *          within its tile-occupation-mask.
 */
int left_offset(const tetromino_t *tetromino);

/**
 * @param tetromino The tetromino from which to fetch the offset.
 * @return The right offset of the actual tetromino
 *          within its tile-occupation-mask.
 */
int right_offset(const tetromino_t *tetromino);

/**
 * Destroys all data associated with the supplied tetromino.
 * @param tetromino The tetromino to destroy.
 */
void destroy_tetromino(tetromino_t *tetromino);

#endif //!UE06_TETROMINO_H
