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

	//the level of clearance needed to obtain each asset
	set<string> clearanceLevels = {
		"1", //low clearance level (base clearance)
		"2",  //Medium clearance level
		"3"  //High clearance level
	};

	set<string> assetTypes = {
		"equipment",
		"consumable",
		"software"
	};

	set<string> assetStatus = {
		"available",
		"reserved",
		"out of service"
	};

	set<string> documentTypes = {
		"manual",
		"warranty",
		"calibration log",
		"maintenance log"
	};
public:
	//constructor
	LabAssetManager(const std::string& firstName = "",
			   const std::string& lastName = "",
			   const std::string& email = "",
			   SystemController* system = nullptr);
		// : User(firstName, lastName, email, system) {};

	void main();

	// Destructor
    virtual ~LabAssetManager();

	// Override pure virtual function from User
    std::string getRole() const override;

	//accounts
	bool createAccount();

	bool updateAccount();

	bool deleteAccount();

	bool listAccounts();

	//assets
	bool addAsset();

	bool updateAsset();

	bool removeAsset();

	bool listAssets();

	//inventory & Documents

	//logs
	bool viewLogs();
};

#endif