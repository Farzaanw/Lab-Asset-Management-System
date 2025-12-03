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

// class includes
#include "SystemController.h"
#include "User.h"
#include "./users/FacultyResearcher.h"
// #include "./users/ResearchStudent.h"
// #include "./users/FacultyResearcher.h"                                   // <--- WAITING ON THESE CLASS IMPLEMENTATIONS
// #include "./users/LabManager.h"
// #include "./users/LabAssetManager.h"

#include "./library/nlohmann/json.hpp"
using json = nlohmann::json;

// Base data directory relative to src/cli (executable working dir)
static const std::string DATA_DIR = "../../data/";
static const std::string ROLE_NAMES[] = {
    "",                     // dummy placeholder so index matches roleChoice (1/2/3/4)
    "Student Researcher",   // 1
    "Faculty Researcher",   // 2
    "Lab Manager",          // 3
    "Lab Asset Manager"     // 4
};

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

    try {
        file >> out; // parse directly from stream
        if (out.is_null()) {
            std::cerr << "JSON file is empty: " << path << "\n";
            return false;
        }
        std::cout << "JSON loaded successfully from " << path << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error in " << path << ": " << e.what() << "\n";
        return false;
    }
}

/////////////////////////////////////////////////////////////////
// CLI Entry Point !!!!
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
        bool userCreated = false;

        switch (choiceChar) {
            // handles log in
            case '1': {
                int roleChoice = role_selection_menu();
                if (roleChoice != -1){
                    userCreated = log_in(roleChoice);   // returns bool (T/F) if user successfully created
                }

                std::cout << "Below should show failure message right now - PLACEHOLDER (uncomment once other User Classes made):\n";
                if (userCreated) {
                    std::cout << "User Logged in.\n";
                    isOpen = false;
                    break;
                } else {
                    std::cout << "Login failed or cancelled.\n";
                    return -1;
                }
            }

            // handles account creation
            case '2': {
                int roleChoice = role_selection_menu();
                if (roleChoice != -1){
                    userCreated = create_account(roleChoice);
                }

                std::cout << "Below should show failure message right now - PLACEHOLDER (uncomment once other User Classes made):\n";
                if (userCreated) {
                    std::cout << "User Created Account (automatic login).\n";
                    isOpen = false;
                    break;
                } else {
                    std::cout << "Account creation failed or cancelled.\n";
                    return -1;
                }
            }

            // handles system exit
            case '3':
                std::cout << "Exiting system. Goodbye!\n";
                isOpen = false;
                return -1;

            // invalid input handler
            default:
                std::cout << "Invalid choice, please try again.\n";
        }
    }

    // INITIALIZE THE USER THAT HAS SUCCESSFULLY LOGGED-IN //
    std::cout << "\nUser has been created. Done.\n";
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

    // Invalid numeric or out-of-range                                                                  < ------------ (ASK PROFESSOR IF APPROPRIATE)                         
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
bool SystemController::log_in(int roleChoice) {
    const int MAX_ATTEMPTS = 3;
    std::string firstName, lastName, email, password;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        std::cout << "\nPlease enter your login credentials (" + ROLE_NAMES[roleChoice] + "):\n";
        std::cout << "First Name: ";
        std::getline(std::cin, firstName);
        std::cout << "Last Name: ";
        std::getline(std::cin, lastName);
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        bool ok = validate_user(email, password, roleChoice);
        if (ok) {
            // update usage log
            update_usage_log("User logged in: " + firstName + " " + lastName + " " + email);
            userActive = true;

            // --------------- Create User instance -------------- //
            currentUser = create_user(firstName, lastName, email, roleChoice);
            return currentUser != nullptr;
            // ----------------------------------------------------- //

        } else {
            std::cout << "Invalid credentials. Attempt " << attempt << " of " << MAX_ATTEMPTS << ".\n";
            update_usage_log("Failed login attempt for email: " + email);
        }
    }

    std::cout << "Maximum login attempts reached. Exiting.\n";
    return false; // failed login after 3 attempts
}


//////////////////////////////////////////////////////////////////
// Create Account Handler
//////////////////////////////////////////////////////////////////
bool SystemController::create_account(int roleChoice) {
    std::string roleName;
    if (roleChoice >= 1 && roleChoice <= 4)
        roleName = ROLE_NAMES[roleChoice]; // ROLE_NAMES defined at the top of SystemController.cpp
    else {
        std::cout << "Invalid role selection.\n";
        return false;
    }

    // user enters account details
    std::cout << "\nCreate your account below:\n";
    std::string firstName, lastName, email, password;
    std::cout << "First Name: ";
    std::getline(std::cin, firstName);
    std::cout << "Last Name: ";
    std::getline(std::cin, lastName);
    std::cout << "Email: ";
    std::getline(std::cin, email);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    json userEntry = {
        {"first_name", firstName},
        {"last_name", lastName},
        {"email", email},
        {"password", password}
    };
    roleLoginJson[roleName].push_back(userEntry);

    // Save immediately in json file
    std::ofstream file(DATA_DIR + "user_logins.json");
    file << roleLoginJson.dump(4);

    update_usage_log("Created account: " + email);

    // Automatically log in the new user
    currentUser = create_user(firstName, lastName, email, roleChoice);
    return currentUser != nullptr;
}

/////////////////////////////////////////////////////////////////
// Validate Login Credentials
/////////////////////////////////////////////////////////////////
bool SystemController::validate_user(const std::string& email, const std::string& password, int role) {
    // get user --> string version for json lookup
    std::string roleName;
    if (role >= 1 && role <= 4)
        roleName = ROLE_NAMES[role]; // ROLE_NAMES defined at the top of SystemController.cpp
    else {
        std::cout << "Invalid role selection.\n"; // just in case
        return false;
    }

    // check if role exists in json
    if (!roleLoginJson.contains(roleName)) return false; // check role exists
    std::cout << "User exists!\n";

    // search for matching email/password
    for (auto& u : roleLoginJson[roleName]) {
        if (u["email"] == email && u["password"] == password) {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////
// Create User Instance
/////////////////////////////////////////////////////////////////
User* SystemController::create_user(const std::string& first_name, const std::string& last_name, const std::string& email, int role) {
    // get user --> string version for json lookup
    std::string roleName;
    if (role >= 1 && role <= 4)
        roleName = ROLE_NAMES[role]; // ROLE_NAMES defined at the top of SystemController.cpp
    else {
        std::cout << "Invalid role selection.\n"; // just in case
        return nullptr;
    }

    if(role == 1) {
        std::cout << "implement" << std::endl;
        return nullptr;
    } else if(role == 2) {
        //return new FacultyResearcher(first_name, last_name, email, role, facultyID, this);
        std::cout << "implement" << std::endl;
        return nullptr;
    } else if(role == 3) {
        std::cout << "implement" << std::endl;
        return nullptr;
    } else if(role == 4) {
        std::cout << "implement" << std::endl;
        return nullptr;
    }
    // ------ !!!!!!!!!!!! WAITING ON IMPLEMENTATION OF USER CLASSES -------- //


    // if (role == "student") return new ResearchStudent(name, email, this);
    // if (role == "faculty") return new FacultyResearcher(name, email, this);
    // if (role == "manager") return new LabManager(name, email, password, this);
    // if (role == "assetManager") return new LabAssetManager(name, email, this);

    // ----------------------------------------------------- //


    std::cout << "^^PLACEHOLDER (INTATIATING USER HERE -- WAITING ON CLASS IMPLEMENTATION - then uncomment; return null_ptr for now:).\n";
    return nullptr;
}


//////////////////////////////////////////////////////////////////
// Load System Data !!
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
        systemPolicies[it.key()] = it.value().get<int>();
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

// Helper to get current time as string (for usage log timestamps)
std::string SystemController::get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
