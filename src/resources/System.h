// CS-7: System.h
// Auth: Wadiwalla, Farzaan
// Supports: US-101, UR-201, UR-310
// Collaborators: User[*]
#include <string>
#include <vector>
#pragma once

class System {
private:
    struct User {
        std::string username;
        std::string password;
    };

    std::vector<User> users; // Stores all registered users

public:
    // Registers a new user with username and password
    bool registerUser(const std::string& username, const std::string& password); // returns true if successful, false if username already exists

    // Validates a user's login credentials
    bool validateLogin(const std::string& username, const std::string& password); // returns true if login is valid, false otherwise

    // Optional: retrieves all usernames
    std::vector<std::string> getAllUsers(); // returns a list of usernames
};
