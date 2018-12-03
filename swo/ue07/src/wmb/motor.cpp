#include "motor.hpp"

using std::ostream;
using std::istream;
using std::endl;

motor_t::motor_t(std::istream &is)
{
    is >> _serial;

    int fuel;
    is >> fuel;
    _fuel = static_cast<fuel_t>(fuel);

    is >> _hp
       >> _avg_consumption
       >> _production_date;
}

std::ostream &motor_t::print(std::ostream &os) const
{
    return os << "==== Motor ====" << endl
              << "Serial: " << _serial << endl
              << "Torque: " << _hp << "hp" << endl
              << "Avg Consumption: " << _avg_consumption << "l/100km";
}

ostream & operator<<(ostream &os, const motor_t &motor)
{
    return motor.print(os);
}

istream & operator>>(istream &is, motor_t &motor)
{
    motor = motor_t(is);
    return is;
}
