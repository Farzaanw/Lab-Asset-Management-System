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

//BASIC
//Navigation, displays the dashboard and the user actions
void FacultyResearcher::display_page() {
  cout << "\n=============================================" << endl;
  cout << "Faculty Researcher Dashboard" << endl;
  cout << "=============================================" << endl;
  cout << "Name: " << Fname << " " << Lname << endl;
  cout << "Email: " << email << endl;
  cout << "FacultyID: " << FacultyID << endl;
  cout << "=============================================\n" << endl;

  cout << "Main Menu: \n" << endl;
  cout << "1. Asset Management" << endl;
  cout << "2. Lab Group Management" << endl;
  cout << "3. Software Licenses" << endl;
  cout << "4. Reports and Documents" << endl;
  cout << "5. Logout" << endl;
  cout << "=============================================\n" << endl;
}

//Main Actions, more general ones
void FacultyResearcher::user_actions() {
  bool loggedIn = true;
  int choice;

  while(loggedIn) {
    display_page();
    do {
      cout << "Choose an Option: " << endl;
      cin >> choice;
      if(!(cin >> choice) || choice < 1 || choice > 5) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(!(cin >> choice) || choice < 1 || choice > 5);

    if(choice == 1) {
      //call asset management options
    } else if(choice == 2) {
      //call lab group options
    } else if(choice == 3) {
      //call software licenses options
    } else if(choice == 4) {
      //call reports and documents
    } else {
      loggedIn = false;
    }
  }
}

//Asset Management
void FacultyResearcher::assetManagementMenu() {
  bool inMenu = true;
  int choice;
  int assetID, reservationID;
  
  while (inMenu) {
    cout << "\n========================================" << endl;
    cout << "       ASSET MANAGEMENT" << endl;
    cout << "========================================" << endl;
    cout << "1. Reserve Asset" << endl;
    cout << "2. Return Asset" << endl;
    cout << "3. View Available Assets" << endl;
    cout << "4. View My Reservations" << endl;
    cout << "5. Cancel Reservation" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "========================================\n" << endl;
      
    do {
      cout << "Choose an Option: " << endl;
      cin >> choice;
      if(!(cin >> choice) || choice < 1 || choice > 6) {
        cout << "Incorrect input, please try again" << endl;
      }
    } while(!(cin >> choice) || choice < 1 || choice > 6);
      
    if (choice == 1) { // Reserve Asset
          cout << "Enter Asset ID to reserve: ";
          cin >> assetID;
          if (Asset->reserveAsset(assetID, permissions)) {
              cout << "Asset reserved successfully!" << endl;
          } else {
              cout << "Failed to reserve asset." << endl;
          }
    }
    else if (choice == 2) {
        cout << "\nEnter Reservation ID to return: ";
        cin >> reservationID;
        if (return_asset(reservationID)) {
            cout << "Asset returned successfully!" << endl;
        } else {
            cout << "Failed to return asset." << endl;
        }
    }
    else if (choice == 3) {
        //view available assets
        //reference assets class
    }
    else if (choice == 4) {
        //view own reservations
        //referwen
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
    else{ // Back
        inMenu = false;
    }
  }
}

//ASSETS
//make a reservation for an asset, returns bool - was it successful in making reservation}
//same as use asset, when they reserve it, we are assuming they are using it
bool FacultyResearcher::reserveAsset(int assetID, bool permissions) {
  cout << "--- Reserve Asset ---\n" << endl;
  //Asset Information
}
//Return an asser
bool FacultyResearcher::return_asset(int reservationID) {
  cout << "--- Return Asset ---\n" << endl;
  //Return Information
}
//view own assets
std::vector<Assets> FacultyResearcher::viewAssets() {

}
// view a student's list of assets they have checked out
std::vector<Assets> FacultyResearcher::viewStudentAssets(int studentID) {
  cout << "--- Student Assets ---\n" << endl;
  //Enter studentID
  //List the current assets the have
}

//GROUP SECTION
//view the groups list of assets they have checked out
vector<Assets> FacultyResearcher::viewGroupAssets(int labGroupID) {

}
//view group usage records(their usage log)
void FacultyResearcher::viewGroupUsage(int labGroupID) {

}
//display a record of the group the faculty is in charge of and their work they have done
void FacultyResearcher::viewGroup(int LabGroupID) {

}

//SOFTWARE LICENSE
//View the available software licenses
std::vector<Documents> FacultyResearcher::viewAvailableLicenseSeats() {

}
//Request software licenses for self, returns a bool for if its successful or not
bool FacultyResearcher::requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate) {

}
//Request software licesnes for group
bool FacultyResearcher::requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate){

}
//view their own licenses
std::vector<Documents> FacultyResearcher::viewLicenses(){

}
//view a groups licenses
std::vector<Documents> FacultyResearcher::viewGroupLicenses(int labGroupID){

}

//Reservations
//View Group reservations
std::vector<Reservations*> FacultyResearcher::viewGroupReservations(int labGroupID){

}
//Manage Group reservations
bool FacultyResearcher::manageGroupReservations(int labGroupID, int reservationID /*Add Info needed to manage reservation*/) {

}

//DOCUMENTS
//produce a usage report for resource usage by a group
Documents FacultyResearcher::generateUsageReport(Assets assetUsed, Documents dataRecords) {

}