#include <string>

class User {
private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string password;

public:
    // Constructor
    User(const std::string& f, const std::string& l,
         const std::string& e, const std::string& p)
        : firstName(f), lastName(l), email(e), password(p) {}

    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }

    std::string getPassword() const { return password; }

    void setFirstName(const std::string& f) { firstName = f; }
    void setLastName(const std::string& l) { lastName = l; }
    void setEmail(const std::string& e) { email = e; }
    void setPassword(const std::string& p) { password = p; }
};
