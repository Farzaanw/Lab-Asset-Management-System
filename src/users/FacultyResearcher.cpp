//CS-1: FacultyResearcher.cpp
//Author: Jack Dunlap
//Supports: UR-101, UR-102, UR-103, UR-104
//Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*]

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <ctime>
#include "FacultyResearcher.h"
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

//Constructor
FacultyResearcher::FacultyResearcher(const std::string& firstName,
                                     const std::string& lastName,
                                     const std::string& email,
                                     SystemController* sys)
    : User(firstName, lastName, email, sys),
      system(sys) {}

//Destructor
FacultyResearcher::~FacultyResearcher() {}

//Override getRole
std::string FacultyResearcher::getRole() const {
    return "faculty researcher";
}

//Main menu
void FacultyResearcher::main() {
    cout << "\n=============================================" << endl;
    cout << "Welcome " << getFirstName() << " " << getLastName() << "!" << endl;
    cout << "=============================================\n" << endl;

    while(true) {
        cout << endl << "---Faculty Researcher Main Menu---" << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Reserve Multiple Assets" << endl;
        cout << "3. Return Asset" << endl;
        cout << "4. View Available Assets" << endl;
        cout << "5. Search/Filter Assets" << endl;
        cout << "6. View Student Assets" << endl;
        cout << "7. View My Reservations" << endl;
        cout << "8. Cancel Reservation" << endl;
        cout << "9. View Available License Seats" << endl;
        cout << "10. Request Software License" << endl;
        cout << "11. Request Group Software License" << endl;
        cout << "12. View My Licenses" << endl;
        cout << "13. View Group Licenses" << endl;
        cout << "14. View Lab Group" << endl;
        cout << "15. View Group Reservations" << endl;
        cout << "16. Cancel Group Reservation" << endl;
        cout << "17. Generate Usage Report" << endl;
        cout << "18. Logout" << endl;
        cout << "Please enter your choice: ";
        
        string choice;
        getline(cin, choice);
        cout << endl;

        if (choice == "1") {
            reserveAsset();
        }
        else if (choice == "2") {
            reserveMultipleAssets();
        }
        else if (choice == "3") {
            return_asset();
        }
        else if (choice == "4") {
            viewAvailableAssets();
        }
        else if (choice == "5") {
            searchAssets("", "");
        }
        else if (choice == "6") {
            viewStudentAssets("");
        }
        else if (choice == "7") {
            viewMyReservations();
        }
        else if (choice == "8") {
            cancelReservation(0);
        }
        else if (choice == "9") {
            viewAvailableLicenseSeats();
        }
        else if (choice == "10") {
            requestSoftwareLicense(0, "", "");
        }
        else if (choice == "11") {
            requestSoftwareLicenseGroup(0, 0, "", "");
        }
        else if (choice == "12") {
            viewLicenses();
        }
        else if (choice == "13") {
            viewGroupLicenses(0);
        }
        else if (choice == "14") {
            viewGroup(0);
        }
        else if (choice == "15") {
            viewGroupReservations(0);
        }
        else if (choice == "16") {
            cancelGroupReservations(0, 0);
        }
        else if (choice == "17") {
            generateUsageReport(0);
        }
        else if (choice == "18") {
            cout << "Logging out of Faculty Researcher." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

//NAVIGATION/DISPLAY
void FacultyResearcher::display_page() {
    cout << "--- Faculty Researcher Dashboard ---" << endl;
    cout << "Name: " << getFirstName() << " " << getLastName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Role: " << getRole() << endl;
}

void FacultyResearcher::user_actions() {
    cout << "Faculty member can perform various actions." << endl;
}

//ASSET MANAGEMENT
//Reserve a single asset
bool FacultyResearcher::reserveAsset() {
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
        {"status", "confirmed"},
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

//Reserve multiple assets at once
bool FacultyResearcher::reserveMultipleAssets() {
    cout << "--- Reserve Multiple Assets ---\n" << endl;
    
    // Show available assets
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
    
    cout << "How many assets would you like to reserve? ";
    int numAssets;
    cin >> numAssets;
    cin.ignore();
    
    if (numAssets <= 0) {
        cout << "Invalid number of assets." << endl;
        return false;
    }

    string startDate, endDate;
    cout << "Enter Start Date (YYYY-MM-DD) for all assets: ";
    getline(cin, startDate);
    cout << "Enter End Date (YYYY-MM-DD) for all assets: ";
    getline(cin, endDate);

    // Load accounts
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    vector<int> assetIDs;
    for (int i = 0; i < numAssets; i++) {
        int assetID;
        cout << "Enter Asset ID #" << (i + 1) << ": ";
        cin >> assetID;
        cin.ignore();
        
        // Verify asset exists and is available
        bool found = false;
        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID && 
                asset["operationalStatus"] == "available") {
                assetIDs.push_back(assetID);
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Asset ID " << assetID << " not found or unavailable!" << endl;
            return false;
        }
    }

    // Reserve all assets
    for (int assetID : assetIDs) {
        // Find asset
        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID) {
                // Create reservation
                json reservation = {
                    {"assetID", assetID},
                    {"assetName", asset["name"]},
                    {"startDate", startDate},
                    {"endDate", endDate},
                    {"status", "confirmed"},
                    {"reason", ""}
                };

                // Add to user's reservations
                for (auto& account : accounts) {
                    if (account["email"].get<string>() == getEmail()) {
                        account["reservations"].push_back(reservation);
                        break;
                    }
                }

                // Update asset status
                asset["operationalStatus"] = "reserved";
                break;
            }
        }
    }

    // Save files
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();

    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();

    cout << "All " << numAssets << " assets reserved successfully!" << endl;
    return true;
}

//Return an asset
bool FacultyResearcher::return_asset() {
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

//View assets checked out by a specific student
bool FacultyResearcher::viewStudentAssets(const std::string& studentEmail) {
    cout << "--- View Student Assets ---\n" << endl;

    string email = studentEmail;
    if (email.empty()) {
        cout << "Enter student email: ";
        getline(cin, email);
    }

    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool studentFound = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == email && 
            account["role"] == "research student") {
            studentFound = true;
            
            if (account["reservations"].empty()) {
                cout << "Student " << account["firstName"] << " " << account["lastName"] 
                     << " has no assets checked out." << endl;
                return true;
            }

            cout << "Assets for " << account["firstName"] << " " << account["lastName"] << ":\n" << endl;
            for (const auto& res : account["reservations"]) {
                if (res["status"] == "confirmed" || res["status"] == "approved") {
                    cout << "Asset ID: " << res["assetID"] << endl;
                    cout << "Name: " << res["assetName"] << endl;
                    cout << "Start Date: " << res["startDate"] << endl;
                    cout << "End Date: " << res["endDate"] << endl;
                    cout << "Status: " << res["status"] << endl;
                    cout << "-----------------------------------" << endl;
                }
            }
            break;
        }
    }

    if (!studentFound) {
        cout << "Student with email " << email << " not found." << endl;
        return false;
    }

    return true;
}

//Search and filter assets
bool FacultyResearcher::searchAssets(const std::string& category, const std::string& status) {
    cout << "--- Search/Filter Assets ---\n" << endl;

    string cat = category;
    string stat = status;
    
    if (cat.empty()) {
        cout << "Enter category (or leave blank): ";
        getline(cin, cat);
    }
    if (stat.empty()) {
        cout << "Enter status (or leave blank): ";
        getline(cin, stat);
    }
    
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
        bool matchCategory = cat.empty() || asset["category"] == cat;
        bool matchStatus = stat.empty() || asset["operationalStatus"] == stat;
        
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

//View all available assets
bool FacultyResearcher::viewAvailableAssets() {
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

//SOFTWARE LICENSE MANAGEMENT
//View available software license seats
bool FacultyResearcher::viewAvailableLicenseSeats() {
    cout << "--- Available Software License Seats ---\n" << endl;
    cout << "TODO: Implement viewAvailableLicenseSeats" << endl;
    return true;
}

//Request software license for self
bool FacultyResearcher::requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate) {
    cout << "--- Request Software License ---\n" << endl;
    cout << "TODO: Implement requestSoftwareLicense" << endl;
    return true;
}

//Request software license for group
bool FacultyResearcher::requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate) {
    cout << "--- Request Group Software License ---\n" << endl;
    cout << "TODO: Implement requestSoftwareLicenseGroup" << endl;
    return true;
}

//View own licenses
bool FacultyResearcher::viewLicenses() {
    cout << "--- My Licenses ---\n" << endl;
    cout << "TODO: Implement viewLicenses" << endl;
    return true;
}

//View group licenses
bool FacultyResearcher::viewGroupLicenses(int labGroupID) {
    cout << "--- View Group Licenses ---\n" << endl;
    cout << "TODO: Implement viewGroupLicenses" << endl;
    return true;
}

//RESERVATION MANAGEMENT
//View all reservations made by this faculty member
bool FacultyResearcher::viewMyReservations() {
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

//Cancel own reservation
bool FacultyResearcher::cancelReservation(int reservationID) {
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

//LAB GROUP MANAGEMENT
//Display group information
void FacultyResearcher::viewGroup(int labGroupID) {
    cout << "--- View Lab Group ---\n" << endl;
    cout << "TODO: Implement viewGroup" << endl;
}

//View group reservations
bool FacultyResearcher::viewGroupReservations(int labGroupID) {
    cout << "--- View Group Reservations ---\n" << endl;
    cout << "TODO: Implement viewGroupReservations" << endl;
    return true;
}

//Cancel group reservation
bool FacultyResearcher::cancelGroupReservations(int labGroupID, int reservationID) {
    cout << "--- Cancel Group Reservation ---\n" << endl;
    cout << "TODO: Implement cancelGroupReservations" << endl;
    return true;
}

//DOCUMENTS/REPORTS
//Generate usage report for resource usage by a group
bool FacultyResearcher::generateUsageReport(int labGroupID) {
    cout << "--- Generate Usage Report ---\n" << endl;
    cout << "TODO: Implement generateUsageReport" << endl;
    return true;
}