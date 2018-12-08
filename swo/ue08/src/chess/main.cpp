#include <iostream>
#include "Chessman.cpp"

int main()
{
    auto *k = new King(Color::BLACK);
    std::cout << k->is_essential() << std::endl;
    delete k;
    return 0;
}