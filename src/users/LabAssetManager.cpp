#include "LabAssetManager.h"

// Example usage (copy into main.cpp):
//    #include "../users/LabAssetManager.h"
//      LabAssetManager manager;
//      manager.createAccount();
bool updateAccount(const std::string& accountID);
	//asks for confirmation to delete account
	//deletes account from file/database
bool deleteAccount(const std::string& accountID);

	//asset Management
Assets addAsset(const Assets& assetInfo);
bool updateAsset(const std::string& assetID, const Assets& updatedInfo);
bool removeAsset(const std::string& assetID);
bool flagAsset(const std::string& assetID);

	//inventory & Documents
Documents InventoryCheck();
Documents uploadDocument(const Documents& document);



void LabAssetManager::createAccount() {
    string accountsFile = "Database/accounts.json";
    json j;
    ifstream inFile(accountsFile);
    if (inFile.is_open()) {
        inFile >> j;
        inFile.close();
    }

    set<string> validRoles = {
        "research student",
        "faculty researcher",
        "lab manager",
        "lab asset manager"
    };

    //asks user for account info
    string firstName, lastName, role, email, password;
    cout << "Creating a new account." << endl;
    cout << "Enter first name: ";
    getline(cin, firstName);
    cout << "Enter last name: ";
    getline(cin, lastName);
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter role: ";
    getline(cin, role);

    //role validation
    while (validRoles.find(role) == validRoles.end()) {
        cout << "Invalid role entered. Please enter a valid role from the list:" << endl << "research student" << endl << "faculty researcher" << endl << "lab manager" << endl << "lab asset manager" << endl;
        getline(cin, role);
    }

    //password
    cout << "Enter password: ";
    getline(cin, password);
    cout << endl;

    // Generate unique ID
    int maxID = 0;
    for (const auto& account : j) {
        if (account.contains("id") && account["id"].is_number()) {
            int id = account["id"];
            if (id > maxID) maxID = id;
        }
    }
    int uniqueID = maxID + 1;

    // Create account
    json account = {
        {"id", uniqueID},
        {"firstName", firstName},
        {"lastName", lastName},
        {"email", email},
        {"role", role},
        {"password", password}
    };

    // Add to JSON array
    j.push_back(account);

    // Save back to file
    ofstream outFile(accountsFile);
    outFile << setw(4) << j << endl;
    outFile.close();
};