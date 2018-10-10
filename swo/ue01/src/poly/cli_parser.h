#ifndef UE01_CLI_PARSER_H
#define UE01_CLI_PARSER_H

#include <ctype.h>

/**
 * An enumeration of errors that can
 * occur during the point parsing
 * process.
 */
typedef enum ParseError
{
    NONE = 0,
    X_COORD = -1,
    Y_COORD = -2
} ParseError;

/**
 * A compound data type for handling
 * points more eloquently.
 */
typedef struct Point
{
    int x;
    int y;
} Point;

/**
 * Calculates the distance between two points.
 * @param p1 One pointy boi.
 * @param p2 The other pointy boi.
 */
double distance_between(Point p1, Point p2);

/**
 * Creates a new point and places it at
 * the origin. Use this function to
 * initialize Point variables.
 */
Point origin();

/**
 * Parses an integer number literal starting
 * at the supplied pointer. If <i>start</i> is
 * a null pointer, 0 is returned.
 * @param start A pointer to the first character which
 *          is part of the number literal.
 * @param length A (nullable) pointer to the variable
 *          which will be overridden with the length
 *          of the literal parsed from the supplied string.
 *          If the number literal is invalid, length is set
 *          to 0.
 */
int parse_number_literal(const char *const start, size_t *length);

/**
 * Parses a string and generates a point from it
 * if it matches the format "%d,%d".
 * @param arg The string holding the point literal.
 * @param error A (nullable) pointer to the variable
 *          which will be overridden with an error code
 *          if any errors occur.
 */
Point parse_point_literal(const char *const arg, ParseError *error);

#endif //UE01_CLI_PARSER_H
