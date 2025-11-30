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

bool LabAssetManager::addAsset(){
	json j;
	json asset;
	ifstream inFile(assetsFile);
	if (inFile.is_open()) {
		inFile >> j;
		inFile.close();
	}

	string name, category, status, accessLevel, condition, location, quantityOnHand, minimumThreshold, description;
	cout << "Creating a new asset." << endl;
	cout << "Enter asset name: ";
	getline(cin, name);
	cout << "Enter asset category: ";// equipment, consumable, software.
	getline(cin, category);
	while (assetTypes.find(category) == assetTypes.end()) {
		cout << "Invalid asset category entered. Please enter a valid asset category from the list:" << endl << "equipment" << endl << "consumable" << endl << "software" << endl;
		getline(cin, category);
	}
	if (category == "consumable") {
		cout << "Because this asset is a consumeable please enter:" << endl;
		cout << "Quantity on hand (grams): ";
		getline(cin, quantityOnHand);
		cout << "minimum threshold (grams): ";
		getline(cin, minimumThreshold);
	}
	cout << "Enter asset status: ";
	getline(cin, status);
	while (assetStatus.find(status) == assetStatus.end()) {
		cout << "Invalid asset status entered. Please enter a valid asset status from the list:" << endl << "available" << endl << "reserved" << endl << "out of service" << endl;
		getline(cin, status);
	}
	cout << "Enter asset securty access level (numeric): ";
	getline(cin, accessLevel);
	while (clearenceLevels.find(accessLevel) == clearenceLevels.end()) {
		cout << "Invalid access level entered. Please enter a valid access level from the list:" << endl << "Research student: (1)" << endl << "Faculty researcher: (2)" << endl << "Lab manager: (3)" << endl;
		getline(cin, accessLevel);
	}
	cout << "Enter asset condition: ";
	getline(cin, condition);
	cout << "Enter asset location: ";
	getline(cin, location);
	cout << "Enter asset description: ";
	getline(cin, description);

	//role validation

	// Generate unique ID. This will generate the highest ID in the accounts.json file.
	int maxID = 0;
	for (const auto& asset : j) {
		if (asset.contains("id") && asset["id"].is_number()) {
			int id = asset["id"];
			if (id > maxID) maxID = id;
		}
	}
	int uniqueID = maxID + 1;

	// Create asset
	asset["id"] = uniqueID;
	asset["name"] = name;
	asset["category"] = category;
	asset["operationalStatus"] = status;
	asset["accessLevel"] = accessLevel;
	asset["condition"] = condition;
	asset["location"] = location;
	if (category == "consumable") {
		asset["quantityOnHand(grams)"] = quantityOnHand;
		asset["minimumThreshold(grams)"] = minimumThreshold;
	}
	asset["description"] = description;

	// Add to JSON array
	j.push_back(asset);

	// Save back to file
	ofstream outFile(assetsFile);
	outFile << setw(4) << j << endl;
	outFile.close();
	return true;
}

bool LabAssetManager::updateAsset(){
	int ID;
	json assets;

	// Load the JSON file
	ifstream inFile(assetsFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open assets.json" << endl;
		return false;
	}
	inFile >> assets;
	inFile.close();

	// Loop until valid ID is entered
	json* assetToUpdate = nullptr;
	while (!assetToUpdate) {
		string input;
		cout << "Please enter the asset ID you would like to modify (type quit to cancel the modification): ";
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

		// Search for the asset
		for (auto& asset : assets) {
			if (asset["id"].get<int>() == ID) {
				assetToUpdate = &asset;
				break;
			}
		}

		if (!assetToUpdate) {
			cout << "Asset ID not found. Please try again." << endl;
		}
	}

	// Display current account info
	cout << "\nCurrent account information:\n";
	for (auto& [key, value] : assetToUpdate->items()) {
		cout << key << ": " << value << endl;
	}

	cout << "\nEnter new information (leave blank to keep current value):\n";

	// Update fields (ID is NOT updated)
	for (auto& [key, value] : assetToUpdate->items()) {
		if (key == "id") continue;

		string input;
		cout << key << " (" << value << "): ";
		getline(cin, input);

		if (input.empty()) {
			continue;
		}


		// Update JSON value
		(*assetToUpdate)[key] = input;
	}

	// Save updated JSON back to file
	ofstream outFile(assetsFile);
	outFile << assets.dump(4);
	outFile.close();

	cout << "\nAsset updated successfully!\n";
	return true;
}

bool LabAssetManager::removeAsset(){
	int ID = -1;
    json assets;

    // Load JSON file
    ifstream inFile(assetsFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

    // Pointer to the asset to delete
    json* assetToDelete = nullptr;

    // Loop until user enters a valid ID
    while (!assetToDelete) {
        string input;
        cout << "Please enter the asset ID you want to delete (or type 'quit' to cancel): ";
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

        // Search for asset
        for (auto& asset : assets) {
            if (asset["id"].get<int>() == ID) {
                assetToDelete = &asset;
                break;
            }
        }

        if (!assetToDelete) {
            cout << "Asset ID not found. Please try again.\n";
        }
    }

    // Display asset info before deleting
    cout << "\nAsset found:\n";
    for (auto& [key, value] : assetToDelete->items()) {
        cout << key << ": " << value << endl;
    }

    // Confirmation
    string confirm;
    cout << "\nAre you sure you want to delete this asset? (yes/no): ";
    getline(cin, confirm);

    while (confirm != "yes" && confirm != "no") {
        cout << "Please enter 'yes' or 'no': ";
        getline(cin, confirm);
    }

    if (confirm == "no") {
        cout << "Deletion canceled.\n";
        return false;
    }

    // Delete asset
    for (size_t i = 0; i < assets.size(); i++) {
        if (assets[i]["id"].get<int>() == ID) {
            assets.erase(assets.begin() + i);
            break;
        }
    }

    // Save updated JSON
    ofstream outFile(assetsFile);
    outFile << assets.dump(4);
    outFile.close();

    cout << "\nAsset deleted successfully!\n";
    return true;
}

bool LabAssetManager::replenishAsset(const std::string& assetID, const std::string& amount){
	return true;
}

bool LabAssetManager::trackConsumables(){
	return true;
}

Documents LabAssetManager::viewDocuments(const std::string& documentID){
	return Documents();
}
Documents LabAssetManager::uploadDocument(const Documents& document){
	return Documents();
}