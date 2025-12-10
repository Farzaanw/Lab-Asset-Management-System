// CS-1: FacultyResearcher.cpp
// Author: Jack Dunlap
// Supports: UR-101, UR-102, UR-103, UR-104
// Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*]

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <ctime>
#include "FacultyResearcher.h"
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"
#include "../resources/Notifications.h"
#include "../resources/Reservations.h"
static Assets *a = nullptr;
static Reservations *r = nullptr;
static Notifications n;
using namespace std;
using json = nlohmann::json;

// Constructor
FacultyResearcher::FacultyResearcher(const std::string &email,
                                     SystemController *sys)
    : User(email, sys),
      system(sys) {}

// Destructor
FacultyResearcher::~FacultyResearcher() {}

// Override getRole
std::string FacultyResearcher::getRole() const
{
    return "faculty researcher";
}

// Main menu
void FacultyResearcher::main()
{
    if (r == nullptr)
    {
        r = new Reservations(system);
    }
    if (a == nullptr)
    {
        a = new Assets(system);
    }
    cout << "\n=============================================" << endl;
    cout << "Welcome " << getEmail() << "!" << endl;
    cout << "=============================================\n"
         << endl;

    while (true)
    {
        cout << endl
             << "---Faculty Researcher Main Menu---" << endl;
        cout << "1. Reserve Asset" << endl;
        cout << "2. Reserve Multiple Assets" << endl;
        //cout << "3. Return Asset" << endl;
        cout << "3. View Available Assets" << endl;
        cout << "4. Search/Filter Assets" << endl;
        cout << "5. View My Reservations" << endl;
        cout << "6. Cancel Reservation" << endl;
        cout << "7. View Notifications" << endl;
        cout << "8. View Student Reservations" << endl;
        cout << "9. View Student Usage" << endl;
        cout << "10. Generate Usage Report" << endl;
        cout << "11. Check-Out (start)" << endl;
        cout << "12. Check-In (return)" << endl;
        cout << "13. Logout" << endl;
        cout << "Please enter your choice: ";

        string choice;
        getline(cin, choice);
        cout << endl;

        if (choice == "1")
        {
            string email = getEmail();
            int result = r->reserveAsset(email);
            if (result == 0)
            {
                cout << "Asset officially reserved." << endl;
            }
            else if (result == 2)
            {
                cout << "Asset reservation requires approval. Request sent to Lab Manager." << endl;
            }
            else
            {
                // when return -1
                cout << "Asset reservation failed." << endl;
            }
        }
        else if (choice == "2")
        {
            string email = getEmail();
            r->reserveMultipleAssets(email);
        }
        /*
        else if (choice == "3")
        {
            //string email = getEmail();
            //a->return_asset(email);
        }
        */
        else if (choice == "3")
        {
            a->viewAvailableAssets();
        }
        else if (choice == "4")
        {
            a->searchAssets("", "");
        }
        else if (choice == "5")
        {
            string email = getEmail();
            r->viewMyReservations(email);
        }
        else if (choice == "6")
        {
            string email = getEmail();
            r->cancelReservation(email);
        }

        else if (choice == "7")
        {
            n.view_notifications(getEmail());
        }
        else if (choice == "8")
        {
            a->viewStudentAssets("");
        }
        else if (choice == "9")
        {
            viewStudentUsage();
        }
        else if (choice == "11")
        {
            generateUsageReport(0);
        }
        else if (choice == "11")
        {
            // Check-Out: will list eligible reservations that have started
            Reservations(system).checkOut(getEmail());
        }
        else if (choice == "12")
        {
            // Check-In: will list currently checked_out reservations
            Reservations(system).checkIn(getEmail());
        }
        else if (choice == "13")
        {
            cout << "Logging out of Faculty Researcher." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// NAVIGATION/DISPLAY
void FacultyResearcher::display_page()
{
    cout << "--- Faculty Researcher Dashboard ---" << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Role: " << getRole() << endl;
}

void FacultyResearcher::user_actions()
{
    cout << "Faculty member can perform various actions." << endl;
}

// Cancel group reservation
bool FacultyResearcher::viewStudentUsage()
{
    cout << "--- View Student Usage ---\n"
         << endl;
    cout << "TODO: Implement viewStudentUsage" << endl;
    system->update_usage_log("User viewed student usage logs");
    return true;
}

// DOCUMENTS/REPORTS
// Generate usage report for resource usage by a group
bool FacultyResearcher::generateUsageReport(int labGroupID)
{
    cout << "--- Generate Usage Report ---\n"
         << endl;
    cout << "TODO: Implement generateUsageReport" << endl;
    system->update_usage_log("User generated a usage report");
    return true;
}
