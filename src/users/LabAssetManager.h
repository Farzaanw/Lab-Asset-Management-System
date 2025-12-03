<<<<<<< HEAD
// CS-4:LabAssetManager.h
// Auth: Kai Johnson
// Supports: UR-400 to UR-431
// Collaborators:  <Assets>[1..*], <document>[0..*], <PI>[1]
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <filesystem>
#include "../library/nlohmann/json.hpp"
//#include "../SystemController.cpp"
#include "User.h"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::ordered_json; // By default json has alphabetical order for keys this prevents that.

class LabAssetManager : public User {
private:
	chrono::system_clock::time_point lastInventoryCheck;
	const string accountsFile = "../../data/accounts.json";
	const string assetsFile = "../../data/assets.json";
	const string documentsFile = "../../data/documents.json";
	const string documentsFolder = "../../data/documents/";
	const string usageLogFile = "../../data/usage_logs.json";
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
			   int role = 0)
		: User(firstName, lastName, email, role) {};

	int main();

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
	bool listDocuments();

	bool uploadDocument();
	//logs
	bool viewLogs();
};
=======
// CS-4:LabAssetManager.h
// Auth: Kai Johnson
// Supports: UR-400 to UR-431
// Collaborators:  <Assets>[1..*], <document>[0..*], <PI>[1]
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <filesystem>
#include "../library/nlohmann/json.hpp"
#include "../SystemController.cpp"
#include "User.h"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::ordered_json; // By default json has alphabetical order for keys this prevents that.

class LabAssetManager : public User {
private:
	chrono::system_clock::time_point lastInventoryCheck;
	const string accountsFile = "../../data/accounts.json";
	const string assetsFile = "../../data/assets.json";
	const string documentsFile = "../../data/documents.json";
	const string documentsFolder = "../../data/documents/";
	const string usageLogFile = "../../data/usage_logs.json";
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
			   int role = 0)
		: User(firstName, lastName, email, role) {};

	int main();

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
	bool listDocuments();

	bool uploadDocument();
	//logs
	bool viewLogs();
};
>>>>>>> class/System
