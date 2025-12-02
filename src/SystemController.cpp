// SystemController.cpp
// Auth: Farzaan Wadiwalla

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <limits>
#include <cctype>
#include "SystemController.h"
#include "User.h"

#include "./library/nlohmann/json.hpp"
using json = nlohmann::json;

// Base data directory relative to src/cli (executable working dir)
static const std::string DATA_DIR = "../../data/";

/////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////
SystemController::SystemController() : isOpen(true), currentUser(nullptr) {   
    // load all persistent data
    load_usage_log();
    load_user_logins();
    load_assets();
    load_policies();

    // If usage_log.json was empty or failed to load, initialize it
    if (usage_log.is_null() || !usage_log.contains("events")) {
        usage_log = nlohmann::json::object();
        usage_log["events"] = nlohmann::json::array();
    }
}

/////////////////////////////////////////////////////////////////
// Safety checker for/before data loading
/////////////////////////////////////////////////////////////////
bool SystemController::load_json_safe(const std::string& path, json& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << path << "\n";
        update_usage_log("ERROR: Failed to load JSON: " + path);
        return false;
    }

    // Read whole file into a string so we can detect empty/whitespace-only files
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // If file is empty or contains only whitespace, treat as missing
    auto it = std::find_if_not(content.begin(), content.end(), [](unsigned char c){ return std::isspace(c); });
    if (it == content.end()) {
        std::cerr << "JSON file is empty: " << path << "\n";
        return false;
    }

    try {
        out = json::parse(content);
        std::cout << "JSON loaded successfully from " << path << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error in " << path << ": " << e.what() << "\n";
        return false;
    }
}

/////////////////////////////////////////////////////////////////
// CLI Entry Point
/////////////////////////////////////////////////////////////////

void SystemController::run() {
    std::cout << "Welcome to the Lab Asset Management System!\n";
    update_usage_log("System started");

    while (isOpen) {
        int result = homepage();
        if (result == -1) {
            update_usage_log("System shutdown");
            isOpen = false;
        }
    }
}

/////////////////////////////////////////////////////////////////
// HOMEPAGE MAIN MENU
/////////////////////////////////////////////////////////////////
int SystemController::homepage() {
    char choiceChar;
    while (isOpen) {
        std::cout << "\n====== MAIN MENU ======\n";
        std::cout << "1. Login\n";
        std::cout << "2. Create Account\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice: ";

        std::cin >> choiceChar;
        int roleChoice = -1; 

        switch (choiceChar) {
            case '1': {
                int roleChoice = role_selection_menu();
                if (roleChoice != -1) log_in(roleChoice);
                isOpen = false;
                break;
            }
            case '2': {
                int roleChoice = role_selection_menu();
                if (roleChoice != -1) create_account(roleChoice);
                isOpen = false;
                break;
            }
            case '3':
                std::cout << "Exiting system. Goodbye!\n";
                isOpen = false;
                return -1;
            default:
                std::cout << "Invalid choice, please try again.\n";
        }
    }

    // INITIALIZE THE USER THAT HAS SUCCESSFULLY LOGGED-IN //

    std::cout << "User has successfully logged in or created an account.\n";
    std::cout << "Now need to instatiate user object here - not implmented yet.\n";
    return -1;
}

////////////////////////////////////////////////////////////////
// Account Creation Handler
/////////////////////////////////////////////////////////////////
int SystemController::role_selection_menu() {
    std::cout << "\nSelect Your Role:\n";
    std::cout << "1. Student Researcher\n";
    std::cout << "2. Faculty Researcher\n";
    std::cout << "3. Lab Manager\n";
    std::cout << "4. Lab Asset Manager\n";
    std::cout << "Enter choice (1-4): ";

    int r;
    std::cin >> r;

    // Invalid numeric or out-of-range
    if (std::cin.fail() || r < 1 || r > 4) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid role selection.\n";
        update_usage_log("Invalid role selection attempted");
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
    return r;
}

/////////////////////////////////////////////////////////////////
// Log-In Handler
/////////////////////////////////////////////////////////////////
int SystemController::log_in(int roleChoice) {
    const int MAX_ATTEMPTS = 3;
    std::string email, password;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        std::cout << "\nPlease enter your login credentials:\n";
        std::cout << "Email: ";
        std::cin >> email;
        std::cout << "Password: ";
        std::cin >> password;

        bool ok = validate_user(email, password, std::to_string(roleChoice));

        if (ok) {
            update_usage_log("User logged in: " + email);
            return 1; // Successful login
        } else {
            std::cout << "Invalid credentials. Attempt " << attempt 
                      << " of " << MAX_ATTEMPTS << ".\n";
            update_usage_log("Failed login attempt for email: " + email);
        }
    }

    std::cout << "Maximum login attempts reached. Exiting.\n";
    return 0; // Failed login after 3 attempts
}


//////////////////////////////////////////////////////////////////
// Create Account Handler
//////////////////////////////////////////////////////////////////
void SystemController::create_account(int roleChoice) {
    // get user --> string version for putting under correct section in json file
    std::string user = "";
    if (roleChoice == 1) {
        user = "Student Researcher";
    } else if (roleChoice == 2) {
        user = "Faculty Researcher";
    } else if (roleChoice == 3) {
        user = "Lab Manager";
    } else if (roleChoice == 4) {
        user = "Lab Asset Manager";
    } else {
        std::cout << "Invalid role selection.\n";   // just in case (already handled earlier)
        return;
    }

    std::cout << "\nCreate your account below:\n";
    std::string email, password;
    std::cout << "Enter email: ";
    std::cin >> email;
    std::cout << "Enter password: ";
    std::cin >> password;

    json userEntry;
    userEntry["email"] = email;
    userEntry["password"] = password;

    roleLoginJson[user].push_back(userEntry);

    // Save immediately
    std::ofstream file(DATA_DIR + std::string("user_logins.json"));
    file << roleLoginJson.dump(4);

    update_usage_log("Created account: " + email);
}

/////////////////////////////////////////////////////////////////
// Validate Login Credentials
/////////////////////////////////////////////////////////////////
bool SystemController::validate_user(const std::string& email, const std::string& password, const std::string& role) {
    // get user --> string version for json lookup
    std::string user = "";
    if (role == "1") {
        user = "Student Researcher";
    } else if (role == "2") {
        user = "Faculty Researcher";
    } else if (role == "3") {
        user = "Lab Manager";
    } else if (role == "4") {
        user = "Lab Asset Manager";
    } else {
        return false;
    }

    if (!roleLoginJson.contains(user)) return false; // check role exists
    std::cout << "User exists!\n";

    for (auto& u : roleLoginJson[user]) {
        if (u["email"] == email && u["password"] == password) {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////
// Load User Logins from JSON
//////////////////////////////////////////////////////////////////
void SystemController::load_user_logins() {
    std::string path = DATA_DIR + std::string("user_logins.json");
    if (!load_json_safe(path, roleLoginJson)) {
        // initialize default structure and create the file
        roleLoginJson = json::object();
        roleLoginJson["users"] = json::array();
        std::ofstream out(path);
        if (out.is_open()) out << roleLoginJson.dump(4);
    }
}

void SystemController::load_assets() {
    // json assetJson;

    // if (!load_json_safe("data/assets.json", assetJson))
    //     return;

    // for (auto& a : assetJson) {
    //     Assets* newAsset = new Assets(
    //         a.value("id", -1),
    //         a.value("name", "UNKNOWN"),
    //         a.value("location", "UNKNOWN"),
    //         std::stoi(a.value("accessLevel", "0"))
    //     );

    //     assets.push_back(newAsset);
    // }
    std::cout << "Currently not loading assets (uncoment method in SystemController.cpp - wwaiting on Asset Intialization!!).\n";
}

void SystemController::load_policies() {
    json j;
    std::string path = DATA_DIR + std::string("policies.json");
    if (!load_json_safe(path, j)) {
        // no policies file; leave policies empty and create an empty file
        std::ofstream out(path);
        if (out.is_open()) out << json::object().dump(4);
        return;
    }

    for (auto& it: j.items()) {
        systemPolicies[it.key()] = it.value();
    }

    std::cout << "................" << std::endl;
}

void SystemController::load_usage_log() {
    std::string path = DATA_DIR + std::string("usage_log.json");
    if (!load_json_safe(path, usage_log)) {
        // initialize and create file
        usage_log = json::object();
        usage_log["events"] = json::array();
        std::ofstream out(path);
        if (out.is_open()) out << usage_log.dump(4);
        return;
    }

    if (!usage_log.contains("events"))
        usage_log["events"] = json::array();
}

void SystemController::update_usage_log(const std::string& message) {
    json entry;
    entry["timestamp"] = get_current_time();
    entry["event"] = message;

    usage_log["events"].push_back(entry);

    std::ofstream out(DATA_DIR + std::string("usage_log.json"));
    out << std::setw(4) << usage_log;
}

// Helper to get current time as string
std::string SystemController::get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
