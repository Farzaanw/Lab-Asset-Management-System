#include "User.h"
#include <iostream>

#include "../SystemController.h"

// Getter functions


std::string User::getEmail() const {
    return email;
}

// Default displayInfo() method
void User::displayInfo() const {
    std::cout << "User Information:\n";
    std::cout << "  Email: " << email << "\n";
}
