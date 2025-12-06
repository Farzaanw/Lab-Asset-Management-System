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
    // Append a usage log entry when a reservation is created

public:

    static bool appendUsageLog(const std::string& email, int assetID, const std::string& startTime, const std::string& endTime);

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

    // STUDENT SPECIFIC FEATURES
    // Submit feedback after using equipment (rating, comments)
    bool submitUsageFeedback(int equipmentID, const std::string& comments, int rating);
    
    // Update student profile information (name, email)
    bool updateUserProfile(const std::string& newName, const std::string& newEmail);
};

#endif