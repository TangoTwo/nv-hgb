#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include "motor.hpp"
#include "wheel.hpp"

/**
 * Random types of cars.
 */
enum class car_type_t
{
    VAN, CONVERTIBLE, SUV, JEEP
};

/**
 * Kinds of transmission.
 */
enum class transmission_t
{
    AUTOMATIC, MANUAL
};

class car_t
{
public: // typedefs

    /**
     * Kilometers per second.
     */
    using kmps = float;

public: // methods

    car_t() = default;

    /**
     * Creates a car by reading the supplied input stream.
     * @param is The stream containing car details.
     */
    explicit car_t(std::istream &is);

    /**
     * Prints a few car details to the supplied output stream.
     * @param os The output stream to write the details to.
     * @return The output stream (to allow for chaining).
     */
    std::ostream & print(std::ostream &os) const;

private: // members (should be self explanatory due to descriptive naming)
    car_type_t      _type                   {car_type_t::SUV};
    std::string     _color;
    unsigned        _serial                 {};
    time_t          _production_date        {};
    std::string     _production_location;
    transmission_t  _transmission           {transmission_t::MANUAL};
    kmps            _max_speed              {};
    motor_t         _motor;
    wheel_t         _wheels;
};

std::ostream & operator<<(std::ostream &os, const car_t &car);
std::istream & operator>>(std::istream &is, car_t &car);