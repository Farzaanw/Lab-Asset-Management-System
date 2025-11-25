#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <string>
#include "./library/nlohmann/json.hpp"

class User;

class SystemController {
public:
    SystemController();
    void run();

private:
    // Core program control
    int homepage();
    int role_selection_menu();         // role selection menu
    void create_account(int roleChoice); 
    int log_in(int roleChoice);        // now matches .cpp

    // JSON & logging
    void load_login_json();
    void update_usage_log(const std::string& message);
    bool validate_user(const std::string& email, const std::string& password, const std::string& role);

    // Helper functions
    std::string get_current_time();
    bool load_json_safe(const std::string& path, nlohmann::json& out);

    bool isOpen;
    User* currentUser = nullptr;
    nlohmann::json roleLoginJson;
    nlohmann::json usage_log;
};

#endif // SYSTEM_CONTROLLER_H