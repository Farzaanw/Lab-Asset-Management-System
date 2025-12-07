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
static Assets a;
static Reservations r;
using namespace std;
using json = nlohmann::json;

//Constructor
FacultyResearcher::FacultyResearcher(const std::string& email,
                                     SystemController* sys)
    : User(email, sys),
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
    cout << "Welcome " << getEmail() <<  "!" << endl;
    cout << "=============================================\n" << endl;

    while(true) {
        cout << endl << "---Faculty Researcher Main Menu---" << endl;

        cout << "Assets: " << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Reserve Multiple Assets" << endl;
        cout << "3. Return Asset" << endl;
        cout << "4. View Available Assets" << endl;
        cout << "5. Search/Filter Assets" << endl;
        cout << "6. View My Reservations" << endl;
        cout << "7. Cancel Reservation" << endl;

        cout << "Software Licenses: " << endl;
        cout << "8. View Available License Seats" << endl; // move to view aavailable ssets
        cout << "9. Request Software License" << endl; //move to reserve asset, chaneg to reserv/request
        cout << "10. View My Licenses" << endl; //move to view my reservations

        cout << "Student/Group Work: " << endl;
        cout << "11. View Student Assets" << endl; //change to view group/student assets
        //when ^ this runs, input either the group ID or student ID, this will show their assets
        cout << "12. View Group Licenses" << endl; //move to above^
        cout << "13. Request Group Software License" << endl; //need to keep
        cout << "14. View Lab Group" << endl; //keep, views students in the group
        cout << "15. View Group Reservations" << endl; //this should be changed to the firsy very one, view student/group assets
        cout << "16. Cancel Group Reservation" << endl; //keep, add student cancelation too

        cout << "Additonal Options: " << endl;
        cout << "17. Generate Usage Report" << endl;
        cout << "18. Logout" << endl;
        cout << "Please enter your choice: ";
        
        string choice;
        getline(cin, choice);
        cout << endl;

        if (choice == "1") {
            string email = getEmail();
            r.reserveAsset(email);
        }
        else if (choice == "2") {
            string email = getEmail();
            r.reserveMultipleAssets(email);
        }
        else if (choice == "3") {
            string email = getEmail();
            a.return_asset(email);
        }
        else if (choice == "4") {
            a.viewAvailableAssets();
        }
        else if (choice == "5") {
            a.searchAssets("", "");
        }
        else if (choice == "6") {
            string email = getEmail();
            r.viewMyReservations(email);
        }
        else if (choice == "7") {
            string email = getEmail();
            r.cancelReservation(email);
        }
        else if (choice == "8") {
            a.viewAvailableLicenseSeats();
        }
        else if (choice == "9") {
            requestSoftwareLicense(0, "", "");
        }
        else if (choice == "10") {
            a.viewLicenses();
            
        }
        else if (choice == "11") {
            a.viewStudentAssets("");
            
        }
        else if (choice == "12") {
            a.viewGroupLicenses(0);
            
        }
        else if (choice == "13") {
            requestSoftwareLicenseGroup(0, 0, "", "");
            
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
    cout << "Email: " << getEmail() << endl;
    cout << "Role: " << getRole() << endl;
}

void FacultyResearcher::user_actions() {
    cout << "Faculty member can perform various actions." << endl;
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
