#include "User.h"

// --------------------
// Base User Constructor
// --------------------
User::User(const std::string& firstName,
           const std::string& lastName,
           const std::string& email,
           int role)
    : firstName(firstName), lastName(lastName), email(email), role(role) {}

// --------------------
// Getter methods
// --------------------
std::string User::getFirstName() const {
    return firstName;
}

std::string User::getLastName() const {
    return lastName;
}

std::string User::getEmail() const {
    return email;
}

int User::getRole() const {
    return role;
}

// --------------------
// Generic display info
// --------------------
void User::displayInfo() const {
    std::cout << "Name: " << firstName << " " << lastName
              << ", Email: " << email
              << ", Role: " << role << std::endl;
}
