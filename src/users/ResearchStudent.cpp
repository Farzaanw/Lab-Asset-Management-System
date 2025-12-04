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
#include "ResearchStudent.h"
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"

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
            reserveAsset();
        }
        else if (choice == "2") {
            return_asset();
        }
        else if (choice == "3") {
            viewAvailableAssets();
        }
        else if (choice == "4") {
            viewAssets();
        }
        else if (choice == "5") {
            string category, status;
            cout << "Enter category (or leave blank): ";
            getline(cin, category);
            cout << "Enter status (or leave blank): ";
            getline(cin, status);
            searchAssets(category, status);
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
    
    // First, show available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

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
    
    // Get asset type/category to determine if approval needed
    string assetCategory = (*targetAsset)["category"].get<string>();
    
    // For certain asset types, may need reason/justification
    if (assetCategory == "high-value" || assetCategory == "restricted") {
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
        cout << "This asset requires approval. You will be notified once reviewed." << endl;
    }
    
    // Validate dates
    if (startDate.empty() || endDate.empty()) {
        cout << "Error: Invalid date format!" << endl;
        return false;
    }
    
    // Create reservation entry
    json reservation = {
        {"assetID", assetID},
        {"assetName", (*targetAsset)["name"]},
        {"startDate", startDate},
        {"endDate", endDate},
        {"status", "pending"},
        {"reason", reason}
    };

    // Update user's reservations in accounts.json
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

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
    
    cout << "Asset reserved successfully!" << endl;
    return true;
}

//Return an asset
bool ResearchStudent::return_asset() {
    cout << "--- Return Asset ---\n" << endl;

    // First, show user's checked-out assets
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    json* userAccount = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            userAccount = &account;
            break;
        }
    }

    if (!userAccount || (*userAccount)["reservations"].empty()) {
        cout << "You have no assets to return." << endl;
        return false;
    }

    cout << "Your Reserved Assets:\n" << endl;
    for (const auto& res : (*userAccount)["reservations"]) {
        if (res["status"] == "confirmed" || res["status"] == "approved") {
            cout << "Asset ID: " << res["assetID"] << " | Name: " << res["assetName"] 
                 << " | Start: " << res["startDate"] << " | End: " << res["endDate"] << endl;
        }
    }

    cout << "\n-----------------------------------\n" << endl;

    // Ask which asset to return
    int assetID;
    cout << "Enter Asset ID to return: ";
    cin >> assetID;
    cin.ignore();

    // Update asset status to available
    json assets;
    ifstream assetFile("../../data/assets.json");
    if (!assetFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    assetFile >> assets;
    assetFile.close();

    bool found = false;
    for (auto& asset : assets) {
        if (asset["id"].get<int>() == assetID) {
            asset["operationalStatus"] = "available";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Asset ID not found" << endl;
        return false;
    }

    // Remove reservation from user's account
    auto& reservations = (*userAccount)["reservations"];
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)["assetID"].get<int>() == assetID) {
            reservations.erase(it);
            break;
        }
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();

    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();

    cout << "Asset returned successfully!" << endl;
    return true;
}

//view own assets
bool ResearchStudent::viewAssets() {
    cout << "--- My Assets ---\n" << endl;

    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool hasAssets = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == getEmail()) {
            if (account["reservations"].empty()) {
                cout << "You have no assets checked out." << endl;
                return true;
            }

            for (const auto& res : account["reservations"]) {
                if (res["status"] == "confirmed" || res["status"] == "approved") {
                    cout << "Asset ID: " << res["assetID"] << endl;
                    cout << "Name: " << res["assetName"] << endl;
                    cout << "Start Date: " << res["startDate"] << endl;
                    cout << "End Date: " << res["endDate"] << endl;
                    cout << "Status: " << res["status"] << endl;
                    cout << "-----------------------------------" << endl;
                    hasAssets = true;
                }
            }
            break;
        }
    }

    if (!hasAssets) {
        cout << "You have no assets checked out." << endl;
    }

    return true;
}

//search and filter assets
bool ResearchStudent::searchAssets(const std::string& category, const std::string& status) {
    cout << "--- Search/Filter Assets ---\n" << endl;
    
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();
    
    bool found = false;
    for (const auto& asset : assets) {
        bool matchCategory = category.empty() || asset["category"] == category;
        bool matchStatus = status.empty() || asset["operationalStatus"] == status;
        
        if (matchCategory && matchStatus) {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Status: " << asset["operationalStatus"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "-----------------------------------" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No assets found matching criteria." << endl;
    }
    
    return true;
}

//view all available
bool ResearchStudent::viewAvailableAssets() {
    cout << "--- Available Assets ---\n" << endl;
    
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }

    try {
        inFile >> assets;
    } catch (const std::exception& e) {
        cerr << "Error reading JSON: " << e.what() << endl;
        inFile.close();
        return false;
    }
    inFile.close();

    if (assets.empty()) {
        cout << "No assets found." << endl;
        return true;
    }

    cout << "Listing all available assets:\n" << endl;
    bool hasAvailable = false;
    for (const auto& asset : assets) {
        if (asset["operationalStatus"] == "available") {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Condition: " << asset["condition"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "Description: " << asset["description"] << endl;
            cout << "-----------------------------------" << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No assets currently available." << endl;
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