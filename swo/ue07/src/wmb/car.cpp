#include "car.hpp"
#include "motor.hpp"
#include "wheel.hpp"

using std::ostream;
using std::istream;
using std::endl;

car_t::car_t(std::istream &is)
{
    int type;
    is >> type;
    _type = static_cast<car_type_t>(type);

    is >> _color
       >> _serial
       >> _production_date
       >> _production_location;

    int transm;
    is >> transm;
    _transmission = static_cast<transmission_t>(transm);

    is >> _max_speed
       >> _motor
       >> _wheels;
}

std::ostream &car_t::print(std::ostream &os) const
{
    return os << "==== Car ====" << endl
              << "Serial: " << _serial << endl
              << "Color: " << _color << endl
              << "Produced in: " << _production_location << endl
              << "Max Speed: " << _max_speed << "km/s" << endl
              << '\t' << _motor << endl
              << '\t' << _wheels;
}

ostream & operator<<(ostream &os, const car_t &car)
{
    return car.print(os);
}


istream & operator>>(istream &is, car_t &car)
{
    car = car_t(is);
    return is;
}