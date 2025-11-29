#include "LabAssetManager.h"

bool LabAssetManager::createAccount() {
	json j;
	json account;
	ifstream inFile(accountsFile);
	if (inFile.is_open()) {
		inFile >> j;
		inFile.close();
	}

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

	// Generate unique ID. This will generate the highest ID in the accounts.json file.
	int maxID = 0;
	for (const auto& account : j) {
		if (account.contains("id") && account["id"].is_number()) {
			int id = account["id"];
			if (id > maxID) maxID = id;
		}
	}
	int uniqueID = maxID + 1;

	// Create account
	account["id"] = uniqueID;
	account["firstName"] = firstName;
	account["lastName"] = lastName;
	account["email"] = email;
	account["role"] = role;
	account["password"] = password;

	// Add to JSON array
	j.push_back(account);

	// Save back to file
	ofstream outFile(accountsFile);
	outFile << setw(4) << j << endl;
	outFile.close();
	return true;
};

bool LabAssetManager::updateAccount() {
	int ID;
	json accounts;

	// Load the JSON file
	ifstream inFile(accountsFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open accounts.json" << endl;
		return false;
	}
	inFile >> accounts;
	inFile.close();

	// Loop until valid ID is entered
	json* accountToUpdate = nullptr;
	while (!accountToUpdate) {
		string input;
		cout << "Please enter the account ID you would like to modify (type quit to cancel the modification): ";
		getline(cin, input);

		if (input == "quit") {
			return false;
		}

		try {
			ID = stoi(input);
		} catch (...) {
			cout << "Invalid input. Please enter a numeric ID." << endl;
			continue;
		}

		// Search for the account
		for (auto& acc : accounts) {
			if (acc["id"].get<int>() == ID) {
				accountToUpdate = &acc;
				break;
			}
		}

		if (!accountToUpdate) {
			cout << "Account ID not found. Please try again." << endl;
		}
	}

	// Display current account info
	cout << "\nCurrent account information:\n";
	for (auto& [key, value] : accountToUpdate->items()) {
		cout << key << ": " << value << endl;
	}

	cout << "\nEnter new information (leave blank to keep current value):\n";

	// Update fields (ID is NOT updated)
	for (auto& [key, value] : accountToUpdate->items()) {
		if (key == "id") continue;

		string input;
		cout << key << " (" << value << "): ";
		getline(cin, input);

		if (input.empty()) {
			continue;
		}

		// Role validation
		if (key == "role") {
			while (validRoles.find(input) == validRoles.end()) {
				cout << "Invalid role. Please enter one of the following:\n";
				for (const auto& role : validRoles) {
					cout << " - " << role << endl;
				}
				getline(cin, input);
			}
		}

		// Update JSON value
		(*accountToUpdate)[key] = input;
	}

	// Save updated JSON back to file
	ofstream outFile(accountsFile);
	outFile << accounts.dump(4);
	outFile.close();

	cout << "\nAccount updated successfully!\n";
	return true;
}

bool LabAssetManager::listAccounts() {
	json accounts;

	ifstream inFile(accountsFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open " << accountsFile << endl;
		return false;
	}

	try {
		inFile >> accounts;
	} catch (const std::exception& e) {
		cerr << "Error reading JSON: " << e.what() << endl;
		inFile.close();
		return false;
	}
	inFile.close();

	if (accounts.empty()) {
		cout << "No accounts found." << endl;
		return true;
	}

	cout << "Listing all accounts:\n" << endl;
	for (const auto& acc : accounts) {
		cout << "ID: " << acc["id"] << endl;
		cout << "First Name: " << acc["firstName"] << endl;
		cout << "Last Name: " << acc["lastName"] << endl;
		cout << "Email: " << acc["email"] << endl;
		cout << "Role: " << acc["role"] << endl;
		cout << "Password: " << acc["password"] << endl;
		cout << "-----------------------------------" << endl;
	}

	return true;
}

bool LabAssetManager::deleteAccount() {
    json accounts;

    // Load JSON file
    ifstream inFile(accountsFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    inFile >> accounts;
    inFile.close();

    // Pointer to the account to delete
    json* accountToDelete = nullptr;
    int ID = -1;

    // Loop until user enters a valid ID
    while (!accountToDelete) {
        string input;
        cout << "Please enter the account ID you want to delete (or type 'quit' to cancel): ";
        getline(cin, input);

        if (input == "quit") {
            cout << "Delete operation canceled.\n";
            return false;
        }

        // Validate numeric ID
        try {
            ID = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a numeric ID.\n";
            continue;
        }

        // Search for account
        for (auto& acc : accounts) {
            if (acc["id"].get<int>() == ID) {
                accountToDelete = &acc;
                break;
            }
        }

        if (!accountToDelete) {
            cout << "Account ID not found. Please try again.\n";
        }
    }

    // Display account info before deleting
    cout << "\nAccount found:\n";
    for (auto& [key, value] : accountToDelete->items()) {
        cout << key << ": " << value << endl;
    }

    // Confirmation
    string confirm;
    cout << "\nAre you sure you want to delete this account? (yes/no): ";
    getline(cin, confirm);

    while (confirm != "yes" && confirm != "no") {
        cout << "Please enter 'yes' or 'no': ";
        getline(cin, confirm);
    }

    if (confirm == "no") {
        cout << "Deletion canceled.\n";
        return false;
    }

    // Delete account
    for (size_t i = 0; i < accounts.size(); i++) {
        if (accounts[i]["id"].get<int>() == ID) {
            accounts.erase(accounts.begin() + i);
            break;
        }
    }

    // Save updated JSON
    ofstream outFile(accountsFile);
    outFile << accounts.dump(4);
    outFile.close();

    cout << "\nAccount deleted successfully!\n";
    return true;
}

Assets LabAssetManager::addAsset(const Assets& assetInfo){
	return Assets();
}
bool LabAssetManager::updateAsset(const std::string& assetID, const Assets& updatedInfo){
	return true;
}
bool LabAssetManager::removeAsset(const std::string& assetID){
	return true;
}
bool LabAssetManager::flagAsset(const std::string& assetID){
	return true;
}
bool LabAssetManager::trackConsumables(){
	return true;
}
bool LabAssetManager::replenishAsset(const std::string& assetID){
	return true;
}

Documents LabAssetManager::viewDocuments(const std::string& documentID){
	return Documents();
}
Documents LabAssetManager::uploadDocument(const Documents& document){
	return Documents();
}