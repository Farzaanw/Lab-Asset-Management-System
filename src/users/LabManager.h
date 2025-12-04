//Author: Kai Johnson

#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <filesystem>
#include "User.h"
#include "../library/nlohmann/json.hpp"

using namespace std;

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

class LabManager : public User {
private:
	const string policiesFile = "../../data/policies.json";
	
public:
	LabManager(const std::string& firstName = "",
			   const std::string& lastName = "",
			   const std::string& email = "",
			   int role = 0)
		: User(firstName, lastName, email, role) {};


	bool main();
	// lists current policies
	bool listPolicies();

	// only includes max booking duration and advance booking horizon
	bool changePolicies();

	// Returns the value of a specific policy
	// policyType can be "MAXBOOKINGDURATION" or "ADVANCEBOOKINGHORIZON"
	int getPolicy(string policyType);
};
