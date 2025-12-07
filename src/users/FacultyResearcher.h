//CS-1: FacultyResearcher.h
//Auth: Dunlap, Jack
//Supports: UR-101, UR-102, UR-103, UR-104
//Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*]

#ifndef FACULTYRESEARCHER_H
#define FACULTYRESEARCHER_H

#include <string>
#include <vector>
#include "../library/nlohmann/json.hpp"
#include "../resources/Notifications.h"
#include "User.h"

// Forward declarations
class SystemController;

class FacultyResearcher : public User {
private:
    SystemController* system;

public:
    // Constructor
    FacultyResearcher(const std::string& email,
                     SystemController* sys);

    // Destructor
    virtual ~FacultyResearcher();

    // Main menu entry point
    void main() override;

    // Override pure virtual function from User
    std::string getRole() const override;

    // NAVIGATION/DISPLAY
    void display_page();
    void user_actions();
    // DOCUMENTS/REPORTS
    // Generate usage report for resource usage by a group
    bool generateUsageReport(int labGroupID);

    bool viewStudentUsage();
};

#endif
