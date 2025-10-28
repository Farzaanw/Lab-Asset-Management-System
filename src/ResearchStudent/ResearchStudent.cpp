// CS-3: ResearchStudent.cpp
// Author: Jacob Munly
// Supports: UR-200 to UR-230, UR-303, UR-331
// Collaborators: LabManager[1], FacultyResearcher[1], Reservation[*], Assets[*]
#include <string>
#include <vector>

class ResearchStudent {
Private:
    int studentID;
    std::string name;
    std::string email;

Public:
    bool permissions;
    std::vector<Reservation*> reservations; //list of all reservations the student has made

    // make a reservation for an asset (requires base-level permissions)
    bool reserveAsset(int assetID, bool permissions) { returns bool - was reservation successful }

    // cancel an existing reservation (checks permissions and reservation validity)
    bool cancelReservation(int assetID, bool permissions) { returns bool - was cancellation successful }

    // view available lab equipment and their current status
    std::list<Assets> viewEquipment() { returns list of available assets }

    // record feedback on an asset after use (rating, condition, comments)
    bool submitUsageFeedback(int equipmentID, std::string comments, int rating) { returns bool - feedback recorded } // Tied to UR-331

    // update profile information such as name, email, or contact details
    bool updateUserProfile(std::map<std::string, std::string> updatedInfo) { returns bool - profile successfully updated } // Tied to UR-303
};
