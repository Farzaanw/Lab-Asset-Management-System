#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <string>
#include "./library/nlohmann/json.hpp"
#include "./resources/Assets.h"
#include <map>
#include <vector>

class User;

class SystemController {
public:
    SystemController();
    void run();
    std::vector<Assets*> assets;

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

    // Member variables
    bool isOpen;                  // Indicates if the system is open
    User* currentUser = nullptr;  // Pointer to currently logged-in user
    std::map<std::string, std::string> systemPolicies; // Stores system policies
    nlohmann::json roleLoginJson; // Stores user login data
    nlohmann::json usage_log;     // Stores usage log data
};

#endif // SYSTEM_CONTROLLER_H