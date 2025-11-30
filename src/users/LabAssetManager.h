// CS-4:LabAssetManager.h
// Auth: Kai
// Supports: UR-400 to UR-431
// Collaborators:  <Assets>[1..*], <document>[0..*], <PI>[1]
#include <string>
#include "../resources/Asset.h"
#include "../resources/Document.h"
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include "../library/nlohmann/json.hpp"
using namespace std;
using json = nlohmann::ordered_json; // By default json has alphabetical order for keys this prevents that.

class LabAssetManager {
private:
	chrono::system_clock::time_point lastInventoryCheck;
	string accountsFile = "../db/accounts.json";
	string assetsFile = "../db/assets.json";
	set<string> validRoles = {
		"research student",
		"faculty researcher",
		"lab manager",
		"lab asset manager"
    };

	//the level of clearence needed to obtain each asset
	set<string> clearenceLevels = {
		"1", // Research Student
		"2", // Facutly Researcher
		"3"  // Lab Manager
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
public:
	//constructor
	LabAssetManager() = default;

	bool createAccount();

	bool updateAccount();

	bool deleteAccount();

	bool listAccounts();

	bool addAsset();

	bool updateAsset();

	bool removeAsset();

	bool trackConsumables();
	
	bool replenishAsset(const std::string& assetID, const std::string& amount);

	//inventory & Documents
	Documents viewDocuments(const std::string& documentID);
	Documents uploadDocument(const Documents& document);
};
