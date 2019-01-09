#include <iostream>
#include <cassert>
#include "deque.hpp"


using swo::deque;

int main()
{
    /*
     * Constructors
     * (Checked with debugger for simplcitiy)
     */

    // construct default
    deque<int> di1;
    // construct from capacity
    deque<int> di2(100);
    // construct from capacity and value
    deque<int> di3(5, -1);
    // construct from initializer list
    deque<int> di4 {1, 2, 3, 4, 5};

    // copy from lvalue reference
    deque<int> di5(di4);
    // copy from rvalue reference
    deque<int> di6(std::move(di4));

    /*
     * Operators
     */

    // =
    // copy assignment
    di4 = di3;
    // move assignment
    di5 = std::move(di6);
    // initializer list assignment
    di6 = {-2, -1, 0, 1, 2};

    // []
    assert(di6[0] == -2);
    assert(di6[4] == 2);
    assert(di4[2] == -1);

    /*
     * Methods
     */
    // at
    assert(di6.at(0) == -2);
    assert(di4.at(3) == -1);
    bool threw = false;
    try {
        di4.at(100);
    } catch (const std::exception &exc) {
        threw = true;
    }
    assert(threw);

    // front
    assert(di6.front() == -2);
    assert(di5.front() == 1);

    // back
    assert(di6.back() == 2);
    assert(di5.back() == 5);

    // empty
    assert(di1.empty());
    assert(di2.empty());
    assert(!di3.empty());
    assert(!di6.empty());

    // size
    assert(di1.size() == 0);
    assert(di2.size() == 0);
    assert(di3.size() == 5);
    assert(deque<int>({1, 2, 3}).size() == 3); // all other deques had size 5 :/

    /*
     * Void bois
     */
    // clear
    deque<char> ds1 = {'h', 'e', 'l', 'l', 'o'};
    ds1.clear();
    assert(ds1.empty());

    // push_back
    ds1.push_back('h');
    ds1.push_back('i');
    assert(ds1.size() == 2);

    // pop_back
    ds1.pop_back();
    assert(ds1.size() == 1);
    assert(ds1.back() == 'h');

    // push_front
    ds1.push_front('a');
    assert(ds1.size() == 2);
    assert(ds1.front() == 'a');

    // pop_front
    ds1.pop_front();
    assert(ds1.front() == 'h');

    // swap
    swap(di3, di6);

    // comparisons
    deque<int> cmp1 {1, 2, 3, 4, 5};
    deque<int> cmp2 {2, 3, 4, 5, 6};
    assert(cmp1 < cmp2);
    assert(cmp1 == cmp1);
    assert(cmp1 != cmp2);
    assert(cmp1 >= cmp1);
    assert(cmp1 >= cmp1);

    /*
     * Iterators
     * (Ony rudimentary tests)
     */
    auto it = di6.begin();
    while (it != di6.end()) {
        std::cout << *it << ' ' << std::flush;
        it += 1;
    }
    std::cout << std::endl;

    auto beg = di3.begin();
    auto mid = di3.begin() + (di3.size() / 2);
    assert(mid - beg == di3.size() / 2);
    assert(beg - di3.begin() == 0);

    for (auto el : di3) {
        std::cout << el << ' ' << std::flush;
    }
    std::cout << std::endl;

    return 0;
}