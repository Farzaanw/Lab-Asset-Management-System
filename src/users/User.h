// User.cpp
// Auth: Farzaan Wadiwala
#ifndef USERS_H
#define USERS_H

#include <string>
#include <iostream>

class User {
public:
    User(const std::string& firstName,
         const std::string& lastName,
         const std::string& email,
         int role);
    virtual ~User() = default;

    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    int getRole() const;
    virtual void displayInfo() const;

protected:
    std::string firstName;
    std::string lastName;
    std::string email;
    int role;
};

#endif // USERS_H