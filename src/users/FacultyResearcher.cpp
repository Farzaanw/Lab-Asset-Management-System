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

/**
 * Main menu:
 * Presents the Faculty Researcher command menu and dispatches to the appropriate
 * actions. Lazily constructs shared helpers (Reservations, Assets) on first use.
 *
 * What each option does connects to:
 * Menu options:
 *  1) Reserve Asset                → Reservations::reserveAsset()
 *  2) Reserve Multiple Assets      → Reservations::reserveMultipleAssets()
 *  3) Return Asset                 → Assets::return_asset()
 *  4) View Available Assets        → Assets::viewAvailableAssets()
 *  5) Search/Filter Assets         → Assets::searchAssets("", "")
 *  6) View My Reservations         → Reservations::viewMyReservations()
 *  7) Cancel Reservation           → Reservations::cancelReservation()
 *  8) View Notifications           → Notifications::view_notifications()
 *  9) View Student Reservations    → Assets::viewStudentAssets("")
 * 10) Check-Out (start)            → Reservations(system).checkOut()
 * 11) Check-In (return)            → Reservations(system).checkIn()
 * 12) Logout                       → exits loop
 *
 * Prints the status messages for success/failure and uses SystemController (via
 * underlying methods) to log important actions.
 *
 * Returns nothing (this runs until user selects Logout).
 */
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
        cout << "9. Check-Out (start)" << endl;
        cout << "10. Check-In (return)" << endl;
        cout << "11. Logout" << endl;
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
            Reservations(system).checkOut(getEmail());
        }
        else if (choice == "10")
        {
            Reservations(system).checkIn(getEmail());
        }
        else if (choice == "11")
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

/**
 * Display page:
 * Prints a minimal dashboard header for the Faculty Researcher including
 * the user’s email and role.
 *
 * Returns nothing (prints to std::cout).
 */

void FacultyResearcher::display_page()
{
    cout << "--- Faculty Researcher Dashboard ---" << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Role: " << getRole() << endl;
}

/**
 * User actions:
 * Prints a short description that the faculty member can perform actions.
 * Intended as a hook for future UI/help output.
 *
 * Returns nothing.
 */
void FacultyResearcher::user_actions()
{
    cout << "Faculty member can perform various actions." << endl;
}
