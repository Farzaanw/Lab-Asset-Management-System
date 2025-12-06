#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "./library/nlohmann/json.hpp"
#include "./resources/Assets.h"
#include "./resources/Reservations.h"


#include <string>
#include <map>
#include <vector>
#include <memory>

class User;

/*
High-Level Overview of SystemController Class:
- Constructor: loads persistent JSON (usage log, user logins, assets, policies).
- load_json_safe(path, out): opens and parses a JSON file (returns false on missing / parse error).
- run() → homepage(): interactive CLI main menu (Login / Create Account / Exit).
- role_selection_menu(): pick a role (1–4).
- create_account(roleChoice): read email/password, add a user to the in-memory JSON and save file.
- log_in(roleChoice): prompt credentials, call validate_user, update usage log on success.
- validate_user(email,password,role): checks credentials against in-memory JSON.
- update_usage_log(message): append to usage_log and write file.
*/
class SystemController {
public:
    // methods
    SystemController();     // Loads all data from /data    
    void run();    // CLI entry point
    // Append a usage entry (reservation) to the in-memory log and persist to disk
    bool appendUsageEntry(const nlohmann::json& entry);

    // variables
    // std::vector<Assets*> assets;     // Stores all the assets available in the system

private:
    // ================
    // CORE UI METHODS
    // ================
    int main();
    // int role_selection_menu();   
    // bool create_account();  // removed arg: int roleChoice) 
    bool log_in();  // removed arg: int roleChoice)        
    User* create_user(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& role);    // create instance of CurrentUser

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
    std::string validate_user(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& password);
    bool load_json_safe(const std::string& path, nlohmann::json& out);
    std::string get_current_time();

    // ====================
    // MEMBER VARIABLES
    // ====================
    bool isOpen;                  // Indicates if the system is open
    bool userActive;                  // Indicates if a user is currently active / logged in

    User* currentUser = nullptr;  // Pointer to currently logged-in user
    std::map<std::string, std::string> systemPolicies; // Stores system policies
    nlohmann::json roleLoginJson; // Stores user login data
    nlohmann::json usage_log;     // Stores usage log data
};

#endif // SYSTEM_CONTROLLER_H