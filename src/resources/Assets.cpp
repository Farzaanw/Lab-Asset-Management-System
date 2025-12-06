#include "Assets.h"

//View all available assets

bool Assets::viewAvailableAssets() {
    cout << "--- Available Assets ---\n" << endl;
    
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }

    try {
        inFile >> assets;
    } catch (const std::exception& e) {
        cerr << "Error reading JSON: " << e.what() << endl;
        inFile.close();
        return false;
    }
    inFile.close();

    if (assets.empty()) {
        cout << "No assets found." << endl;
        return true;
    }

    cout << "Listing all available assets:\n" << endl;
    bool hasAvailable = false;
    for (const auto& asset : assets) {
        if (asset["operationalStatus"] == "available") {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Condition: " << asset["condition"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "Description: " << asset["description"] << endl;
            cout << "-----------------------------------" << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No assets currently available." << endl;
    }

    return true;
};

//SOFTWARE LICENSE MANAGEMENT
//View available software license seats
bool Assets::viewAvailableLicenseSeats() {
    cout << "--- Available Software License Seats ---\n" << endl;
    cout << "TODO: Implement viewAvailableLicenseSeats" << endl;
    return true;
}

//View own licenses
bool Assets::viewLicenses() {
    cout << "--- My Licenses ---\n" << endl;
    cout << "TODO: Implement viewLicenses" << endl;
    return true;
}

//View group licenses
bool Assets::viewGroupLicenses(int labGroupID) {
    cout << "--- View Group Licenses ---\n" << endl;
    cout << "TODO: Implement viewGroupLicenses" << endl;
    return true;
}

//Search and filter assets
bool Assets::searchAssets(const std::string& category, const std::string& status) {
    cout << "--- Search/Filter Assets ---\n" << endl;

    string cat = category;
    string stat = status;
    
    if (cat.empty()) {
        cout << "Enter category (or leave blank): ";
        getline(cin, cat);
    }
    if (stat.empty()) {
        cout << "Enter status (or leave blank): ";
        getline(cin, stat);
    }
    
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();
    
    bool found = false;
    for (const auto& asset : assets) {
        bool matchCategory = cat.empty() || asset["category"] == cat;
        bool matchStatus = stat.empty() || asset["operationalStatus"] == stat;
        
        if (matchCategory && matchStatus) {
            cout << "ID: " << asset["id"] << endl;
            cout << "Name: " << asset["name"] << endl;
            cout << "Category: " << asset["category"] << endl;
            cout << "Status: " << asset["operationalStatus"] << endl;
            cout << "Location: " << asset["location"] << endl;
            cout << "-----------------------------------" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No assets found matching criteria." << endl;
    }
    
    return true;
}

//View assets checked out by a specific student
bool Assets::viewStudentAssets(const std::string& studentEmail) {
    cout << "--- View Student Assets ---\n" << endl;

    string email = studentEmail;
    if (email.empty()) {
        cout << "Enter student email: ";
        getline(cin, email);
    }

    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool studentFound = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == email && 
            account["role"] == "research student") {
            studentFound = true;
            
            if (account["reservations"].empty()) {
                cout << "Student " << account["firstName"] << " " << account["lastName"] 
                     << " has no assets checked out." << endl;
                return true;
            }

            cout << "Assets for " << account["firstName"] << " " << account["lastName"] << ":\n" << endl;
            for (const auto& res : account["reservations"]) {
                if (res["status"] == "confirmed" || res["status"] == "approved") {
                    cout << "Asset ID: " << res["assetID"] << endl;
                    cout << "Name: " << res["assetName"] << endl;
                    cout << "Start Date: " << res["startDate"] << endl;
                    cout << "End Date: " << res["endDate"] << endl;
                    cout << "Status: " << res["status"] << endl;
                    cout << "-----------------------------------" << endl;
                }
            }
            break;
        }
    }

    if (!studentFound) {
        cout << "Student with email " << email << " not found." << endl;
        return false;
    }

    return true;
}

//view own assets
bool Assets::viewAssets(const std::string& email) {
    cout << "--- My Assets ---\n" << endl;

    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool hasAssets = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            if (account["reservations"].empty()) {
                cout << "You have no assets checked out." << endl;
                return true;
            }

            for (const auto& res : account["reservations"]) {
                if (res["status"] == "confirmed" || res["status"] == "approved") {
                    cout << "Asset ID: " << res["assetID"] << endl;
                    cout << "Name: " << res["assetName"] << endl;
                    cout << "Start Date: " << res["startDate"] << endl;
                    cout << "End Date: " << res["endDate"] << endl;
                    cout << "Status: " << res["status"] << endl;
                    cout << "-----------------------------------" << endl;
                    hasAssets = true;
                }
            }
            break;
        }
    }

    if (!hasAssets) {
        cout << "You have no assets checked out." << endl;
    }

    return true;
}

bool Assets::listAssets(){
	json assets;

	ifstream inFile("../../data/assets.json");

	try {
		inFile >> assets;
	} catch (const std::exception& e) {
		cerr << "Error reading JSON: " << e.what() << endl;
		inFile.close();
		return false;
	}
	inFile.close();

	if (assets.empty()) {
		cout << "No assets found." << endl;
		return true;
	}

	cout << "Listing all assets:\n" << endl;
	for (const auto& asset : assets) {
		cout << "ID: " << asset["id"] << endl;
		cout << "Name: " << asset["name"] << endl;
		cout << "Category: " << asset["category"] << endl;
		cout << "Operational Status: " << asset["operationalStatus"] << endl;
		cout << "Condition: " << asset["condition"] << endl;
		cout << "Location: " << asset["location"] << endl;
		cout << "Clearance Level: " << asset["clearanceLevel"] << endl;
		if (asset["category"] == "consumable") {
			cout << "Quantity On Hand (grams): " << asset["quantityOnHand(grams)"] << endl;
			cout << "Minimum Threshold (grams): " << asset["minimumThreshold(grams)"] << endl;
		}
		cout << "Description: " << asset["description"] << endl;
		cout << "-----------------------------------" << endl;
	}
	return true;
}

bool Assets::removeAsset(){
	int ID = -1;
	json assets;
    string assetsFile = "../../data/assets.json";
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
        listAssets();
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

bool Assets::addAsset(){
	json assetFile;
	json asset;
    string assetsFile = "../../data/assets.json";
	ifstream inFile(assetsFile);
	if (inFile.is_open()) {
		inFile >> assetFile;
		inFile.close();
	}

	string name, category, status, accessLevel, condition, location, quantityOnHand, minimumThreshold, description;
	cout << "Creating a new asset." << endl;
	cout << "Enter asset name: ";
	getline(cin, name);
	cout << "Enter asset category (equipment, consumable, software): ";
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
	cout << "Enter asset status (available, reserved, out of service): ";
	getline(cin, status);
	while (assetStatus.find(status) == assetStatus.end()) {
		cout << "Invalid asset status entered. Please enter a valid asset status from the list:" << endl << "available" << endl << "reserved" << endl << "out of service" << endl;
		getline(cin, status);
	}
	cout << "Enter asset condition: ";
	getline(cin, condition);
	cout << "Enter asset access level (1, 2, or 3): ";
	getline(cin, accessLevel);
	while (clearanceLevels.find(accessLevel) == clearanceLevels.end()) {
		cout << "Invalid access level entered. Please enter a valid access level from the list:		1, 2, or 3" << endl;
		getline(cin, accessLevel);
	}
	cout << "Enter asset location: ";
	getline(cin, location);
	cout << "Enter asset description: ";
	getline(cin, description);

	//role validation

	// Generate unique ID. This will generate the highest ID in the accounts.json file.
	int maxID = 0;
	for (const auto& asset : assetFile) {
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
	asset["condition"] = condition;
	asset["location"] = location;
	asset["clearanceLevel"] = accessLevel;
	if (category == "consumable") {
		asset["quantityOnHand(grams)"] = quantityOnHand;
		asset["minimumThreshold(grams)"] = minimumThreshold;
	}
	asset["description"] = description;

	// Add to JSON array
	assetFile.push_back(asset);

	// Save back to file
	ofstream outFile(assetsFile);
	outFile << setw(4) << assetFile << endl;
	outFile.close();
	return true;
}

bool Assets::updateAsset(){
	int ID;
	json assets;
    string assetsFile = "../../data/assets.json";
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
	cout << endl << "Please note that the asset category and asset ID cannot be changed once created." << endl;
	cout << "\nEnter new information (leave blank to keep current value):\n";

	// Update fields (ID is NOT updated)
	for (auto& [key, value] : assetToUpdate->items()) {
		if (key == "id") continue;
		if (key == "category") continue;
		string input;
		cout << key << " (" << value << "): ";
		getline(cin, input);

		if (input.empty()) {
			continue;
		}
		if (key == "operationalStatus") {
			while (assetStatus.find(input) == assetStatus.end()) {
				cout << "Invalid asset status entered. Please enter a valid asset status from the list:" << endl << "available" << endl << "reserved" << endl << "out of service" << endl;
				getline(cin, input);
			}
		}
		if (key == "clearanceLevel") {
			while (clearanceLevels.find(input) == clearanceLevels.end()) {
				cout << "Invalid access level entered. Please enter a valid access level from the list:		1, 2, or 3" << endl;
				getline(cin, input);
			}
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

//Return an asset
bool Assets::return_asset(const std::string& email) {
    cout << "--- Return Asset ---\n" << endl;

    // First, show user's checked-out assets
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    // get user's account
    json* userAccount = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            userAccount = &account;
            break;
        }
    }
    if (!userAccount || (*userAccount)["reservations"].empty()) {
        cout << "You have no assets to return." << endl;
        return false;
    }

    cout << "Your Reserved Assets:\n" << endl;
    for (const auto& res : (*userAccount)["reservations"]) {
        if (res["status"] == "confirmed" || res["status"] == "approved") {
            cout << "Asset ID: " << res["assetID"] << " | Name: " << res["assetName"] 
                 << " | Start: " << res["startDate"] << " | End: " << res["endDate"] << endl;
        }
    }

    cout << "\n-----------------------------------\n" << endl;

    // Ask which asset to return
    int assetID;
    cout << "Enter Asset ID to return: ";
    cin >> assetID;
    cin.ignore();

    // Update asset status to available
    json assets;
    ifstream assetFile("../../data/assets.json");
    if (!assetFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    assetFile >> assets;
    assetFile.close();

    bool found = false;
    for (auto& asset : assets) {
        if (asset["id"].get<int>() == assetID) {
            asset["operationalStatus"] = "available";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Asset ID not found" << endl;
        return false;
    }

    // Remove reservation from user's account
    auto& reservations = (*userAccount)["reservations"];
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)["assetID"].get<int>() == assetID) {
            reservations.erase(it);
            break;
        }
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();

    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();

    cout << "Asset returned successfully!" << endl;
    return true;
}