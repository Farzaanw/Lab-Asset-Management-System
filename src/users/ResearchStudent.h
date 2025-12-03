<<<<<<< HEAD
//CS-2: ResearchStudent.h
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#ifndef RESEARCHSTUDENT_H
#define RESEARCHSTUDENT_H

#include <string>
#include <vector>
#include "./library/nlohmann/json.hpp"
#include "User.h"

// Forward declarations
class SystemController;

class ResearchStudent : public User {
private:
    int studentID;
    SystemController* system;

public:
    // Constructor
    ResearchStudent(const std::string& firstName,
                   const std::string& lastName,
                   const std::string& email,
                   int role,
                   int studentID,
                   SystemController* sys);

    // Destructor
    virtual ~ResearchStudent();

    // Main menu entry point
    int main();

    // Helper function for making reservations
    bool makeReservation(int assetID, const std::string& assetType);

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
=======
//CS-2: ResearchStudent.cpp
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#include <string>
#include <vector>
#include "Reservations.h"
#include "Assets.h"
#pragma once

class ResearchStudent: public User {
private:
    int studentID;
    std::string name;
    std::string email;

public:
    bool permissions;
    std::vector<Reservations*> reservations; //list of all reservations the student has made

    //make a reservation for an asset (requires base-level permissions)
    bool reserveAsset(int assetID, bool permissions);

    //cancel an existing reservation (checks permissions and reservation validity)
    bool cancelReservation(int assetID, bool permissions);

    //view available lab equipment and their current status
    std::vector<Assets> viewEquipment(); // using vector instead of list

    //record feedback on an asset after use (rating, condition, comments)
    bool submitUsageFeedback(int equipmentID, const std::string& comments, int rating); // Tied to UR-331

    //update profile information such as name or email
    bool updateUserProfile(const std::string& newName, const std::string& newEmail); // Tied to UR-303
};
>>>>>>> class/System
