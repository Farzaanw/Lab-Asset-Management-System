//CS-2: ResearchStudent.cpp
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include "ResearchStudent.h"

using namespace std;
using json = nlohmann::json;

//Constructor
ResearchStudent::ResearchStudent(const std::string& firstName,
                                 const std::string& lastName,
                                 const std::string& email,
                                 SystemController* sys)
    : User(firstName, lastName, email),
      system(sys) {}

//Destructor
ResearchStudent::~ResearchStudent() {}

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
            int reservationID;
            cout << "Enter Reservation ID to cancel: ";
            cin >> reservationID;
            cin.ignore();
            cancelReservation(reservationID);
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

//HELPER FUNCTION - Make Reservation (collects all reservation info)
bool ResearchStudent::makeReservation(int assetID, const std::string& assetType) {
    cout << "--- Creating Reservation ---\n" << endl;
    
    string startDate, endDate, reason;
    
    // Get reservation dates
    cout << "Enter Start Date (MM-DD-YYYY): ";
    getline(cin, startDate);
    cout << "Enter End Date (MM-DD-YYYY): ";
    getline(cin, endDate);
    
    // For certain asset types, may need reason/justification
    if (assetType == "high-value" || assetType == "restricted") {
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
    }
    
    // Validate dates (basic check)
    if (startDate.empty() || endDate.empty()) {
        cout << "Error: Invalid date format!" << endl;
        return false;
    }
    
    // PLACEHOLDER: Create reservation through Reservations class
    // TODO: This will call Reservations::createReservation(assetID, studentID, startDate, endDate, reason)
    cout << "[PLACEHOLDER] Creating reservation through Reservations class..." << endl;
    cout << "Asset ID: " << assetID << endl;
    cout << "Asset Type: " << assetType << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Start Date: " << startDate << endl;
    cout << "End Date: " << endDate << endl;
    if (!reason.empty()) {
        cout << "Reason: " << reason << endl;
    }
    
    // PLACEHOLDER: Create usage log entry
    // TODO: UsageLog::createEntry(studentID, assetID, startDate, endDate)
    cout << "[PLACEHOLDER] Creating usage log entry..." << endl;
    
    // PLACEHOLDER: For restricted assets, create approval request
    // TODO: If asset requires approval, create ApprovalRequest
    if (assetType == "restricted" || assetType == "high-value") {
        cout << "[PLACEHOLDER] This asset requires approval. Creating approval request..." << endl;
        cout << "You will be notified once your request is reviewed." << endl;
    }
    
    cout << "\nReservation request submitted successfully!" << endl;
    return true;
}

//ASSETS
//reserve an asset, returns a bool
bool ResearchStudent::reserveAsset() {
    int assetID;
    cout << "--- Reserve Asset ---\n" << endl;
    cout << "Please enter the AssetID you would like to reserve: ";
    cin >> assetID;
    cin.ignore();

    // Load and validate asset
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

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

    // Get asset type/category to determine if approval needed
    string assetCategory = (*targetAsset)["category"].get<string>();
    
    // Call makeReservation with asset info
    bool reservationSuccess = makeReservation(assetID, assetCategory);
    
    if (reservationSuccess) {
        // Update asset status to reserved
        (*targetAsset)["operationalStatus"] = "reserved";
        
        ofstream outAssetFile("../../data/assets.json");
        outAssetFile << setw(4) << assets << endl;
        outAssetFile.close();
        
        cout << "Asset reserved successfully!" << endl;
        return true;
    }
    
    return false;
}

//Return an asset
bool ResearchStudent::return_asset() {
    cout << "--- Return Asset ---\n" << endl;

    // PLACEHOLDER: Call Reservations class to show user's active reservations
    // TODO: Implement once Reservations class structure is finalized
    // Reservations::viewUserReservations(studentID);
    cout << "[PLACEHOLDER] Displaying reservations..." << endl;

    // Ask which asset to return
    int assetID;
    cout << "Enter Asset ID to return: ";
    cin >> assetID;
    cin.ignore();

    // PLACEHOLDER: Mark reservation as returned
    cout << "\n[PLACEHOLDER] Marking reservation as returned through Reservations class..." << endl;
    cout << "Asset ID: " << assetID << " | Student: " << studentID << endl;

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

    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();

    cout << "Asset returned successfully!" << endl;
    return true;
}

//view own assets
bool ResearchStudent::viewAssets() {
    cout << "--- My Assets ---\n" << endl;

    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

    bool hasAssets = false;
    for (const auto& asset : assets) {
        // Check if reserved by this student (you'd need to add reservedBy field)
        if (asset["operationalStatus"] == "reserved") {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "-----------------------------------" << endl;
            hasAssets = true;
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
    if (!category.empty()) {
        cout << "Category Filter: " << category << endl;
    }
    if (!status.empty()) {
        cout << "Status Filter: " << status << endl;
    }

    //PLACEHOLDER: implement
    cout << "Displaying filtered results..." << endl;

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
    for (const auto& asset : assets) {
        if (asset["operationalStatus"] == "available") {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Condition: " << asset["condition"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "Description: " << asset["description"] << endl;
            cout << "-----------------------------------" << endl;
        }
    }

    return true;
}

//RESERVATIONS
//view own reservations
bool ResearchStudent::viewMyReservations() {
    cout << "--- My Reservations ---\n" << endl;
    
    //PLACEHOLDER: implement
    cout << "[PLACEHOLDER] Fetching reservations through Reservations class..." << endl;
    cout << "Student ID: " << studentID << endl;
    
    return true;
}

//cancel reservation
bool ResearchStudent::cancelReservation(int reservationID) {
    cout << "--- Cancel Reservation ---\n" << endl;
    cout << "Reservation ID: " << reservationID << endl;
    
    // PLACEHOLDER: Check if reservation can be cancelled (before start time)
    // TODO: Reservations::checkIfCancellable(reservationID, studentID)
    cout << "[PLACEHOLDER] Verifying reservation belongs to student and can be cancelled..." << endl;
    
    // PLACEHOLDER: Cancel through Reservations class
    // TODO: Reservations::cancelReservation(reservationID, studentID)
    cout << "[PLACEHOLDER] Cancelling through Reservations class..." << endl;
    
    // PLACEHOLDER: Update asset status back to available if needed
    cout << "[PLACEHOLDER] Updating asset status to available..." << endl;
    
    cout << "Reservation cancelled successfully!" << endl;
    
    return true;
}

//STUDENT SPECIFIC FEATURES
//record feedback on an asset after use
bool ResearchStudent::submitUsageFeedback(int equipmentID, const std::string& comments, int rating) {
    cout << "--- Submit Usage Feedback ---\n" << endl;
    cout << "Equipment ID: " << equipmentID << endl;
    cout << "Comments: " << comments << endl;
    cout << "Rating: " << rating << "/5" << endl;
    
    // Validate rating
    if (rating < 1 || rating > 5) {
        cout << "Error: Rating must be between 1 and 5!" << endl;
        return false;
    }
    
    //PLACEHOLDER: Save to feedback.json or similar
    // TODO: Create feedback entry with timestamp
    cout << "[PLACEHOLDER] Saving feedback to system..." << endl;
    cout << "Feedback submitted successfully!" << endl;
    
    return true;
}

//update profile information
bool ResearchStudent::updateUserProfile(const std::string& newName, const std::string& newEmail) {
    cout << "--- Update User Profile ---\n" << endl;
    cout << "New Name: " << newName << endl;
    cout << "New Email: " << newEmail << endl;
    
    //PLACEHOLDER: Update in user_logins.json
    // TODO: Update user information in system
    cout << "[PLACEHOLDER] Updating profile in system..." << endl;
    cout << "Profile updated successfully!" << endl;
    
    return true;
}