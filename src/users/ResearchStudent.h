//CS-2: ResearchStudent.h
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#ifndef RESEARCHSTUDENT_H
#define RESEARCHSTUDENT_H

#include <string>
#include <vector>
#include "../library/nlohmann/json.hpp"
#include "User.h"

// Forward declarations
class SystemController;

class ResearchStudent : public User {
private:
    SystemController* system;

public:
    // Constructor
    ResearchStudent(const std::string& firstName,
                   const std::string& lastName,
                   const std::string& email,
                   SystemController* sys);

    // Destructor
    virtual ~ResearchStudent();

    // Main menu entry point
    void main() override;

    // Override pure virtual function from User
    std::string getRole() const override;

    // ASSET MANAGEMENT
    // Reserve an asset for research use
    bool reserveAsset();
    
    // Return a borrowed asset
    bool return_asset();
    
    // View assets currently checked out by this student
    bool viewAssets();
    
    // Search and filter assets by category and status
    bool searchAssets(const std::string& category, const std::string& status);
    
    // View all available assets in the system
    bool viewAvailableAssets();

    // RESERVATION MANAGEMENT
    // View all reservations made by this student
    bool viewMyReservations();
    
    // Cancel a reservation by ID
    bool cancelReservation(int reservationID);

    // STUDENT SPECIFIC FEATURES
    // Submit feedback after using equipment (rating, comments)
    bool submitUsageFeedback(int equipmentID, const std::string& comments, int rating);
    
    // Update student profile information (name, email)
    bool updateUserProfile(const std::string& newName, const std::string& newEmail);
};

#endif