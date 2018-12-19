#include <iostream>
#include <fstream>
#include "Types.hpp"
#include "Game.hpp"
#include "Chessman.hpp"

using namespace Chess;
using std::cout;
using std::endl;

static void run_multiplayer()
{
    Game game;
    while (!game.is_over()) {
        game.next_move(std::cin, std::cout);
    }
}

static Position generate_pos (std::ostream &os) {
    time_t t;
//    srand(time(&t)); // uncomment for madness
    Position pos = {
            static_cast<unsigned>(std::rand() % 8),
            static_cast<char>('a' + (std::rand() % 8))
    };
    // write new position to the supplied output
    // for visualization. (Otherwise you would not
    // be able to see directly what the generator
    // produces!)
    os << pos.first + 1 << ' ' << pos.second << std::endl;
    return pos;
};

static void run_generated()
{
    Game game;
    std::ofstream out("./output.txt");
    auto generate_pos_bound = [&out](){
        return generate_pos(out);
    };
    while (!game.is_over()) {
        game.next_move(generate_pos_bound, out);
    }
}

int main(int argc, char **)
{
    // lazy switch: if any program arguments were supplied
    // use chess game generator, otherwise start M U L T I P
    // L A Y E R
    if (argc > 1) {
        run_generated();
    } else {
        run_multiplayer();
    }

    return 0;
}