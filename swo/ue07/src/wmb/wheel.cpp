#include "wheel.hpp"

using std::ostream;
using std::istream;
using std::endl;

wheel_t::wheel_t(std::istream &is)
{
    is >> _diameter
       >> _production_date
       >> _velocity_index
       >> _brand;
}

std::ostream &wheel_t::print(std::ostream &os) const
{
    return os << "==== Wheels ====" << endl
              << "Brand: " << _brand << endl
              << "Diameter: " << _diameter << endl
              << "Vel. Index: " << _velocity_index;
}

ostream & operator<<(ostream &os, const wheel_t &wheel)
{
    return wheel.print(os);
}

istream & operator>>(istream &is, wheel_t &wheel)
{
    wheel = wheel_t(is);
    return is;
}


