#ifndef USER_HPP
#define USER_HPP

#include <string>

class SystemController;  // forward declaration

class User {
protected:
    std::string firstName;
    std::string lastName;
    std::string email;
    SystemController* system;  // pointer back to controller

public:
    User(const std::string& firstName,
         const std::string& lastName,
         const std::string& email,
         SystemController* system)        // <-- instance of System !!!!
        : firstName(firstName), lastName(lastName), email(email), system(system) {}

    virtual ~User() {}

    // ----------- PURE VIRTUAL METHODS ----------- //
    virtual void main() = 0;     // each user type implements this
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    virtual void displayInfo() const;

    // optional virtual methods for future features
    virtual std::string getRole() const = 0;
};

#endif