// User.cpp
// Auth: Farzaan Wadiwala

class User {
private:
    std::string email;
    std::string password;
    std::string role;
public:
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; }
    std::string getRole() const { return role; }
};