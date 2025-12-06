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
using namespace std;
using json = nlohmann::json;

//Constructor
ResearchStudent::ResearchStudent(const std::string& firstName,
                                 const std::string& lastName,
                                 const std::string& email,
                                 SystemController* sys)
    : User(firstName, lastName, email, sys),
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
    cout << "Welcome " << getFirstName() << " " << getLastName() << "!" << endl;
    cout << "=============================================\n" << endl;

    while(true) {
        cout << endl << "---Research Student Main Menu---" << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Return Asset" << endl;
        cout << "3. View Available Assets" << endl;
        cout << "4. View My Assets" << endl;
        cout << "5. Search/Filter Assets" << endl;
        cout << "6. View My Reservations" << endl;
        cout << "7. Cancel Reservation" << endl;
        cout << "8. Submit Usage Feedback" << endl;
        cout << "9. Update Profile" << endl;
        cout << "10. Logout" << endl;
        cout << "Please enter your choice: ";
        
        string choice;
        getline(cin, choice);
        cout << endl;

        if (choice == "1") {
            ///////////////////
            if (reserveAsset()){
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
        else if (choice == "4") {
            string email = getEmail();
            a.viewAssets(email);
        }
        else if (choice == "5") {
            a.searchAssets("", ""); // Will prompt inside function
        }
        else if (choice == "6") {
            viewMyReservations();
        }
        else if (choice == "7") {
            cancelReservation(0); // Will prompt inside function
        }
        else if (choice == "8") {
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
        else if (choice == "9") {
            string newName, newEmail;
            cout << "Enter new name: ";
            getline(cin, newName);
            cout << "Enter new email: ";
            getline(cin, newEmail);
            updateUserProfile(newName, newEmail);
        }
        else if (choice == "10") {
            cout << "Logging out of Research Student." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

//ASSETS
//reserve an asset, returns a bool
bool ResearchStudent::reserveAsset() {
    cout << "--- Reserve Asset ---\n" << endl;
    
    // Get all available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

    // ---------- get all accounts ----------
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();
    // --------------------------------------------------

    // First, show available assets
    cout << "Available Assets:\n" << endl;
    bool hasAvailable = false;
    for (const auto& asset : assets) {
        if (asset["operationalStatus"] == "available") {
            cout << "ID: " << asset["id"] << " | Name: " << asset["name"] 
                 << " | Category: " << asset["category"] << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No assets currently available for reservation." << endl;
        return false;
    }

    cout << "\n-----------------------------------\n" << endl;
    
    int assetID;
    string startDate, endDate, reason;
    
    cout << "Please enter the AssetID you would like to reserve: ";
    cin >> assetID;
    cin.ignore();

    //find the right asset
    json* targetAsset = nullptr;
    for (auto& asset : assets) {
        if (asset["id"].get<int>() == assetID) {
            targetAsset = &asset;
            break;
        }
    }
    if (!targetAsset) {
        cout << "Error: Asset ID " << assetID << " not found!" << endl;
        return false;
    }

    //check if its available
    string status = (*targetAsset)["operationalStatus"].get<string>();
    if (status != "available") {
        cout << "Error: Asset is not available!" << endl;
        cout << "Current Status: " << status << endl;
        return false;
    }

    // Get reservation dates
    cout << "Enter Start Date (YYYY-MM-DD): ";
    getline(cin, startDate);
    cout << "Enter End Date (YYYY-MM-DD): ";
    getline(cin, endDate);
    // Validate dates
    if (startDate.empty() || endDate.empty()) {
        cout << "Error: Invalid date format!" << endl;
        return false;
    }

    // --------- find the user's account info
    json* targetUser = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            targetUser = &account;
            break;
        }
    }
    // -----------------------------------------
    
    // Get user's clerance level (ADDED HERE)
    string userClearanceLevel = (*targetUser)["clearanceLevel"].get<string>();

    // Get asset clearance level
    string assetClearanceLevel = (*targetAsset)["clearanceLevel"].get<string>();
    
    // If user's clerance is lower than asset's, require reason (SEND REQUEST TO LAB MANAGER!!!!!!!!!1)
    if (std::stoi(userClearanceLevel) < std::stoi(assetClearanceLevel)) {
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
        cout << "This asset requires approval. You will be notified once reviewed." << endl;

        // Create reservation entry with "pending" status
        json reservation = {
            {"assetID", assetID},
            {"assetName", (*targetAsset)["name"]},
            {"startDate", startDate},
            {"endDate", endDate},
            {"status", "pending"},
            {"reason", reason}
        };

        // Update user's reservations in accounts.json
        bool userFound = false;
        for (auto& account : accounts) {
            if (account["email"].get<string>() == getEmail()) {
                account["reservations"].push_back(reservation);
                userFound = true;
                break;
            }
        }
        if (!userFound) {
            cout << "Error: User account not found!" << endl;
            return false;
        }

        // Save updated accounts
        ofstream accountsOut("../../data/accounts.json");
        accountsOut << setw(4) << accounts << endl;
        accountsOut.close();

        // Notify Lab Manager (TODO: implement notification system)
        cout << "NEED TO IMPLMENT ---- Notification sent to Lab Manager for approval." << endl;
        return false; // Indicate that approval is needed
    }
    
    // ELSE --- Create reservation entry
    json reservation = {
        {"assetID", assetID},
        {"assetName", (*targetAsset)["name"]},
        {"startDate", startDate},
        {"endDate", endDate},
        {"status", "approved"},
        {"reason", reason}
    };

    // Update user's reservations (reuse already loaded accounts)
    bool userFound = false;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            account["reservations"].push_back(reservation);
            userFound = true;
            break;
        }
    }

    if (!userFound) {
        cout << "Error: User account not found!" << endl;
        return false;
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();
    
    // Update asset status to reserved
    (*targetAsset)["operationalStatus"] = "reserved";
    
    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();
    
    // Append usage log entry for this reservation
    if (!appendUsageLog(getEmail(), assetID, startDate, endDate)) {
        cerr << "Warning: Failed to write usage log entry." << endl;
    }

    // If this asset is a consumable, decrement quantity on hand and check low-stock
    try {
        bool becameLow = false;
        if (a.decrementConsumable(assetID, 1, becameLow)) {
            if (becameLow && system != nullptr) {
                    system->update_usage_log("Low stock alert for asset ID " + to_string(assetID));
                }
        }
    } catch (...) {
        // non-fatal
    }

    cout << "Asset reserved successfully!" << endl;
    return true;
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
    if (system != nullptr) {
        try {
            return system->appendUsageEntry(entry);
        } catch (const std::exception& e) {
            cerr << "Error: system->appendUsageEntry threw: " << e.what() << endl;
            // fall through to direct write
        }
    }

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


//RESERVATIONS
//view own reservations
bool ResearchStudent::viewMyReservations() {
    cout << "--- My Reservations ---\n" << endl;
    
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool hasReservations = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            if (account["reservations"].empty()) {
                cout << "No active reservations found." << endl;
                return true;
            }

            int index = 1;
            for (const auto& res : account["reservations"]) {
                cout << "Reservation #" << index << endl;
                cout << "Asset ID: " << res["assetID"] << endl;
                cout << "Asset Name: " << res["assetName"] << endl;
                cout << "Start Date: " << res["startDate"] << endl;
                cout << "End Date: " << res["endDate"] << endl;
                cout << "Status: " << res["status"] << endl;
                if (res.contains("reason") && !res["reason"].get<string>().empty()) {
                    cout << "Reason: " << res["reason"] << endl;
                }
                cout << "-----------------------------------" << endl;
                hasReservations = true;
                index++;
            }
            break;
        }
    }

    if (!hasReservations) {
        cout << "No active reservations found." << endl;
    }
    
    return true;
}

//cancel reservation
bool ResearchStudent::cancelReservation(int reservationID) {
    cout << "--- Cancel Reservation ---\n" << endl;
    
    // Load accounts
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    // Find user and list their reservations
    json* userAccount = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            userAccount = &account;
            break;
        }
    }

    if (!userAccount || (*userAccount)["reservations"].empty()) {
        cout << "You have no reservations to cancel." << endl;
        return false;
    }

    cout << "Your Reservations:\n" << endl;
    int index = 1;
    for (const auto& res : (*userAccount)["reservations"]) {
        cout << index << ". Asset ID: " << res["assetID"] << " | Name: " << res["assetName"] 
             << " | Status: " << res["status"] << endl;
        index++;
    }

    cout << "\nEnter the number of the reservation to cancel: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (*userAccount)["reservations"].size()) {
        cout << "Invalid selection." << endl;
        return false;
    }

    // Get the asset ID before removing
    int assetID = (*userAccount)["reservations"][choice - 1]["assetID"].get<int>();

    // Remove reservation
    auto& reservations = (*userAccount)["reservations"];
    reservations.erase(reservations.begin() + (choice - 1));

    // Update asset status back to available
    json assets;
    ifstream assetFile("../../data/assets.json");
    if (assetFile.is_open()) {
        assetFile >> assets;
        assetFile.close();

        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID) {
                asset["operationalStatus"] = "available";
                break;
            }
        }

        ofstream outAssetFile("../../data/assets.json");
        outAssetFile << setw(4) << assets << endl;
        outAssetFile.close();
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();
    
    cout << "Reservation cancelled successfully!" << endl;
    
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
bool ResearchStudent::updateUserProfile(const std::string& newName, const std::string& newEmail) {
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
            // Parse new name
            size_t spacePos = newName.find(' ');
            if (spacePos != string::npos) {
                account["firstName"] = newName.substr(0, spacePos);
                account["lastName"] = newName.substr(spacePos + 1);
            }
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