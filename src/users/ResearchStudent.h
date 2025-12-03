//CS-2: ResearchStudent.cpp
//Author: Jacob Munly
//Supports: UR-200 to UR-230, UR-303, UR-331
//Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]

#ifndef RESEARCHSTUDENT_H
#define RESEARCHSTUDENT_H

#include <string>
#include <vector>
#include <fstream>
#include "./library/nlohmann/json.hpp"
#include "Reservations.h"
#include "Assets.h"
#include "Documents.h"
#include "User.h"
#include "SystemController.h"

using json = nlohmann::json;

class ResearchStudent: public User {
private:
    int studentID;
    std::string Fname;
    std::string Lname;
    std::string email;
    bool permissions;
    SystemController* system;

public:
    //Constructor
    ResearchStudent(const std::string& firstName,
                   const std::string& lastName,
                   const std::string& email,
                   int role,
                   int studentID,
                   SystemController* sys);

    //Destructor
    virtual ~ResearchStudent() override;

    //main starting point
    int main();

    //Menus
    void assetManagementMenu();
    void reservationsMenu();
    void profileMenu();

    //BASIC
    //Navigation, displays the dashboard and the user actions
    void display_page();
    void user_actions();

    //ASSETS
    //make a reservation for an asset (same as Faculty but student level permissions)
    bool reserveAsset();
    
    //Return an asset
    bool return_asset();
    
    //view own assets
    std::vector<Assets*> viewAssets();
    
    //search and filter assets 
    std::vector<Assets*> searchAssets(const std::string& category, const std::string& status);
    
    //view all available assets
    std::vector<Assets*> viewAvailableAssets();

    //RESERVATIONS
    //view own reservations
    std::vector<Reservations*> viewMyReservations();
    
    //cancel own reservation
    bool cancelReservation(int reservationID);
    
    //make reservation (students can only make for self, no group option)
    bool makeReservation(int reservationID);

    //STUDENT SPECIFIC FEATURES
    //record feedback on an asset after use (rating, condition, comments)
    bool submitUsageFeedback(int equipmentID, const std::string& comments, int rating);
    
    //update profile information such as name or email
    bool updateUserProfile(const std::string& newName, const std::string& newEmail);
};

#endif