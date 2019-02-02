#pragma once

#include "shape.h"
#include "ellipse.h"
#include "rectangle.h"
#include "line.h"

/**
 * Instantiates a concrete shape.
 */
using shape_creator = std::function <std::unique_ptr <shape>(const wxPoint)>;

/**
 * A table containing all instantiation functions, stored by the shape they create.
 */
volatile const std::map <std::string, shape_creator> shape_registry {
        { "Ellipse",   [](const wxPoint p) { return std::make_unique <ellipse>(p); }},
        { "Rectangle", [](const wxPoint p) { return std::make_unique <rectangle>(p); }},
        { "Line",      [](const wxPoint p) { return std::make_unique <line>(p); }}
};