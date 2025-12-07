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
#include <time.h>

// needed for lockdown upon incorrect user login
#ifdef _WIN32
    #include <windows.h>  // for Sleep() on Windows
    #define sleep(seconds) Sleep((seconds) * 1000)
#else
    #include <unistd.h>  // for sleep() on Unix/Linux   
#endif

// class includes
#include "SystemController.h"
#include "./users/User.h"
#include "./users/ResearchStudent.h"
#include "./users/FacultyResearcher.h"
#include "./users/LabManager.h"
#include "./users/LabAssetManager.h"

#include "./library/nlohmann/json.hpp"
using json = nlohmann::json;

// Base data directory relative to src/cli (executable working dir)
static const std::string DATA_DIR = "../../data/";

/////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////
SystemController::SystemController() : isOpen(true), currentUser(nullptr) {   
    load_usage_log();
    load_accounts();
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
                if (userCreated) {
                    std::cout << "User Logged in.\n";
                    isOpen = false;
                    break;  // exit switch
                } else {
                    std::cout << "...Login failed or cancelled....\n";
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
    // std::cout << "...User has been created. Calling user's main()...\n";
    currentUser->main();    // Calls ResearchStudent::main() or LabManager::main() etc based on currentUser's actual type

    return -1;
}

/////////////////////////////////////////////////////////////////
// Log-In Handler
/////////////////////////////////////////////////////////////////
bool SystemController::log_in() {
    time_t start = time(NULL);

    const int MAX_ATTEMPTS = 5;
    std::string email, password;

    // user gets max 5 attempts to log in
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        std::cout << "\nPlease enter your login credentials:\n";             
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        std::string role = validate_user(email, password);   // validate user step (check accounts.json for user existence)

        // roleLoginJson is assumed to be a JSON array
        if (!roleLoginJson.is_array()) {
            std::cerr << "Error: login JSON is not an array.\n";
            return "";
        }

        // if user exists
        if (role != "") {
            // update usage log
            update_usage_log("User logged in: " + email);
            userActive = true;

            std::cout << "CHECK:: User is a " << role << std::endl;

            // --------------- Create User instance -------------- //
            currentUser = create_user(email, role);
            return currentUser != nullptr;
            // ----------------------------------------------------- //

        } else {
            std::cout << "Invalid credentials. Attempt " << attempt << " of " << MAX_ATTEMPTS << ".\n";
            update_usage_log("Failed login attempt for email: " + email);
        }
    }

    // FR-9: Account lockout after max attempts within 5 minutes
    time_t end = time(NULL);
    double minutes = difftime(end, start) / 60.0;
    if (minutes < 5) {
        printf("*Account locked for 10 minutes*\n");

        // Create notification to send to Lab Asset Manager
        json notification = {
            {"message", "Multiple failed login attempts detected for email: " + email},
            {"type", "security_alert"},
            {"timeStamp", get_current_time()},
            {"metaData", {
                {"email", email},
                {"attempts", MAX_ATTEMPTS}
            }}
        };

        // ----------------------- send message to LAM
        Notifications notif;
        notif.send_notifications("lab asset manager", notification);
        // ------------------------------ //

        // pause for 10 minutes
        sleep(600); 

        return log_in(); // retry login after lockout
    } else {
        std::cout << "Maximum login attempts reached. Exiting.\n";
        return false; // failed login after 3 attempts
    }
}

/////////////////////////////////////////////////////////////////
// Validate Login Credentials
/////////////////////////////////////////////////////////////////
std::string SystemController::validate_user(const std::string& email, const std::string& password) {
    // roleLoginJson is assumed to be a JSON array
    if (!roleLoginJson.is_array()) {
        std::cerr << "Error: login JSON is not an array.\n";
        return "";
    }

    for (const auto& user : roleLoginJson) {
        // Safely check all fields
        if (!user.contains("email") || !user.contains("password") ||
            !user.contains("role"))
            continue;

        if (user["email"].get<std::string>() == email && user["password"].get<std::string>() == password){
            return user["role"].get<std::string>();          // <--- changed to return a string that gets the users assigned role
        }
    }

    return "";
}

/////////////////////////////////////////////////////////////////
// Create User Instance
/////////////////////////////////////////////////////////////////
User* SystemController::create_user(const std::string& email, const std::string& role) {
    if (role == "research student") {
        // std::cout << "Creating ResearchStudent instance for " << email <<  "\n";
        currentUser = new ResearchStudent(email, this);
        return currentUser;
    }
    else if (role == "faculty researcher") {
        // std::cout << "Creating ResearchStudent instance for " << email << "\n";
        currentUser = new FacultyResearcher(email, this);
        return currentUser;
    }
    else if (role == "lab manager") {
        // std::cout << "Creating LabManager instance for " << email << "\n";
        currentUser = new LabManager(email, this);
        return currentUser;
    }
    else if (role == "lab asset manager") {
        // std::cout << "Creating LabAssetManager instance for " << email << "\n";
        currentUser = new LabAssetManager(email, this);
        return currentUser;
    }
    else {
        std::cerr << "System (new role or role doesn't exist): " << role << "\n";
        return nullptr;
    }

    // std::cout << "^^PLACEHOLDER (INTATIATING USER HERE -- WAITING ON CLASS IMPLEMENTATION - then uncomment; return null_ptr for now:).\n";
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

void SystemController::load_accounts() {
    std::string path = DATA_DIR + "accounts.json";
    if (!load_json_safe(path, roleLoginJson)) {
        // initialize as empty array, not object
        roleLoginJson = json::array();
        std::ofstream out(path);
        if (out.is_open()) out << roleLoginJson.dump(4);
    }
}

void SystemController::load_assets() {
    json assetJson;
    std::string path = DATA_DIR + "assets.json";

    if (!load_json_safe(path, assetJson))
        return;

    std::cout << "Loaded " << assetJson.size() << " assets from assets.json\n";
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

    // Write in-memory usage_log (includes both "events" and "usage" arrays)
    try {
        std::string path = DATA_DIR + std::string("usage_log.json");
        std::ofstream out(path);
        if (!out.is_open()) {
            std::cerr << "Warning: Could not open usage_log.json for writing: " << path << std::endl;
            return;
        }
        out << std::setw(4) << usage_log;
        out.close();
    } catch (const std::exception& e) {
        std::cerr << "Exception while updating usage log: " << e.what() << std::endl;
    }
}

// Append a usage entry (reservation) into the central usage_log and persist it.
// bool SystemController::appendUsageEntry(const nlohmann::json& entry) {
//     // Ensure events exists (update_usage_log already does this on startup)
//     if (!usage_log.is_object()) usage_log = json::object();

//     if (!usage_log.contains("usage") || !usage_log["usage"].is_array())
//         usage_log["usage"] = json::array();

//     usage_log["usage"].push_back(entry);

//     // Write back to disk, preserving events (usage_log already contains events)
//     try {
//         std::string path = DATA_DIR + std::string("usage_log.json");
//         std::ofstream out(path);
//         if (!out.is_open()) {
//             std::cerr << "Error: Could not open usage_log.json for appending usage entry: " << path << std::endl;
//             return false;
//         }
//         out << std::setw(4) << usage_log;
//         out.close();
//         return true;
//     } catch (const std::exception& e) {
//         std::cerr << "Exception writing usage log: " << e.what() << std::endl;
//         return false;
//     }
// }

// Helper to get current time as string (for usage log timestamps)
std::string SystemController::get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
