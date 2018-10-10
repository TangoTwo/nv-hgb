#include "cli_parser.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define nullptr 0

double distance_between(Point p1, Point p2)
{
    int x_dist = p2.x - p1.x;
    int y_dist = p2.y - p1.y;
    return sqrt(x_dist * x_dist + y_dist * y_dist);
}

Point origin() {
    Point p = {0, 0};
    return p;
}

int parse_number_literal(const char *const start, size_t *length)
{
    int dest = 0;
    if (start != nullptr) {
        size_t cpy_length = 0;
        // NumberLiteral = [ '-' ] Digit { Digit } .
        if (start[cpy_length] == '-') {
            cpy_length = 1;
        }
        // cpy_length should span the whole literal!
        while (isdigit(start[cpy_length])) {
            ++cpy_length;
        }
        if (length != nullptr) {
            if (cpy_length == 1 && start[0] == '-') {
                *length = 0; // indicate error
            } else {
                *length = cpy_length;
            }
        }
        char *temp = (char *) malloc(cpy_length + 1);
        strncpy(temp, start, cpy_length);
        dest = atoi(temp); // NOLINT(cert-err34-c)
        free(temp);
    }
    return dest;
}

Point parse_point_literal(const char *const arg, ParseError *error)
{
    Point p = origin();
    if (arg != nullptr) {
        if (error != nullptr) {
            *error = NONE;
        }
        size_t literal_length = 0;
        p.x = parse_number_literal(arg, &literal_length);
        if (error != nullptr && literal_length == 0) {
            *error = X_COORD;
        }
        // coordinates are separated by a comma so
        // the next number literal starts at the length
        // of the last number + 1!
        p.y = parse_number_literal(arg + literal_length + 1, &literal_length);
        if (error != nullptr && literal_length == 0) {
            *error = Y_COORD;
        }
    }
    return p;
}
