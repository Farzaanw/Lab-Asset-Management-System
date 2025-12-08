// CS-4:LabAssetManager.h
// Auth: Kai Johnson
// Supports: UR-400 to UR-431
// Collaborators:  <Assets>[1..*], <document>[0..*], <PI>[1]
#ifndef LABASSETMANAGER_H
#define LABASSETMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <filesystem>
#include "../library/nlohmann/json.hpp"
#include "../resources/Notifications.h"
#include "User.h"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json; // By default json has alphabetical order for keys this prevents that.

class LabAssetManager : public User {
private:
	SystemController* system;
	chrono::system_clock::time_point lastInventoryCheck;
	const string accountsFile = "../../data/accounts.json";
	const string assetsFile = "../../data/assets.json";
	const string usageLogFile = "../../data/usage_log.json";
	set<string> validRoles = {
		"research student",
		"faculty researcher",
		"lab manager",
		"lab asset manager"
	};

	

	set<string> documentTypes = {
		"manual",
		"warranty",
		"calibration log",
		"maintenance log"
	};
public:
	//constructor
	LabAssetManager(const std::string& email = "",
			   SystemController* system = nullptr);
		// : User(email, system) {};

	void main() override;

	// Destructor
	virtual ~LabAssetManager();

	// Override pure virtual function from User
	std::string getRole() const override;

	//accounts
	bool createAccount();

	bool updateAccount();

	bool deleteAccount();

	bool listAccounts();

	//logs
	bool viewLogs();

	bool viewAuditLog();

	bool setConsumableThreshold();
};

#endif
