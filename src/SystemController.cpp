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
<<<<<<< HEAD
#include "User.h"
//#include "./users/FacultyResearcher.h"
=======
#include "./users/User.h"
>>>>>>> cbe6caa58aa6439ed0ce45040c3be907a4983ca3
// #include "./users/ResearchStudent.h"
// #include "./users/FacultyResearcher.h"                                   // <--- WAITING ON THESE CLASS IMPLEMENTATIONS
// #include "./users/LabManager.h"
// #include "./users/LabAssetManager.h"

#include "./library/nlohmann/json.hpp"
using json = nlohmann::json;

// Base data directory relative to src/cli (executable working dir)
static const std::string DATA_DIR = "../../data/";

/////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////
SystemController::SystemController() : isOpen(true), currentUser(nullptr) {   
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
// CLI Entry Point !!!!
/////////////////////////////////////////////////////////////////
void SystemController::run() {
    std::cout << "Welcome to the Lab Asset Management System!\n";
    update_usage_log("System started");

    while (isOpen) {
        int result = main();
        if (result == -1) {
            update_usage_log("System shutdown");
            isOpen = false;
        }
    }
}

/////////////////////////////////////////////////////////////////
// HOMEPAGE MAIN MENU
/////////////////////////////////////////////////////////////////
int SystemController::main() {
    char choiceChar;
    while (isOpen) {
        std::cout << "\n====== MAIN MENU ======\n";
        std::cout << "1. Login\n";
        // std::cout << "2. Create Account\n"; 
        std::cout << "2. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choiceChar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int roleChoice = -1; 
        bool userCreated = false;

        switch (choiceChar) {
            // handles log in
            case '1': {
                userCreated = log_in();          // <------ returns bool (T/F) if user successfully created

                std::cout << "PLACEHOLDER (uncomment once other User Classes made and uncomment in create_user()):\n";
                if (userCreated) {
                    std::cout << "User Logged in.\n";
                    isOpen = false;
                    break;  // exit switch
                } else {
                    std::cout << "Login failed or cancelled.\n";
                    return -1;
                }
            }

            // handles system exit
            case '2':
                std::cout << "Exiting system. Goodbye!\n";
                isOpen = false;
                return -1;

            // invalid input handler
            default:
                std::cout << "Invalid choice, please try again.\n";
        }
    }

    // ------ CALL currentUser's main() -------- //
    std::cout << "\nUser has been created. Calling user's main()....\n";
    // currentUser->main();    // Calls ResearchStudent::main() or LabManager::main() etc based on currentUser's actual type

    return -1;
}

/////////////////////////////////////////////////////////////////
// Log-In Handler
/////////////////////////////////////////////////////////////////
bool SystemController::log_in() {
    const int MAX_ATTEMPTS = 3;
    std::string firstName, lastName, email, password;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        std::cout << "\nPlease enter your login credentials:\n";             // had before ... (" + ROLE_NAMES[roleChoice] + "):\n";
        std::cout << "First Name: ";
        std::getline(std::cin, firstName);
        std::cout << "Last Name: ";
        std::getline(std::cin, lastName);
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        std::string role = validate_user(firstName, lastName, email, password);   // validate user step (check accounts.json for user existence)
        
        if (role != "") {
            // update usage log
            update_usage_log("User logged in: " + firstName + " " + lastName + " " + email);
            userActive = true;

            std::cout << "CHECK:: User is a " << role << std::endl;

            // --------------- Create User instance -------------- //
            currentUser = create_user(firstName, lastName, email, role);
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

/////////////////////////////////////////////////////////////////
// Validate Login Credentials
/////////////////////////////////////////////////////////////////
std::string SystemController::validate_user(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& password) {
  
    // roleLoginJson is assumed to be a JSON array
    if (!roleLoginJson.is_array()) {
        std::cerr << "Error: login JSON is not an array.\n";
        return "";
    }

    for (const auto& user : roleLoginJson) {
        // Safely check all fields
        if (!user.contains("firstName") || !user.contains("lastName") ||
            !user.contains("email")     || !user.contains("password") ||
            !user.contains("role"))
            continue;

        if (user["firstName"].get<std::string>() == firstName &&
            user["lastName"].get<std::string>() == lastName &&
            user["email"].get<std::string>() == email &&
            user["password"].get<std::string>() == password)
        {
            return user["role"].get<std::string>();          // <--- changed to return a string that gets the users assigned role
        }
    }

    return "";
}

/////////////////////////////////////////////////////////////////
// Create User Instance
/////////////////////////////////////////////////////////////////
User* SystemController::create_user(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& role) {

    // ------ !!!!!!!!!!!! WAITING ON IMPLEMENTATION OF USER CLASSES -------- //

    if (role == "research student") {
        std::cout << "Creating ResearchStudent instance for " << firstName << " " << lastName << "\n";
        // currentUser = new ResearchStudent(firstName, lastName, email, this);
    }
    // else if (role == "faculty researcher") {
    //     currentUser = new FacultyResearcher(firstName, lastName, email, this);
    // }
    // else if (role == "lab manager") {
    //     currentUser = new LabManager(firstName, lastName, email, this);
    // }
    // else if (role == "lab asset manager") {
    //     currentUser = new LabAssetManager(firstName, lastName, email, this);
    // }
    else {
        std::cerr << "Unknown role (*UNCOMMENT OTHERS^^): " << role << "\n";
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
// Safety checker for/before data loading
// This function safely loads and parses a JSON file from 
// a given path, logging errors if the file cannot be opened, is empty, or contains invalid JSON.
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

void SystemController::load_user_logins() {
    std::string path = DATA_DIR + "accounts.json";
    if (!load_json_safe(path, roleLoginJson)) {
        // initialize as empty array, not object
        roleLoginJson = json::array();
        std::ofstream out(path);
        if (out.is_open()) out << roleLoginJson.dump(4);
    }
}

void SystemController::load_assets() {
//     json assetJson;
//     std::string path = DATA_DIR + "assets.json";

//     if (!load_json_safe(path, assetJson))
//         return;

//     for (auto& a : assetJson) {
//         int id = a.value("id", -1);
//         std::string name = a.value("name", "UNKNOWN");
//         std::string location = a.value("location", "UNKNOWN");
//         std::string category = a.value("category", "equipment");
//         int accessLevel = a.value("accessLevel", 1);

//         Assets* newAsset = nullptr;

//         if (category == "consumable") {
//             // Safely parse quantities (string or int)
//             int quantity = 0;
//             int threshold = 0;

//             if (a.contains("quantityOnHand(grams)")) {
//                 quantity = std::stoi(a["quantityOnHand(grams)"].get<std::string>());
//             } else {
//                 quantity = a.value("quantityOnHand", 0);
//             }

//             if (a.contains("minimumThreshold(grams)")) {
//                 threshold = std::stoi(a["minimumThreshold(grams)"].get<std::string>());
//             } else {
//                 threshold = a.value("minimumThreshold", 0);
//             }

//             newAsset = new Consumable(id, name, location, accessLevel, quantity, threshold);
//         }
//         else {
//             // Default = Equipment
//             std::string condition = a.value("condition", "good");
//             std::string status = a.value("operationalStatus", "available");

//             Equipment* e = new Equipment(id, name, location, accessLevel, condition);

//             if (status == "maintenance" || status == "out-of-service") {
//                 e->setOutOfService(true, "Loaded as " + status);
//             }

//             newAsset = e;
//         }

//         assets.push_back(newAsset);
//     }

//     std::cout << "Loaded " << assets.size() << " assets from assets.json\n";
    std::cout << "LOAD ASSETS: Function not implemented yet (waiting on Asset classes)\n";
}

void SystemController::load_policies() {
    // std::cerr << "DEBUG: load_policies() called\n";
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
