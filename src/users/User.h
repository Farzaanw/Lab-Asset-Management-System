#ifndef USER_HPP
#define USER_HPP

#include <string>

class SystemController;  // forward declaration

class User {
protected:
    std::string email;
    SystemController* system;  // pointer back to controller

public:
    User(const std::string& email,
         SystemController* system)        // <-- instance of System !!!!
        : email(email), system(system) {}

    virtual ~User() {}

    // ----------- PURE VIRTUAL METHODS ----------- //
    virtual void main() = 0;     // each user type implements this
    std::string getEmail() const;
    virtual void displayInfo() const;

    // optional virtual methods for future features
    virtual std::string getRole() const = 0;
};

#endif
