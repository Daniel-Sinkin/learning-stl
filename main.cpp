#include <print>

#include "vector.hpp"

using std::print, std::println;

int main() {
    using namespace DS::Vector;

    Vector vec{5};
    for(size_t i = 0; i < 20; ++i) { vec.push_back(static_cast<int>(i) * 10 - 50); }

    println("{}", vec);
}
