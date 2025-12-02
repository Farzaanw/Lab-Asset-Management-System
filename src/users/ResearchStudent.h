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
