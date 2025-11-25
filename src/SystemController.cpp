// SystemController.cpp
// Auth: Farzaan Wadiwalla

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <limits>
#include "SystemController.h"
// #include "User.h"

#include "./library/nlohmann/json.hpp"
using json = nlohmann::json;

/////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////
SystemController::SystemController()
    : isOpen(true), currentUser(nullptr)
{
    load_login_json();

    // Initialize usage log if empty
    usage_log = json::object();
    usage_log["events"] = json::array();
    static const std::string ROLES[] = {
        "INVALID",
        "Student Researcher",
        "Faculty Researcher",
        "Lab Manager",
        "Lab Asset Manager"
    };
}

/////////////////////////////////////////////////////////////////
// load user log-in data
/////////////////////////////////////////////////////////////////
bool SystemController::load_json_safe(const std::string& path, json& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << path << "\n";
        return false;
    }
    try {
        file >> out;
        std::cout << "JSON loaded successfully from " << path << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return false;
    }
}

// Load login JSON (uses helper)
void SystemController::load_login_json() {
    if (!load_json_safe("../../data/user_logins.json", roleLoginJson)) {
        // fallback default structure
        roleLoginJson = {
            {"Student Researcher", json::array()},
            {"Faculty Researcher", json::array()},
            {"Lab Manager", json::array()},
            {"Lab Asset Manager", json::array()}
        };
        std::cout << "Initialized roleLoginJson with default values.\n";
    }
}

/////////////////////////////////////////////////////////////////
// Timestamp helper
/////////////////////////////////////////////////////////////////
std::string SystemController::get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/////////////////////////////////////////////////////////////////
// Usage log writer
/////////////////////////////////////////////////////////////////
void SystemController::update_usage_log(const std::string& message) {
    json entry;
    entry["timestamp"] = get_current_time();
    entry["event"] = message;

    usage_log["events"].push_back(entry);

    std::ofstream out("../../data/usage_log.json");
    out << std::setw(4) << usage_log;
}

/////////////////////////////////////////////////////////////////
// CLI Entry Point
/////////////////////////////////////////////////////////////////
void SystemController::run() {
    std::cout << "Welcome to the Lab Asset Management System!\n";
    homepage();   // This now loops internally until exit
}

/////////////////////////////////////////////////////////////////
// Homepage Menu
/////////////////////////////////////////////////////////////////
int SystemController::homepage() {
    int choice = 0;
    // repeat until proper input 
    while (true) {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Create Account\n";
        std::cout << "2. Log In\n";
        std::cout << "3. Exit\n";
        std::cout << "Select (enter #): ";

        std::cin >> choice;

        if (std::cin.fail()) {
            // User entered something that is not a number
            std::cin.clear(); // clear the fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
            continue; // prompt again
        }

        // Valid number, check range
        if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Please select 1, 2, or 3.\n";
            continue;
        }

        // Choice is valid, break loop
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // remove any leftover
        break;
    }

    // Process choice
    while (isOpen) {
        switch (choice) {
            // Account Creation Flow
            case 1:
                {
                    int roleChoice = role_selection_menu();
                    if (roleChoice != -1) {
                        create_account(roleChoice);
                    }
                }
                isOpen = false; 
                break;
            
            // Log-In Flow
            case 2:
                {
                    int roleChoice = role_selection_menu();
                    if (roleChoice != -1) {
                        log_in(roleChoice);
                    }
                }
                isOpen = false; 
                break;

            // Exit
            case 3:
                std::cout << "Exiting system. Goodbye!\n";
                isOpen = false;
                break;
                
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////
// Account Creation Handler
/////////////////////////////////////////////////////////////////
int SystemController::role_selection_menu() {
    int roleChoice = 0;

    std::cout << "\nSelect account role to create:\n";
    std::cout << "1. Student Researcher\n";
    std::cout << "2. Faculty Researcher\n";
    std::cout << "3. Lab Manager\n";
    std::cout << "4. Lab Asset Manager\n";
    std::cout << "Enter number: ";

    std::cin >> roleChoice;

    // Invalid numeric or out-of-range
    if (std::cin.fail() || roleChoice < 1 || roleChoice > 4) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid role selection.\n";
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
    return roleChoice;
}

//////////////////////////////////////////////////////////////////
// Create Account Handler
//////////////////////////////////////////////////////////////////
void SystemController::create_account(int roleChoice) {
    static const std::string ROLES[] = {
        "INVALID",
        "Student Researcher",
        "Faculty Researcher",
        "Lab Manager",
        "Lab Asset Manager"
    };
    std::string role = ROLES[roleChoice];

    std::string firstName, lastName, email, password, confirmPassword;

    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);
    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Create password: ";
    std::getline(std::cin, password);
    std::cout << "Confirm password: ";
    std::getline(std::cin, confirmPassword);

    if (password != confirmPassword) {
        std::cout << "Passwords do not match. Account creation failed.\n";
        return;
    }

     // Add user to in-memory JSON
    json newUser = { {"first_name", firstName}, {"last_name", lastName}, {"email", email}, {"password", password} };
    roleLoginJson[role].push_back(newUser);

    // Save updated login info to file
    std::ofstream file("../../data/user_logins.json");
    if (file.is_open()) {
        file << roleLoginJson.dump(4);  // pretty print
        std::cout << "Account created successfully for " << email << "\n";
    } else {
        std::cerr << "Failed to save user_logins.json\n";
    }

    // Update usage log
    std::string logMessage = email + " created an account as " + role;
    update_usage_log(logMessage);

    // Optionally, log them in immediately
    log_in(roleChoice);
}

/////////////////////////////////////////////////////////////////
// Log-In Handler
/////////////////////////////////////////////////////////////////
int SystemController::log_in(int roleChoice) {
    static const std::string ROLES[] = {
        "INVALID",
        "Student Researcher",      // 1
        "Faculty Researcher",      // 2
        "Lab Manager",             // 3
        "Lab Asset Manager"        // 4
    };

    std::string role = ROLES[roleChoice];
    std::cout << "\nLogging in as " << role << ":\n";

    // Attempt login up to 3 times
    for (int attempts = 3; attempts > 0; attempts--) {
        std::string email, password;

        std::cout << "Enter email: ";
        std::getline(std::cin, email);

        std::cout << "Enter password: ";
        std::getline(std::cin, password);

        if (validate_user(email, password, role)) {
            update_usage_log(email + " logged in as " + role);
            return roleChoice;
        }

        std::cout << "Login failed. Attempts remaining: " << (attempts - 1) << "\n";
    }

    std::cout << "Too many failed attempts. Exiting.\n";
    exit(0);
}

/////////////////////////////////////////////////////////////////
// Validate Login Credentials
/////////////////////////////////////////////////////////////////
bool SystemController::validate_user(const std::string& email, const std::string& password, const std::string& role) {
    if (!roleLoginJson.contains(role))
        return false;

    for (const auto& user : roleLoginJson[role]) {
        if (user["email"] == email && user["password"] == password) {
            std::cout << "Login successful!\n";
            return true;
        }
    }
    std::cout << "Invalid email or password.\n";
    return false;
}