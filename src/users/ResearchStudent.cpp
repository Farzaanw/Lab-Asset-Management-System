//CS-2: ResearchStudent.cpp
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "ResearchStudent.h"
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"
static Assets a;
static Reservations r;
using namespace std;
using json = nlohmann::json;

//Constructor
ResearchStudent::ResearchStudent(const std::string& email,
                                 SystemController* sys)
    : User(email, sys),
      system(sys) {}

//Destructor
ResearchStudent::~ResearchStudent() {}

//Override getRole
std::string ResearchStudent::getRole() const {
    return "research student";
}

//Main menu
void ResearchStudent::main() {
    cout << "\n=============================================" << endl;
    cout << "Welcome " << getEmail() << endl;
    cout << "=============================================\n" << endl;

    while(true) {
        cout << endl << "---Research Student Main Menu---" << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Return Asset" << endl;
        cout << "3. View Available Assets" << endl;
        //cout << "4. View My Assets" << endl;
        cout << "4. Search/Filter Assets" << endl;
        cout << "5. View My Reservations" << endl;
        cout << "6. Cancel Reservation" << endl;
        cout << "7. Submit Usage Feedback" << endl;
        cout << "8. Update Profile" << endl;
        cout << "9. Logout" << endl;
        cout << "Please enter your choice: ";
        
        string choice;
        getline(cin, choice);
        cout << endl;

        if (choice == "1") {
            ///////////////////
            string email = getEmail();
            if (r.reserveAsset(email)){
                cout << "Asset officially reserved." << endl;
            } else {
                cout << "Asset reservation failed or request sent to Lab Manager for approval." << endl;
                cout << "^Check which one true ---- Implment SEND NOTIFICATION TO LAB MANAGER in reserveAsset() function." << endl;
            }
            ///////////////////////////
        }
        else if (choice == "2") {
            string email = getEmail();
            a.return_asset(email);
        }
        else if (choice == "3") {
            a.viewAvailableAssets();
        }
        /*
        else if (choice == "4") {
            string email = getEmail();
            a.viewAssets(email);
        }
        */
        else if (choice == "4") {
            a.searchAssets("", ""); // Will prompt inside function
        }
        else if (choice == "5") {
            string email = getEmail();
            r.viewMyReservations(email);
        }
        else if (choice == "6") {
            string email = getEmail();
            r.cancelReservation(email); // Will prompt inside function
        }
        else if (choice == "7") {
            int equipmentID, rating;
            string comments;
            cout << "Enter Equipment ID: ";
            cin >> equipmentID;
            cin.ignore();
            cout << "Enter comments: ";
            getline(cin, comments);
            cout << "Enter rating (1-5): ";
            cin >> rating;
            cin.ignore();
            submitUsageFeedback(equipmentID, comments, rating);
        }
        else if (choice == "8") {
            string newEmail;
            cout << "Enter new email: ";
            getline(cin, newEmail);
            updateUserProfile(newEmail);
        }
        else if (choice == "9") {
            cout << "Logging out of Research Student." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}



// Append a usage log entry describing the reservation
bool ResearchStudent::appendUsageLog(const std::string& email, int assetID, const std::string& startTime, const std::string& endTime) {
    const std::string path = "../../data/usage_log.json";
    json doc;

    // Debug: print current working directory and resolved path to help diagnose write failures
    try {
        auto cwd = std::filesystem::current_path();
        std::cerr << "[appendUsageLog] CWD=" << cwd.string() << "; resolving '" << path << "' -> "
                  << std::filesystem::absolute(path).string() << std::endl;
        std::cerr << "[appendUsageLog] target exists=" << (std::filesystem::exists(path) ? "yes" : "no") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[appendUsageLog] filesystem error: " << e.what() << std::endl;
    }

    // Load existing file (if any)
    std::ifstream in(path);
    if (in.is_open()) {
        try {
            in >> doc;
        } catch (const std::exception& e) {
            // malformed file: start fresh
            doc = json::object();
        }
        in.close();
    } else {
        doc = json::object();
    }

    // Ensure we have an array to store usage entries. Use key "usage" so we don't clobber existing "events".
    if (!doc.contains("usage") || !doc["usage"].is_array()) {
        doc["usage"] = json::array();
    }

    // Create entry
    json entry;
    entry["email"] = email;
    entry["assetID"] = assetID;
    entry["start"] = startTime;
    entry["end"] = endTime;

    // add a timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    entry["loggedAt"] = ss.str();

    doc["usage"].push_back(entry);

    // If we have a SystemController pointer, prefer using it so the in-memory
    // usage_log is updated as well; this prevents later overwrites from removing
    // usage entries.
    // if (system != nullptr) {
    //     try {
    //         return system->appendUsageEntry(entry);
    //     } catch (const std::exception& e) {
    //         cerr << "Error: system->appendUsageEntry threw: " << e.what() << endl;
    //         // fall through to direct write
    //     }
    // }

    // Fallback: write directly to file (legacy behavior)
    std::ofstream out(path);
    if (!out.is_open()) {
        cerr << "Error: Could not open usage log for writing: " << path << endl;
        return false;
    }

    try {
        out << setw(4) << doc << endl;
        out.close();
    } catch (const std::exception& e) {
        cerr << "Error writing usage log: " << e.what() << endl;
        if (out.is_open()) out.close();
        return false;
    }

    return true;
}


//STUDENT SPECIFIC FEATURES
//record feedback on an asset after use
bool ResearchStudent::submitUsageFeedback(int equipmentID, const std::string& comments, int rating) {
    cout << "--- Submit Usage Feedback ---\n" << endl;
    
    if (rating < 1 || rating > 5) {
        cout << "Error: Rating must be between 1 and 5!" << endl;
        return false;
    }
    
    // Load existing feedback
    json feedbackData;
    ifstream inFile("../../data/feedback.json");
    if (inFile.is_open()) {
        inFile >> feedbackData;
        inFile.close();
    } else {
        feedbackData = json::array();
    }
    
    // Create new feedback entry
    json newFeedback = {
        {"equipmentID", equipmentID},
        {"comments", comments},
        {"rating", rating},
        {"timestamp", time(nullptr)}
    };
    
    feedbackData.push_back(newFeedback);
    
    // Save back to file
    ofstream outFile("../../data/feedback.json");
    outFile << setw(4) << feedbackData << endl;
    outFile.close();
    
    cout << "Feedback submitted successfully!" << endl;
    return true;
}

//update profile information
bool ResearchStudent::updateUserProfile(const std::string& newEmail) {
    cout << "--- Update User Profile ---\n" << endl;
    
    json accounts;
    ifstream inFile("../../data/accounts.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    inFile >> accounts;
    inFile.close();
    
    // Find and update the student's profile
    bool found = false;
    for (auto& account : accounts) {
        if (account["email"] == getEmail() && account["role"] == "research student") {
            account["email"] = newEmail;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Error: Could not find user profile!" << endl;
        return false;
    }
    
    // Save back
    ofstream outFile("../../data/accounts.json");
    outFile << setw(4) << accounts << endl;
    outFile.close();
    
    cout << "Profile updated successfully!" << endl;
    return true;
}