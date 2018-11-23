#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

#include "tetromino.h"
#include "gameboard.h"

#define WIDTH  400
#define HEIGHT WIDTH * (GB_ROWS / GB_COLS)

/**
 * Prints the keyboard controls to the
 * standard output.
 */
static void print_instructions()
{
    printf("Keymap:\n");
    printf("Arrow Up ^ : Rotate tetromino clockwise\n");
    printf("Arrow Down v : Rotate tetromino counter clockwise\n");
    printf("Arrow Right > : Move tetromino to the right\n");
    printf("Arrow Left < : Move tetromino to the left\n");
    printf("Space _ : Drop the tetromino like it's hot\n");
}

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Tetris 4.0", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Failed to create window.\n");
        return EXIT_SUCCESS;
    }

    print_instructions();

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetWindowAspectRatio(window, width, height);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, gameboard_keymap);

    init_game_board();

    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // the board is always rendered!
        // when running == false, the background
        // color fades through a few colors over
        // time!
        render_board(window, width, height);

        if (running) {
            // the update rate changes over time so
            // it's a variable
            if (glfwGetTime() - last_time > (1.0 / ticks_pers_second)) {
                last_time = glfwGetTime();
                update_board();
            }
        }

        const GLenum error = glGetError();
        if(error != GL_NO_ERROR) fprintf(stderr, "ERROR: %d\n", error);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy_game_board();
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
