#include <string>

class User {
private:
    std::string firstName;
    std::string lastName;
    std::string role;
    std::string email;
    std::string password;

public:
    // Constructor
    User(const std::string& f, const std::string& l,
         const std::string& e, const std::string& p)
        : firstName(f), lastName(l), email(e), password(p) {}
};
