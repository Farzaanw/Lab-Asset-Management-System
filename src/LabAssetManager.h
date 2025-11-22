// CS-4:LabAssetManager.h
// Auth: Kai
// Supports: UR-400 to UR-431
// Collaborators:  <Assets>[1..*], <document>[0..*], <PI>[1]
#include <string>
#include "Assets.h"
#include "Documents.h"
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <set>
#include "library/nlohmann/json.hpp"
#pragma once
using namespace std;
using json = nlohmann::json;

class LabAssetManager {
private:
	std::vector<std::string> accountList;
	std::vector<Assets> assetInventory;
	std::vector<Documents> documentRepository;
	std::chrono::system_clock::time_point lastInventoryCheck;
	std::string assetManagerID;
	bool isAdmin;
	bool permissions;

public:
	//constructor
	LabAssetManager() = default;

	//Will ask user for first name and last name, email, password, role
	//Will return a string wether the account has been created successfully or not
	//Stores account in a file/database
	static void createAccount();
	//asks user for accountID to update
	//replaces old info with new info
	bool updateAccount(const std::string& accountID);
	//asks for confirmation to delete account
	//deletes account from file/database
	bool deleteAccount(const std::string& accountID);

	//asset Management
	Assets addAsset(const Assets& assetInfo);
	bool updateAsset(const std::string& assetID, const Assets& updatedInfo);
	bool removeAsset(const std::string& assetID);
	bool flagAssetOutOfService(const std::string& assetID);

	//inventory & Documents
	Documents runInventoryCheck();
	Documents uploadDocument(const Documents& document);
};
