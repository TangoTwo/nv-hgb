#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cli_parser.h"

/**
 * Prints a short guide for the command line
 * interface.
 */
void print_cli_guide()
{
    printf("Program usage: polycirc <point> <point> <point> [<point> ...]\n");
    printf("\t- <point> = <x>:<y>.\n");
    printf("\t- <x> and <y> can be an arbitrary integer number.");
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printf("polycirc needs at least 3 points to calculate the circumference!\n");
        print_cli_guide();
        return EXIT_FAILURE;
    }

    double distance = 0;
    // initialize to 1 so errors on the first point
    // literal are logged correctly
    int i = 1;
    ParseError error = NONE;
    // store first point in extra variable so the
    // last point can be "connected to it"
    Point initial = parse_point_literal(argv[1], &error);
    Point buff = initial;
    if (error == NONE) {
        printf("Point %d: (%d,%d)\n", i, buff.x, buff.y);
        i = 2;
        while (i < argc && error == NONE) {
            Point new_point = parse_point_literal(argv[i], &error);
            distance += distance_between(buff, new_point);
            buff = new_point;
            // Note: no increment without error ONLY
            // because error is a termination condition
            if (error == NONE) {
                printf("Point %d: (%d,%d)\n", i, buff.x, buff.y);
                ++i;
            }
        }
    }

    switch (error) {
        case NONE:
            // add last edge of the polygon to the total distance
            distance += distance_between(buff, initial);
            printf("The total circumference is %f\n", distance);
            break;
        case X_COORD:
            printf("The x coordinate on point %d is missing.\n", i);
            break;
        case Y_COORD:
            printf("The y coordinate on point %d is missing.\n", i);
            break;
        default:
            printf("Encountered unknown parsing error: %d\n", error);
            break;
    }

    return error;
}