#include <iostream>
#include "FacultyResearcher.h"
using namespace std;

//starting point
int FacultyResearcher::main() {
  cout << "\n=============================================" << endl;
  cout << "Welcome " << Fname << " " << Lname << "!" << endl;
  cout << "=============================================\n" << endl;
  
  user_actions();

  cout << "Logging out of Faculty Researcher" << endl;
}

//Destructor
FacultyResearcher::~FacultyResearcher() {
  cout << "Destructor Called" << endl;
}

//Constructor
FacultyResearcher::FacultyResearcher(const std::string& firstName,
                                     const std::string& lastName,
                                     const std::string& email,
                                     int role,
                                     int facultyID,
                                     SystemController* sys) 
      : User(firstName, lastName, email, role), 
      FacultyID(facultyID),
      system(sys) {}

//BASIC
//displays the dashboard and the user actions
void FacultyResearcher::display_page() {
  cout << "\n=============================================" << endl;
  cout << "Faculty Researcher Dashboard" << endl;
  cout << "=============================================" << endl;
  cout << "Name: " << getFirstName() << " " << getLastName() << endl;
  cout << "Email: " << getEmail() << endl;
  cout << "FacultyID: " << FacultyID << endl;
  cout << "=============================================\n" << endl;

  cout << "Main Menu: \n" << endl;
  cout << "1. Asset Management" << endl;
  cout << "2. Lab Group Management" << endl;
  cout << "3. Software Licenses" << endl;
  cout << "4. Reservations Management" << endl;
  cout << "5. Reports and Documents" << endl;
  cout << "6. Logout" << endl;
  cout << "=============================================\n" << endl;
}

//main actions
void FacultyResearcher::user_actions() {
  bool loggedIn = true;
  int choice;

  while(loggedIn) {
    display_page();
    do {
      cout << "Choose an Option: " << endl;
      cin >> choice;
      if(choice < 1 || choice > 8) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(choice < 1 || choice > 8);

    if(choice == 1) {
      assetManagementMenu();
    } else if(choice == 2) {
      labGroupManagementMenu();
    } else if(choice == 3) {
      softwareLicenseMenu();
    } else if(choice == 4) {
      reservationsMenu();
    } else if(choice == 5) {
      reportsAndDocumentsMenu();
    } else {
      loggedIn = false;
    }
  }
}

//menu to manage assets
void FacultyResearcher::assetManagementMenu() {
  bool inMenu = true;
  int choice;
  int assetID;
  bool permissions;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "Asset Management" << endl;
    cout << "========================================" << endl;
    cout << "1. Reserve Single Asset" << endl;
    cout << "2. Reserve Multiple Assets" << endl;
    cout << "3. Return Asset" << endl;
    cout << "4. Search/Filter Assets" << endl;
    cout << "5. View Available Assets" << endl;
    cout << "6. View My Assets" << endl;
    cout << "7. View Student Assets" << endl;
    cout << "8. Back to Main Menu" << endl;
    cout << "========================================\n" << endl;
      
    do {
      cout << "Choose an Option: " << endl;
      cin >> choice;
      if(choice < 1 || choice > 6) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(choice < 1 || choice > 6);
      
    if (choice == 1) {
        cout << "Enter Asset ID to reserve: ";
        cin >> assetID;
        if (reserveAsset()) {
            cout << "Asset reserved successfully!" << endl;
        } else {
            cout << "Failed to reserve asset." << endl;
        }
    }
    else if (choice == 2) {
        int reservationID;
        cout << "\nEnter Reservation ID to return: ";
        cin >> reservationID;
        if (return_asset()) {
            cout << "Asset returned successfully!" << endl;
        } else {
            cout << "Failed to return asset." << endl;
        }
    }
    else if (choice == 3) {
        //view available assets
        cout << "\n--- Available Assets ---" << endl;
        // Call to Assets class method to display available assets
    }
    else if (choice == 4) {
        //view own assets
        viewAssets();
    }
    else if (choice == 5) {
        int studentID;
        cout << "\nEnter Student ID: ";
        cin >> studentID;
        viewStudentAssets(studentID);
    }
    else { // Back
        inMenu = false;
    }
  }
}

void FacultyResearcher::labGroupManagementMenu() {
  bool inMenu = true;
  int choice;
  int studentID, labGroupID, reservationID;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "Lab Group Management" << endl;
    cout << "========================================" << endl;
    cout << "1. View Group Information" << endl;
    cout << "2. View Group Assets" << endl;
    cout << "3. View Group Reservations" << endl;
    cout << "4. Cancel Group Reservation" << endl;
    cout << "5. View Student Assets" << endl;
    cout << "6. View Group Usage Log" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "========================================\n" << endl;
      
    do {
      cout << "Choose an Option: ";
      cin >> choice;
      if(choice < 1 || choice > 7) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(choice < 1 || choice > 7);
      
    if (choice == 1) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroup(labGroupID);
    }
    else if (choice == 2) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroupAssets(labGroupID);
    }
    else if (choice == 3) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroupReservations(labGroupID);
    }
    else if (choice == 4) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        cout << "Enter Reservation ID to cancel: ";
        cin >> reservationID;
        if (cancelGroupReservations(labGroupID, reservationID)) {
            cout << "Reservation cancelled successfully!" << endl;
        } else {
            cout << "Failed to cancel reservation." << endl;
        }
    }
    else if (choice == 5) {
        cout << "\nEnter Student ID: ";
        cin >> studentID;
        viewStudentAssets(studentID);
    }
    else if (choice == 6) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroupUsage(labGroupID);
    }
    else { // Back
        inMenu = false;
    }
    
  }
}

void FacultyResearcher::softwareLicenseMenu() {
  bool inMenu = true;
  int choice;
  int licenseID, labGroupID;
  string startDate, endDate;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "Software License Menu" << endl;
    cout << "========================================" << endl;
    cout << "1. View Available License Seats" << endl;
    cout << "2. Request License (Self)" << endl;
    cout << "3. Request License (Group)" << endl;
    cout << "4. View My Licenses" << endl;
    cout << "5. View Group Licenses" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "========================================\n" << endl;
      
    do {
      cout << "Choose an Option: ";
      cin >> choice;
      if(choice < 1 || choice > 6) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(choice < 1 || choice > 6);
      
    if (choice == 1) {
        viewAvailableLicenseSeats();
    }
    else if (choice == 2) {
        cout << "\nEnter License ID: ";
        cin >> licenseID;
        cin.ignore();
        cout << "Enter Start Date (MM-DD-YYYY): ";
        getline(cin, startDate);
        cout << "Enter End Date (MM-DD-YYYY): ";
        getline(cin, endDate);
        if (requestSoftwareLicense(licenseID, startDate, endDate)) {
            cout << "License requested successfully!" << endl;
        } else {
            cout << "Failed to request license." << endl;
        }
    }
    else if (choice == 3) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        cout << "Enter License ID: ";
        cin >> licenseID;
        cin.ignore();
        cout << "Enter Start Date (MM-DD-YYYY): ";
        getline(cin, startDate);
        cout << "Enter End Date (MM-DD-YYYY): ";
        getline(cin, endDate);
        if (requestSoftwareLicenseGroup(licenseID, labGroupID, startDate, endDate)) {
            cout << "Group license requested successfully!" << endl;
        } else {
            cout << "Failed to request group license." << endl;
        }
    }
    else if (choice == 4) {
        viewLicenses();
    }
    else if (choice == 5) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroupLicenses(labGroupID);
    }
    else {
        inMenu = false;
    }

  }
}

void FacultyResearcher::reservationsMenu() {
  bool inMenu = true;
  int choice;
  int reservationID, labGroupID;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "Reservations Management" << endl;
    cout << "========================================" << endl;
    cout << "1. Make Reservation (Self)" << endl;
    cout << "2. Make Reservation (Group)" << endl;
    cout << "3. View My Reservations" << endl;
    cout << "4. View Group Reservations" << endl;
    cout << "5. Cancel My Reservation" << endl;
    cout << "6. Cancel Group Reservation" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "========================================\n" << endl;
      
    do {
      cout << "Choose an Option: ";
      cin >> choice;
      if(choice < 1 || choice > 7) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(choice < 1 || choice > 7);
      
    if (choice == 1) {
        cout << "\nEnter Reservation ID: ";
        cin >> reservationID;
        if (makeReservation(reservationID, 0)) { // 0 or NULL for self
            cout << "Reservation made successfully!" << endl;
        } else {
            cout << "Failed to make reservation." << endl;
        }
    }
    else if (choice == 2) {
        cout << "\nEnter Reservation ID: ";
        cin >> reservationID;
        cout << "Enter Lab Group ID: ";
        cin >> labGroupID;
        if (makeReservation(reservationID, labGroupID)) {
            cout << "Group reservation made successfully!" << endl;
        } else {
            cout << "Failed to make group reservation." << endl;
        }
    }
    else if (choice == 3) {
        viewMyReservations();
    }
    else if (choice == 4) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        viewGroupReservations(labGroupID);
    }
    else if (choice == 5) {
        cout << "\nEnter Reservation ID to cancel: ";
        cin >> reservationID;
        if (cancelReservation(reservationID)) {
            cout << "Reservation cancelled successfully!" << endl;
        } else {
            cout << "Failed to cancel reservation." << endl;
        }
    }
    else if (choice == 6) {
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        cout << "Enter Reservation ID to cancel: ";
        cin >> reservationID;
        if (cancelGroupReservations(labGroupID, reservationID)) {
            cout << "Group reservation cancelled successfully!" << endl;
        } else {
            cout << "Failed to cancel group reservation." << endl;
        }
    }
    else {
        inMenu = false;
    }
  }
}

void FacultyResearcher::reportsAndDocumentsMenu() {
  bool inMenu = true;
  int choice;
  int labGroupID;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "Reports and Documents" << endl;
    cout << "========================================" << endl;
    cout << "1. Generate Usage Report" << endl;
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
        cout << "\nEnter Lab Group ID: ";
        cin >> labGroupID;
        cout << "Generating usage report for Lab Group " << labGroupID << "..." << endl;
        //retrieve asset usage and group data, usage log(recent actions?)
        //generateUsageReport(assetUsed, dataRecords);
        cout << "Report generated successfully!" << endl;
    }
    else {
        inMenu = false;
    }
  }
}

//ASSETS
//reserve an asset, returns a bool
bool FacultyResearcher::reserveAsset() {
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
  // Reservations::createReservation(assetID, FacultyID, startDate, endDate);
  cout << "\n[PLACEHOLDER] Creating reservation through Reservations class..." << endl;
  cout << "Asset ID: " << assetID << " | User: " << FacultyID << endl;
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
bool FacultyResearcher::return_asset() {
  cout << "--- Return Asset ---\n" << endl;
  
  // PLACEHOLDER: Call Reservations class to show user's active reservations
  // TODO: Implement once Reservations class structure is finalized
  // Reservations::viewUserReservations(FacultyID);
  cout << "[PLACEHOLDER] Displaying reservations..." << endl;
  
  // Ask which asset to return
  int assetID;
  cout << "Enter Asset ID to return: ";
  cin >> assetID;
  
  // PLACEHOLDER: Call Reservations class to mark reservation as returned
  // TODO: Implement once Reservations class structure is finalized
  // Reservations::returnReservation(assetID, FacultyID);
  cout << "\n[PLACEHOLDER] Marking reservation as returned through Reservations class..." << endl;
  cout << "Asset ID: " << assetID << " | User: " << FacultyID << endl;
  
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
std::vector<Assets*> FacultyResearcher::viewAssets() {
  cout << "--- My Assets ---\n" << endl;
  std::vector<Assets*> myAssets;
  //implement
  return myAssets;
}

// view a student's list of assets they have checked out
std::vector<Assets*> FacultyResearcher::viewStudentAssets(int studentID) {
  cout << "--- Student Assets ---\n" << endl;
  cout << "Student ID: " << studentID << endl;
  std::vector<Assets*> studentAssets;
  //implement
  return studentAssets;
}

//GROUPS
//view the groups list of assets they have checked out
vector<Assets*> FacultyResearcher::viewGroupAssets(int labGroupID) {
  cout << "--- Group Assets ---\n" << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  vector<Assets*> groupAssets;
  //implement
  return groupAssets;
}

//view group usage records(their usage log)
void FacultyResearcher::viewGroupUsage(int labGroupID) {
  cout << "--- Group Usage Log ---\n" << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  //display usage log
}

//display a record of the group the faculty is in charge of and their work they have done
void FacultyResearcher::viewGroup(int LabGroupID) {
  cout << "--- Group Information ---\n" << endl;
  cout << "Lab Group ID: " << LabGroupID << endl;
  //view whos in a group and their information
}

//SOFTWARE LICENSE
//view available software licenses
std::vector<Documents*> FacultyResearcher::viewAvailableLicenseSeats() {
  cout << "--- Available License Seats ---\n" << endl;
  std::vector<Documents*> licenses;
  //implement
  return licenses;
}

//request software licenses for self, returns a bool for if its successful or not
bool FacultyResearcher::requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate) {
  cout << "--- Request Software License ---\n" << endl;
  cout << "License ID: " << licenseID << endl;
  cout << "Start Date: " << startDate << endl;
  cout << "End Date: " << endDate << endl;
  return true;
}

//request software licenses for group
bool FacultyResearcher::requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate){
  cout << "--- Request Group Software License ---\n" << endl;
  cout << "License ID: " << licenseID << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  cout << "Start Date: " << startDate << endl;
  cout << "End Date: " << endDate << endl;
  return true;
}

//view their own licenses
std::vector<Documents*> FacultyResearcher::viewLicenses(){
  cout << "--- My Licenses ---\n" << endl;
  std::vector<Documents*> myLicenses;
  //implement
  return myLicenses;
}

//view a groups licenses
std::vector<Documents*> FacultyResearcher::viewGroupLicenses(int labGroupID){
  cout << "--- Group Licenses ---\n" << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  std::vector<Documents*> groupLicenses;
  //implement
  return groupLicenses;
}

//RESERVATIONS
//view Group reservations
std::vector<Reservations*> FacultyResearcher::viewGroupReservations(int labGroupID){
  cout << "--- Group Reservations ---\n" << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  std::vector<Reservations*> groupReservations;
  //implement
  return groupReservations;
}

//cancel Group reservations
bool FacultyResearcher::cancelGroupReservations(int labGroupID, int reservationID) {
  cout << "--- Cancel Group Reservation ---\n" << endl;
  cout << "Lab Group ID: " << labGroupID << endl;
  cout << "Reservation ID: " << reservationID << endl;
  return true;
}

//make reservation for self, labGroupId only if for group, else its 0 or NULL
bool FacultyResearcher::makeReservation(int reservationID, int labGroupID) {
  cout << "--- Make Reservation ---\n" << endl;
  cout << "Reservation ID: " << reservationID << endl;
  if (labGroupID != 0) {
    cout << "Lab Group ID: " << labGroupID << endl;
  }
  return true;
}

//view own reservations
std::vector<Reservations*> FacultyResearcher::viewMyReservations() {
  cout << "--- My Reservations ---\n" << endl;
  std::vector<Reservations*> myReservations;
  //implement
  return myReservations;
}

//cancel reservation
bool FacultyResearcher::cancelReservation(int reservationID) {
  cout << "--- Cancel Reservation ---\n" << endl;
  cout << "Reservation ID: " << reservationID << endl;
  return true;
}

//DOCUMENTS
//produce a usage report for resource usage by a group
Documents FacultyResearcher::generateUsageReport(Assets* assetUsed, Documents* dataRecords) {
  cout << "--- Generate Usage Report ---\n" << endl;
  Documents report;
  //implement
  return report;
}

//reserve multiple assets
bool FacultyResearcher::reserveMultipleAssets(std::vector<int> assetIDs, const std::string& startDate, const std::string& endDate) {
  cout << "--- Reserve Multiple Assets ---\n" << endl;
  cout << "Number of assets: " << assetIDs.size() << endl;
  cout << "Start Date: " << startDate << endl;
  cout << "End Date: " << endDate << endl;
  
  //check each asset information and check status
  for (int assetID : assetIDs) {
    cout << "Reserving Asset ID: " << assetID << endl;
  }
  
  return true;
}

//search and filter assets
std::vector<Assets*> FacultyResearcher::searchAssets(const std::string& category, const std::string& status) {
  cout << "--- Search/Filter Assets ---\n" << endl;
  if (!category.empty()) {
    cout << "Category Filter: " << category << endl;
  }
  if (!status.empty()) {
    cout << "Status Filter: " << status << endl;
  }
  
  std::vector<Assets*> filteredAssets;
  //implement
  cout << "Displaying filtered results..." << endl;
  
  return filteredAssets;
}

//view all available
std::vector<Assets*> FacultyResearcher::viewAvailableAssets() {
  cout << "--- Available Assets ---\n" << endl;
  std::vector<Assets*> availableAssets;
  //implement
  cout << "Displaying all available assets..." << endl;
  
  return availableAssets;
}