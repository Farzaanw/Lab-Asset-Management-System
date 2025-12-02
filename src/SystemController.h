#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "./library/nlohmann/json.hpp"
#include "./resources/Assets.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class User;

class SystemController {
public:
    // methods
    SystemController();     // Loads all data from /data    
    void run();    // CLI entry point

    // variables
    std::vector<Assets*> assets;     // Stores all the assets available in the system

private:
    // ================
    // CORE UI METHODS
    // ================
    int homepage();
    int role_selection_menu();         // role selection menu
    void create_account(int roleChoice); 
    int log_in(int roleChoice);        // now matches .cpp

    // ==============================
    // PERSISTENT DATA LOAD 
    // ==============================
    void load_assets();
    void load_policies();
    void load_usage_log();
    void load_user_logins();

    // ================
    // HELPER FUNCTIONS
    // ================
    void update_usage_log(const std::string& message);
    bool validate_user(const std::string& email, const std::string& password, const std::string& role);
    bool load_json_safe(const std::string& path, nlohmann::json& out);
    std::string get_current_time();

    // ====================
    // MEMBER VARIABLES
    // ====================
    bool isOpen;                  // Indicates if the system is open
    User* currentUser = nullptr;  // Pointer to currently logged-in user
    std::map<std::string, std::string> systemPolicies; // Stores system policies
    nlohmann::json roleLoginJson; // Stores user login data
    nlohmann::json usage_log;     // Stores usage log data
};

#endif // SYSTEM_CONTROLLER_H