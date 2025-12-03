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
                                 int role,
                                 int studentID,
                                 SystemController* sys)
    : User(firstName, lastName, email, role),
      studentID(studentID),
      system(sys) {}

//Destructor
ResearchStudent::~ResearchStudent() {
    cout << "Destructor Called" << endl;
}

//starting point
int ResearchStudent::main() {
    cout << "\n=============================================" << endl;
    cout << "Welcome " << Fname << " " << Lname << "!" << endl;
    cout << "=============================================\n" << endl;

    user_actions();

    cout << "Logging out of Research Student" << endl;
}

//displays the dashboard and the user actions
void ResearchStudent::display_page() {
    cout << "\n=============================================" << endl;
    cout << "Research Student Dashboard" << endl;
    cout << "=============================================" << endl;
    cout << "Name: " << getFirstName() << " " << getLastName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "StudentID: " << studentID << endl;
    cout << "=============================================\n" << endl;

    cout << "Main Menu: \n" << endl;
    cout << "1. Asset Management" << endl;
    cout << "2. Reservations Management" << endl;
    cout << "3. Profile Settings" << endl;
    cout << "4. Logout" << endl;
    cout << "=============================================\n" << endl;
}

//main actions
void ResearchStudent::user_actions() {
    bool loggedIn = true;
    int choice;

    while(loggedIn) {
        display_page();
        do {
            cout << "Choose an Option: " << endl;
            cin >> choice;
            if(choice < 1 || choice > 4) {
                cout << "Incorrect input, please try again" << endl;
            }
        } while(choice < 1 || choice > 4);

        if(choice == 1) {
            assetManagementMenu();
        } else if(choice == 2) {
            reservationsMenu();
        } else if(choice == 3) {
            profileMenu();
        } else {
            loggedIn = false;
        }
    }
}

//menu to manage assets
void ResearchStudent::assetManagementMenu() {
    bool inMenu = true;
    int choice;

    while (inMenu) {
        cout << "\n========================================" << endl;
        cout << "Asset Management" << endl;
        cout << "========================================" << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Return Asset" << endl;
        cout << "3. View Available Assets" << endl;
        cout << "4. View My Assets" << endl;
        cout << "5. Search/Filter Assets" << endl;
        cout << "6. Submit Usage Feedback" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================\n" << endl;

        do {
            cout << "Choose an Option: " << endl;
            cin >> choice;
            if(choice < 1 || choice > 7) {
                cout << "Incorrect input, please try again" << endl;
            }
        } while(choice < 1 || choice > 7);

        if (choice == 1) {
            if (reserveAsset()) {
                cout << "Asset reserved successfully!" << endl;
            } else {
                cout << "Failed to reserve asset." << endl;
            }
        }
        else if (choice == 2) {
            if (return_asset()) {
                cout << "Asset returned successfully!" << endl;
            } else {
                cout << "Failed to return asset." << endl;
            }
        }
        else if (choice == 3) {
            viewAvailableAssets();
        }
        else if (choice == 4) {
            viewAssets();
        }
        else if (choice == 5) {
            string category, status;
            cin.ignore();
            cout << "Enter category (or leave blank): ";
            getline(cin, category);
            cout << "Enter status (or leave blank): ";
            getline(cin, status);
            searchAssets(category, status);
        }
        else if (choice == 6) {
            int equipmentID, rating;
            string comments;
            cout << "Enter Equipment ID: ";
            cin >> equipmentID;
            cin.ignore();
            cout << "Enter comments: ";
            getline(cin, comments);
            cout << "Enter rating (1-5): ";
            cin >> rating;
            submitUsageFeedback(equipmentID, comments, rating);
        }
        else {
            inMenu = false;
        }
    }
}

void ResearchStudent::reservationsMenu() {
    bool inMenu = true;
    int choice;

    while (inMenu) {
        cout << "\n========================================" << endl;
        cout << "Reservations Management" << endl;
        cout << "========================================" << endl;
        cout << "1. Make Reservation" << endl;
        cout << "2. View My Reservations" << endl;
        cout << "3. Cancel My Reservation" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "========================================\n" << endl;

        do {
            cout << "Choose an Option: ";
            cin >> choice;
            if(choice < 1 || choice > 4) {
                cout << "Incorrect input, please try again" << endl;
            }
        } while(choice < 1 || choice > 4);

        if (choice == 1) {
            int reservationID;
            cout << "\nEnter Reservation ID: ";
            cin >> reservationID;
            if (makeReservation(reservationID)) {
                cout << "Reservation made successfully!" << endl;
            } else {
                cout << "Failed to make reservation." << endl;
            }
        }
        else if (choice == 2) {
            viewMyReservations();
        }
        else if (choice == 3) {
            int reservationID;
            cout << "\nEnter Reservation ID to cancel: ";
            cin >> reservationID;
            if (cancelReservation(reservationID)) {
                cout << "Reservation cancelled successfully!" << endl;
            } else {
                cout << "Failed to cancel reservation." << endl;
            }
        }
        else {
            inMenu = false;
        }
    }
}

void ResearchStudent::profileMenu() {
    bool inMenu = true;
    int choice;

    while (inMenu) {
        cout << "\n========================================" << endl;
        cout << "Profile Settings" << endl;
        cout << "========================================" << endl;
        cout << "1. Update Profile" << endl;
        cout << "2. Back to Main Menu" << endl;
        cout << "========================================\n" << endl;

        do {
            cout << "Choose an Option: ";
            cin >> choice;
            if(choice < 1 || choice > 2) {
                cout << "Incorrect input, please try again" << endl;
            }
        } while(choice < 1 || choice > 2);

        if (choice == 1) {
            string newName, newEmail;
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, newName);
            cout << "Enter new email: ";
            getline(cin, newEmail);
            updateUserProfile(newName, newEmail);
        }
        else {
            inMenu = false;
        }
    }
}

//ASSETS
//reserve an asset, returns a bool
bool ResearchStudent::reserveAsset() {
    string startDate, endDate;
    int assetID;
    cout << "--- Reserve Asset ---\n" << endl;
    cout << "Please enter the AssetID you would like to reserve: " << endl;
    cin >> assetID;
    cin.ignore();

    //get dates for reservation
    cout << "Enter Start Date (MM-DD-YYYY): ";
    getline(cin, startDate);
    cout << "Enter End Date (MM-DD-YYYY): ";
    getline(cin, endDate);

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

    // PLACEHOLDER: Call Reservations class to create reservation
    // TODO: Implement once Reservations class structure is finalized
    // Reservations::createReservation(assetID, studentID, startDate, endDate);
    cout << "\n[PLACEHOLDER] Creating reservation through Reservations class..." << endl;
    cout << "Asset ID: " << assetID << " | Student: " << studentID << endl;
    cout << "Dates: " << startDate << " to " << endDate << endl;

    //update to reserved
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

    // PLACEHOLDER: Call Reservations class to show user's active reservations
    // TODO: Implement once Reservations class structure is finalized
    // Reservations::viewUserReservations(studentID);
    cout << "[PLACEHOLDER] Displaying reservations..." << endl;

    // Ask which asset to return
    int assetID;
    cout << "Enter Asset ID to return: ";
    cin >> assetID;

    // PLACEHOLDER: Call Reservations class to mark reservation as returned
    // TODO: Implement once Reservations class structure is finalized
    // Reservations::returnReservation(assetID, studentID);
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
std::vector<Assets*> ResearchStudent::viewAssets() {
    cout << "--- My Assets ---\n" << endl;
    std::vector<Assets*> myAssets;
    //PLACEHOLDER: implement through Reservations/Assets class
    cout << "[PLACEHOLDER] Fetching student assets..." << endl;
    return myAssets;
}

//search and filter assets
std::vector<Assets*> ResearchStudent::searchAssets(const std::string& category, const std::string& status) {
    cout << "--- Search/Filter Assets ---\n" << endl;
    if (!category.empty()) {
        cout << "Category Filter: " << category << endl;
    }
    if (!status.empty()) {
        cout << "Status Filter: " << status << endl;
    }

    std::vector<Assets*> filteredAssets;
    //PLACEHOLDER: implement
    cout << "Displaying filtered results..." << endl;

    return filteredAssets;
}

//view all available
std::vector<Assets*> ResearchStudent::viewAvailableAssets() {
    cout << "--- Available Assets ---\n" << endl;
    std::vector<Assets*> availableAssets;
    //PLACEHOLDER: implement
    cout << "Displaying all available assets..." << endl;

    return availableAssets;
}

//RESERVATIONS
//view own reservations
std::vector<Reservations*> ResearchStudent::viewMyReservations() {
    cout << "--- My Reservations ---\n" << endl;
    std::vector<Reservations*> myReservations;
    //PLACEHOLDER: implement
    cout << "[PLACEHOLDER] Fetching reservations through Reservations class..." << endl;
    cout << "Student ID: " << studentID << endl;
    return myReservations;
}

//cancel reservation
bool ResearchStudent::cancelReservation(int reservationID) {
    cout << "--- Cancel Reservation ---\n" << endl;
    cout << "Reservation ID: " << reservationID << endl;
    //PLACEHOLDER: implement through Reservations class
    cout << "[PLACEHOLDER] Cancelling through Reservations class..." << endl;
    return true;
}

//make reservation for self only
bool ResearchStudent::makeReservation(int reservationID) {
    cout << "--- Make Reservation ---\n" << endl;
    cout << "Reservation ID: " << reservationID << endl;
    //PLACEHOLDER: implement through Reservations class
    cout << "[PLACEHOLDER] Creating reservation for student..." << endl;
    return true;
}

//STUDENT SPECIFIC FEATURES
//record feedback on an asset after use
bool ResearchStudent::submitUsageFeedback(int equipmentID, const std::string& comments, int rating) {
    cout << "--- Submit Usage Feedback ---\n" << endl;
    cout << "Equipment ID: " << equipmentID << endl;
    cout << "Comments: " << comments << endl;
    cout << "Rating: " << rating << "/5" << endl;
    
    //PLACEHOLDER: Save to feedback.json or similar
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
    cout << "[PLACEHOLDER] Updating profile in system..." << endl;
    cout << "Profile updated successfully!" << endl;
    
    return true;
}