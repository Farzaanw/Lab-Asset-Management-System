//Author: Kai Johnson
#ifndef LABMANAGER_H
#define LABMANAGER_H

#include <string>
#include <vector>
#include "../library/nlohmann/json.hpp"
#include "User.h"

using namespace std;

// Forward declarations
class SystemController;

class LabManager : public User {
private:
	const std::string policiesFile = "../../data/policies.json";
    SystemController* system;
	
public:
	LabManager(const std::string& firstName = "",
			   const std::string& lastName = "",
			   const std::string& email = "",
			   SystemController* sys = nullptr);

	void main() override;

	// Destructor
    virtual ~LabManager();

	// Override pure virtual function from User
    std::string getRole() const override;

	// lists current policies
	bool listPolicies();

	// only includes max booking duration and advance booking horizon
	bool changePolicies();

	// Returns the value of a specific policy
	// policyType can be "MAXBOOKINGDURATION" or "ADVANCEBOOKINGHORIZON"
	int getPolicy(string policyType);
};

#endif