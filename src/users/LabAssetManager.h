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
	string assetManagerID;
	string accountsFile = "../db/accounts.json";
	string assetsFile = "../db/assets.json";
	set<string> validRoles = {
		"research student",
		"faculty researcher",
		"lab manager",
		"lab asset manager"
    };
	bool isAdmin;
	bool permissions;

public:
	//constructor
	LabAssetManager() = default;

	//Will ask user for first name and last name, email, password, role
	//Will return a string wether the account has been created successfully or not
	//Stores account in a file/database
	bool createAccount();
	//asks user for accountID to update
	//replaces old info with new info
	bool updateAccount();
	//asks for confirmation to delete account
	//deletes account from file/database
	bool deleteAccount();

	bool listAccounts();
	//asset Management
	Assets addAsset(const Assets& assetInfo);
	bool updateAsset(const std::string& assetID, const Assets& updatedInfo);
	bool removeAsset(const std::string& assetID);
	bool flagAsset(const std::string& assetID);
	bool trackConsumables();
	bool replenishAsset(const std::string& assetID);

	//inventory & Documents
	Documents viewDocuments(const std::string& documentID);
	Documents uploadDocument(const Documents& document);
};
