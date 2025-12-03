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
		: User(firstName, lastName, email, role) {}
	//Max booking duration, advance booking horizon, and reservation priority rules.
	bool listPolicies();

	bool changePolicies();

	int getPolicy(string policyType);
};
