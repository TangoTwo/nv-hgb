#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "gameboard.h"

/**
 * @return A reference to the currently falling tetromino.
 */
static tetromino_t *currently_falling_tetromino();

/**
 * Applies a translation to the supplied tetromino.
 * @param tetromino The tetromino to move.
 * @param xDelta The distance to move it on the x-axis.
 * @param yDelta The distance to move it on the y-axis.
 */
static void move_tetromino(tetromino_t *tetromino, int xDelta, int yDelta);

/**
 * Generates a new tetromino and adds it to the game board.
 */
static void spawn_tetromino();

/**
 * Writes the supplied tetromino to the game board matrix
 * with regards to its current position (on the board).
 * @param tetromino The tetromino to write.
 */
static void write_tetromino_to_matrix(const tetromino_t *tetromino);

/**
 * Removes any rows that are filled with tiles.
 */
static void remove_first_filled_row();

/**
 * @param row The row to check.
 * @return True if the specified row is filled with tiles,
 *          False otherwise.
 */
static bool row_can_be_deleted(int row);

/**
 * @param tetromino The tetromino to check.
 * @return True if the supplied tetromino can still
 *          fall further down on the game board without
 *          intersecting with other tetrominoes.
 */
static bool can_fall(const tetromino_t *tetromino);

/**
 * Attempts to rotate the tetromino. If the rotation
 * would lead to the tetromino goin out of bounds
 * the rotation is prevented.
 * @param tetromino The tetromino to rotate.
 * @param rotation The rotation direction.
 */
static void rotate_within_board(tetromino_t *tetromino, rotation_t rotation);

/**
 * @param tetromino The tetromino to check.
 * @return True if the tetromino itself is within
 *         the board boundaries, False otherwise.
 */
static bool is_within_board(const tetromino_t *tetromino);

/**
 * @return True if at least one tile of the uppermost row
 *          is not empty, False otherwise.
 */
static bool is_board_full();

// global (extern) variables
tetris_board_t game_board;
bool running = true;
int ticks_pers_second = 2;

void init_game_board()
{
    // seed RNG with system time
    time_t t;
    srand((unsigned) time(&t));

    // the average player won't be able to reconcile more
    // than 20 tetrominoes on the board :P
    game_board.tetrominoes = malloc(sizeof(tetromino_t) * 50);
    game_board.vec_size = 50;

    // create a matrix representing the game boards occupied tiles
    game_board.tile_matrix = malloc(sizeof(bool) * GB_ROWS * GB_COLS);
    for (size_t i = 0; i < GB_ROWS * GB_COLS; ++i) {
        game_board.tile_matrix[i] = false;
    }

    game_board.nr_of_tetrominoes = 0;
}

void render_board(GLFWwindow *window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // basically flip this boi so that the coordinate system
    // has its origin in the upper left hand corner. that makes
    // it easier to work with the matrices!
    glOrtho(0, width, height, 0, 0, 1);

    glScalef((float) width / (float) GB_COLS, (float) height / (float) GB_ROWS, 1);

    // let all tetrominoes render themselves
    if (running) {
        for (int i = 0; i < game_board.nr_of_tetrominoes; ++i) {
            render_tetromino(&game_board.tetrominoes[i]);
        }
    } else {
        // hehe cheeky macro!
#define color(f) (float) (f(glfwGetTime()))
        glClearColor(color(0.7*sin), color(cos), color(0.3*sin), 1.0f);
    }
}

void update_board()
{
    tetromino_t *curr = currently_falling_tetromino();
    if (curr != NULL) {
        if (can_fall(curr)) {
            move_tetromino(curr, 0, 1);
        } else {
            // fix tetromino on the board so
            // new falling tetrominoes can check
            // for intersections
            write_tetromino_to_matrix(curr);
            remove_first_filled_row();
            if (!is_board_full()) {
                spawn_tetromino();
            } else {
                running = false;
            }
        }
    } else { // curr == NULL -> no tetrominoes on the board
        spawn_tetromino();
    }
}

void gameboard_keymap(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void) window; (void) scancode; (void) mods;

    tetromino_t *curr = currently_falling_tetromino();
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_RIGHT:
                move_tetromino(curr, 1, 0);
                break;
            case GLFW_KEY_LEFT:
                move_tetromino(curr, -1, 0);
                break;
            case GLFW_KEY_UP:
                rotate_within_board(curr, CLOCKWISE);
                break;
            case GLFW_KEY_DOWN:
                rotate_within_board(curr, COUNTER_CLOCKWISE);
                break;
            case  GLFW_KEY_SPACE:
                while (can_fall(currently_falling_tetromino())) {
                    update_board();
                }
                break;
            default:
                // do nothing
                break;
        }
    }
}

void destroy_game_board()
{
    assert(game_board.tile_matrix);
    assert(game_board.tetrominoes);

    free(game_board.tile_matrix);
    game_board.tile_matrix = NULL;

    for (int i = 0; i < game_board.nr_of_tetrominoes; ++i) {
        destroy_tetromino(&game_board.tetrominoes[i]);
    }
    free(game_board.tetrominoes);
    game_board.tetrominoes = NULL;

    game_board.nr_of_tetrominoes = 0;
}

// ----------- TRANSLATION-UNIT-LOCAL FUNCTIONS ----------- //

void remove_first_filled_row()
{
    // find the first row that can be deleted
    int row = GB_ROWS - 1;
    while (row > 0 && !row_can_be_deleted(row)) {
        --row;
    }

    if (row > 0) { // a full row exists

        // adjust the masks of tetrominoes that overlap with that row
        int overlap = 0;
        while (overlap < game_board.nr_of_tetrominoes) {
            tetromino_t *curr = &game_board.tetrominoes[overlap];
            if (curr->pos.y <= row && curr->pos.y + curr->side_len - 1 >= row) {
                remove_tetromino_row(curr, row - curr->pos.y);
            }
            if (curr->pos.y <= row) {
                curr->pos.y += 1;
            }
            ++overlap;
        }

        // rewrite matrix with "new tetromino configuration"
        for (int i = 0; i < GB_COLS * GB_ROWS; ++i) {
            game_board.tile_matrix[i] = false;
        }
        for (int i = 0; i < game_board.nr_of_tetrominoes; ++i) {
            write_tetromino_to_matrix(&game_board.tetrominoes[i]);
        }

        // recurse if any rows have been removed
        // (in case row right above is filled as well)
        remove_first_filled_row();
    }
}

bool row_can_be_deleted(int row)
{
    int i = 0;
    while (i < GB_COLS && game_board.tile_matrix[row * GB_COLS + i]) {
        ++i;
    }
    return i == GB_COLS;
}

void write_tetromino_to_matrix(const tetromino_t *tetromino)
{
    assert(tetromino);
    int row_offset = tetromino->pos.y;
    int col_offset = tetromino->pos.x;

    for (int row = 0; row < tetromino->side_len; ++row) {
        for (int col = 0; col < tetromino->side_len; ++col) {
            // calculate the game board index to write to using
            // the tetrominoes position.

            if (col_offset + col >= 0) {
                // tetrominoes can have a blank mask column / row;
                // that row is allowed to go outside the bounds of
                // the game board so that the actual tetromino can
                // align to the edge, thats why I test for x >= 0

                // EDIT: I think there should also be tests for
                // x > GB_COLS and y > GB_ROWS but it works and
                // I don't want to break anything

                int index = (row_offset + row) * GB_COLS + (col_offset + col);
                game_board.tile_matrix[index] |= get_tetromino_mask_at(tetromino, row, col);
            }
        }
    }
}

void spawn_tetromino()
{
    // resize the vector if its full
    if (game_board.nr_of_tetrominoes == game_board.vec_size) {
        // for some reason realloc removed like the 4th element
        // from the original vector ??? And I thought I understood
        // pointers...
        tetromino_t *new_vec = malloc(sizeof(tetromino_t) * (size_t) game_board.vec_size * 2);
        for (int i = 0; i < game_board.vec_size; ++i) {
            new_vec[i] = game_board.tetrominoes[i];
        }
        free(game_board.tetrominoes);
        game_board.tetrominoes = new_vec;
        game_board.vec_size *= 2;
    }

    // exploit the nature of enums: the underlying data type is numeric
    // so a tetromino can be generated using RNG
    tetromino_shape shape = rand() % 7;
    tetromino_t tetromino = create_tetromino(shape);
    tetromino.pos.x = GB_COLS / 2 - tetromino.side_len / 2;
    game_board.tetrominoes[game_board.nr_of_tetrominoes] = tetromino;
    game_board.nr_of_tetrominoes += 1;

    // also increase speed after every 5th tetromino spawn
    if (game_board.nr_of_tetrominoes % 10 == 0) {
        ticks_pers_second += 1;
    }
}

void move_tetromino(tetromino_t *tetromino, int xDelta, int yDelta)
{
    assert(tetromino);
    tetromino_t cpy = *tetromino;
    cpy.pos.x+=xDelta;
    cpy.pos.y+=yDelta;
    if (is_within_board(&cpy)) {
        // apply changes if they are valid
        tetromino->pos.x += xDelta;
        tetromino->pos.y += yDelta;
    }
}

void rotate_within_board(tetromino_t *tetromino, rotation_t rotation)
{
    assert(tetromino);
    tetromino_t cpy = *tetromino;
    rotate_tetromino(&cpy, rotation);
    if (is_within_board(&cpy)) {
        // apply changes if they are valid
        rotate_tetromino(tetromino, rotation);
    }
}

bool can_fall(const tetromino_t *tetromino)
{
    assert(tetromino);
    tetromino_t tetr_cpy = *tetromino;
    tetr_cpy.pos.y += 1;

    int row_offset = tetr_cpy.pos.y;
    int col_offset = tetr_cpy.pos.x;

    // if the tetromino hit the bottom already,
    // it can't fall any further
    bool can_fall = row_offset + real_height(&tetr_cpy) <= GB_ROWS;
    // if it's still above ground level, check if it would
    // intersect with other tetrominoes if it were to fall
    // one tile further
    int row = 0;
    while (row < tetr_cpy.side_len && can_fall) {
        int col = 0;
        while (col < tetr_cpy.side_len && can_fall) {
            // get the matrix index relative to the tetrominoes position
            int gb_index = (row_offset + row) * GB_COLS + (col_offset + col);
            // either one of the tile flags must be False, otherwise they intersect
            can_fall = game_board.tile_matrix[gb_index] == false ||
                    get_tetromino_mask_at(&tetr_cpy, row, col) == false;
            ++col;
        }
        ++row;
    }
    return can_fall;
}

tetromino_t *currently_falling_tetromino()
{
    tetromino_t *last = NULL;
    if (game_board.nr_of_tetrominoes > 0) {
        last = &game_board.tetrominoes[game_board.nr_of_tetrominoes - 1];
    }
    return last;
}

bool is_within_board(const tetromino_t *tetromino)
{
    assert(tetromino);

    // the x position might be negative with the full tetromino
    // still being completely within bounds, hence the complex check
    bool boundary_ok = tetromino->pos.x + left_offset(tetromino) >= 0 &&
                       tetromino->pos.x + (tetromino->side_len - right_offset(tetromino) - 1) < GB_COLS;

    bool no_intersects = true;
    int row = 0;
    while (row < tetromino->side_len && no_intersects) {
        int col = 0;
        while (col < tetromino->side_len && no_intersects) {
            int matrix_ind = (tetromino->pos.y + row) * GB_COLS + (tetromino->pos.x + col);
            // if both tiles are set, they intersect
            no_intersects = get_tetromino_mask_at(tetromino, row, col) == false ||
                            game_board.tile_matrix[matrix_ind] == false;
            ++col;
        }
        ++row;
    }

    return boundary_ok && no_intersects;
}

bool is_board_full()
{
    int col = 0;
    while (col < GB_COLS && game_board.tile_matrix[col] == 0) {
        ++col;
    }
    return col != GB_COLS;
}
