#pragma once
#include <string>
#include <chrono>
#include <iostream>

class wheel_t
{
public: // methods

    wheel_t() = default;

    /**
     * Creates a wheel by reading the supplied input stream.
     * @param is The stream containing wheel details.
     */
    explicit wheel_t(std::istream &is);

    /**
     * Prints a few wheel details to the supplied output stream.
     * @param os The output stream to write the details to.
     * @return The output stream (to allow for chaining).
     */
    std::ostream & print(std::ostream &os) const;

private: // members
    float        _diameter          {};
    time_t       _production_date   {};
    char         _velocity_index    {};
    std::string  _brand;
};

std::ostream & operator<<(std::ostream &, const wheel_t &);
std::istream & operator>>(std::istream &, wheel_t &);
