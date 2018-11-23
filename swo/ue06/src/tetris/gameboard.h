#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <GLFW/glfw3.h>
#include "tetromino.h"

#define GB_ROWS 22
#define GB_COLS 11

/**
 * Every information about the
 * game board and the current state
 * of the game.
 */
typedef struct {
    /**
     * All tetrominoes that are on the game board.
     */
    tetromino_t *tetrominoes;

    /**
     * The number of tetrominoes on the game board.
     */
    int nr_of_tetrominoes;

    /**
     * The current size of the tetromino vector.
     */
    int vec_size;

    /**
     * A matrix of boolean values which state whether
     * a particular tile is occupied.
     */
    bool *tile_matrix;
} tetris_board_t;

/**
 * The single game board instance.
 */
extern tetris_board_t game_board;

/**
 * A global flag which reflects
 * the current state of the game.
 */
extern bool running;

/**
 * The number of updates that
 * the game receives per second
 * (approximately).
 */
extern int ticks_pers_second;

/**
 * Initializes the game board.
 * (Bet u didn't suspect that huh?)
 */
void init_game_board();

/**
 * Renders the game board to the supplied glfw window.
 * @param window The window to draw to.
 * @param width The width of the viewport.
 * @param height The height of the viewport.
 */
void render_board(GLFWwindow *window, int width, int height);

/**
 * Updates the game state.
 */
void update_board();

/**
 * The keymap for the game. (Using GLFWs setKeyCallback)
 */
void gameboard_keymap(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * Destroys the game board and cleans up.
 */
void destroy_game_board();

#endif
