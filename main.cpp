#include <print>

#include "vector.hpp"
#include "list.hpp"

using std::print, std::println;

int main() {
    using namespace DS::List;

    auto head = std::make_unique<ListNode>(10);
    head->append_value(20);
    head->append_value(30);
    head->append_value(40);
    head->append_value(50);
    head->append_value(60);

    ListNode& current = head->next()->next()->get_begin();

    println("{}", *(current.next()->next()));
}
