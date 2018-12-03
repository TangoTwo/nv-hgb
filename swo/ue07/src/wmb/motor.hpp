#pragma once
#include <string>
#include <chrono>
#include <iostream>

/**
 * All types of fuels (that are supported by this
 * totally legitimate business software).
 */
enum class fuel_t
{
    DIESEL, GASOLINE
};

class motor_t
{
public: // typedefs

    /**
     * Horse power.
     */
    using hp = float;

public: // methods

    motor_t() = default;

    /**
     * Creates a motor by reading the supplied input stream.
     * @param is The stream containing motor details.
     */
    explicit motor_t(std::istream &is);

    /**
     * Prints a few motor details to the supplied output stream.
     * @param os The output stream to write the details to.
     * @return The output stream (to allow for chaining).
     */
    std::ostream & print(std::ostream &os) const;

private: // members
    unsigned  _serial           {};
    // "MY BLOOD IS GASOLINEEE" (R&M Reference)
    fuel_t    _fuel             {fuel_t::GASOLINE};
    hp        _hp               {};
    float     _avg_consumption  {};
    time_t    _production_date  {};
};

std::ostream & operator<<(std::ostream &os, const motor_t &motor);
std::istream & operator>>(std::istream &is, motor_t &motor);
