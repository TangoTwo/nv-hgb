#include <iostream>
#include <fstream>
#include "car.hpp"

using std::cout;
using std::endl;
using std::ifstream;

int main()
{
    ifstream cars_file("./cars.txt");

    car_t car;
    while (cars_file.good()) {
        cars_file >> car;
        cout << car << endl;
    }

    return 0;
}
