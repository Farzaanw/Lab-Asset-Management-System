#include "User.h"
#include <iostream>

#include "../SystemController.h"

// Getter functions
std::string User::getFirstName() const {
    return firstName;
}

std::string User::getLastName() const {
    return lastName;
}

std::string User::getEmail() const {
    return email;
}

// Default displayInfo() method
void User::displayInfo() const {
    std::cout << "User Information:\n";
    std::cout << "  First Name: " << firstName << "\n";
    std::cout << "  Last Name: " << lastName << "\n";
    std::cout << "  Email: " << email << "\n";
}
