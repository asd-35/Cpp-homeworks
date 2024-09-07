#include "contact_list.h"
#include "hw03.h"

#include <iostream>

namespace contact_list {

void test_contact_list() {
    storage abook;
    std::cout << add(abook, "Best Friend", 421234) << std::endl;
    std::cout << add(abook, "False Friend", 10032331) << std::endl;
    std::cout << add(abook, "False Friend", 10032331) << std::endl;
    std::cout << get_number_by_name(abook,"Best Friend") << std::endl;
    std::cout << get_number_by_name(abook,"False Friend") << std::endl;
    std::cout << get_number_by_name(abook,"False Frien ") << std::endl;
    std::cout << to_string(abook) << std::endl;
    std::cout << get_name_by_number(abook,421234) << std::endl;
    std::cout << get_name_by_number(abook,10032331) << std::endl;
    std::cout << get_name_by_number(abook,1003231) << std::endl;
    sort(abook);
    std::cout << to_string(abook) << std::endl;
    std::cout << remove(abook,"Best Friend") << std::endl;
    std::cout << size(abook) << std::endl;
    sort(abook);
    
    }
} // namespace contact_list


int main() {
    contact_list::test_contact_list();

    return 0;
}
