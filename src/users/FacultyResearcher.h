//CS-1: FacultyResearcher.h
//Auth: Dunlap, Jack
//Supports: UR-101, UR-102, UR-103, UR-104
//Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*]

#ifndef FACULTYRESEARCHER_H
#define FACULTYRESEARCHER_H

#include <string>
#include <vector>
#include "../library/nlohmann/json.hpp"
#include "User.h"

// Forward declarations
class SystemController;

class FacultyResearcher : public User {
private:
    SystemController* system;

public:
    // Constructor
    FacultyResearcher(const std::string& firstName,
                     const std::string& lastName,
                     const std::string& email,
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

    // ASSET MANAGEMENT
    // Reserve a single asset
    bool reserveAsset();
    
    // Reserve multiple assets at once
    bool reserveMultipleAssets();
    
    // Return a borrowed asset
    bool return_asset();
    
    // View assets checked out by a specific student
    bool viewStudentAssets(const std::string& studentEmail);
    
    // Search and filter assets by category and status
    bool searchAssets(const std::string& category, const std::string& status);
    
    // View all available assets in the system
    bool viewAvailableAssets();

    // LAB GROUP MANAGEMENT
    // View the group's list of checked-out assets
    bool viewGroupAssets(int labGroupID);
    
    // View group usage records/log
    void viewGroupUsage(int labGroupID);
    
    // Display group information and their work
    void viewGroup(int labGroupID);

    // SOFTWARE LICENSE MANAGEMENT
    // View available software license seats
    bool viewAvailableLicenseSeats();
    
    // Request software license for self
    bool requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate);
    
    // Request software license for group
    bool requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate);
    
    // View own licenses
    bool viewLicenses();
    
    // View group licenses
    bool viewGroupLicenses(int labGroupID);

    // RESERVATION MANAGEMENT
    // View all reservations made by this faculty member
    bool viewMyReservations();
    
    // Cancel own reservation
    bool cancelReservation(int reservationID);
    
    // Make reservation (for self or group)
    bool makeReservation(int reservationID, int labGroupID);
    
    // View group reservations
    bool viewGroupReservations(int labGroupID);
    
    // Cancel group reservation
    bool cancelGroupReservations(int labGroupID, int reservationID);

    // DOCUMENTS/REPORTS
    // Generate usage report for resource usage by a group
    bool generateUsageReport(int labGroupID);
};

#endif