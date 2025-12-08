#include "Assets.h"
#include "../SystemController.h"
#include <vector>

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
				if (sysController) sysController->update_usage_log("User viewed available assets");
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

		if (sysController) sysController->update_usage_log("User viewed available assets");
    return true;
};

//Search and filter assets
bool Assets::searchAssets(const std::string& category, const std::string& status) {
    cout << "--- Search/Filter Assets ---\n" << endl;

    string cat = category;
    string stat = status;
    
    if (cat.empty()) {
        cout << "Enter category (leave blank for no filter, type \"back\" to return to menu): ";
        getline(cin, cat);
    }

		if(cat == "back") {
			cout << "Reservation cancelled, returning to menu" << endl;
			if (sysController) sysController->update_usage_log("User cancelled reservation");
			return false;
		}

    if (stat.empty()) {
        cout << "Enter status (leave blank for no filter): ";
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
    
		if (sysController) sysController->update_usage_log("User searched assets");
    return true;
}

//View assets checked out by a specific student
bool Assets::viewStudentAssets(const std::string& studentEmail) {
    cout << "--- View Student Assets ---\n" << endl;

    string email = studentEmail;
    if (email.empty()) {
        cout << "Enter student email(type \"back\" to return to menu): ";
        getline(cin, email);
    }

		if(email == "back") {
			cout << "Action cancelled, returning to menu" << endl;
			if (sysController) sysController->update_usage_log("Action cancelled");
			return false;
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
								if (sysController) sysController->update_usage_log("User viewed student assets");
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
		if (sysController) sysController->update_usage_log("User viewed student assets");
    return true;
}

//view own assets, dont need
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
		if(asset["operationalStatus"] == "out of service"){
			cout << "Out of Service Reason: " << asset["outOfServiceReason"] << endl;
			cout << "Expected Return Date: " << asset["expectedReturnDate"] << endl;
		}
		cout << "Condition: " << asset["condition"] << endl;
		cout << "Location: " << asset["location"] << endl;
		cout << "Clearance Level: " << asset["clearanceLevel"] << endl;
		if (asset["category"] == "consumable") {
			if (asset.contains("stock")) cout << "stock: " << asset["stock"] << endl;
			if (asset.contains("minimumThreshold")) cout << "Low Stock Threshold: " << asset["minimumThreshold"] << endl;
			if (asset.contains("lowStock")) cout << "Low Stock Flag: " << asset["lowStock"] << endl;
		}
		if (asset["category"] == "software") {
			if (asset.contains("renewalDate")) cout << "Renewal Date: " << asset["renewalDate"] << endl;
			if (asset.contains("seatCount")) cout << "Seat Count: " << asset["seatCount"] << endl;
		}
		cout << "Description: " << asset["description"] << endl;
		cout << "-----------------------------------" << endl;
	}
	if (sysController) sysController->update_usage_log("User listed all assets");
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
		cout << "Please enter the asset ID you want to delete (type \"back\" to return to menu): ";
		getline(cin, input);

		if (input == "back") {
			cout << "Remove Operation Cancelled" << endl;
			if (sysController) sysController->update_usage_log("Remove asset cancelled");
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
	if (sysController) sysController->update_usage_log("User deleted an asset");
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

	string name, category, status, accessLevel, condition, location, stock, minimumThreshold, description, renewalDate;
	int minThresholdInt, stockInt, seatCountInt;
	cout << "Creating a new asset." << endl;
	cout << "Enter asset name(type \"back\" to return to menu): ";
	getline(cin, name);

	if(name == "back") {
			cout << "Add Operation Cancelled" << endl;
			if (sysController) sysController->update_usage_log("Add asset cancelled");
			return false;
	}

	cout << "Enter asset category (equipment, consumable, software): ";
	getline(cin, category);
	while (assetTypes.find(category) == assetTypes.end()) {
		cout << "Invalid asset category entered. Please enter a valid asset category from the list:" << endl << "equipment" << endl << "consumable" << endl << "software" << endl;
		getline(cin, category);
	}
	if (category == "consumable") {
		cout << "Because this asset is a consumeable please enter:" << endl;
		cout << "stock: ";
		int stockInt;
		cin >> stockInt;
		cout << "minimum threshold: ";
		cin >> minThresholdInt;
	}
	if (category == "software") {
		cout << "Because this asset is software please enter:" << endl;
		cout << "Seat count:";
		int seatCountInt;
		cin >> seatCountInt;
		// FIX: Ignore/clear the remaining newline character from the buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Enter renewal date (e.g. 2025-12-31): ";
		getline(cin, renewalDate);
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
	if (category == "software") {
		asset["renewalDate"] = renewalDate;
		asset["seatCount"] = seatCountInt;
	}
	if (category == "consumable") {
		asset["stock"] = stockInt;
		asset["minimumThreshold"] = minThresholdInt;
	}
	asset["description"] = description;

	// Add to JSON array
	assetFile.push_back(asset);

	// Save back to file
	ofstream outFile(assetsFile);
	outFile << setw(4) << assetFile << endl;
	outFile.close();
	if (sysController) sysController->update_usage_log("User created an asset");
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
		cout << "Please enter the asset ID you would like to modify (type \"back\" to return to menu): ";
		getline(cin, input);

	if(input == "back") {
			cout << "Update Operation Cancelled" << endl;
			if (sysController) sysController->update_usage_log("Update asset cancelled");
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
	// Build a fixed list of keys to iterate so newly-added fields (like OOS metadata)
	// aren't processed in this same update loop, which would cause duplicate prompts.
	std::vector<std::string> keys;
	for (auto it = assetToUpdate->begin(); it != assetToUpdate->end(); ++it) {
		keys.push_back(it.key());
	}

	for (const auto& key : keys) {
		if (key == "id") continue;
		if (key == "category") continue;

		json value = (*assetToUpdate)[key];
		string input;
		cout << key << " (" << value << "): ";
		getline(cin, input);

		if (input.empty()) {
			continue;
		}

		// Validate asset status if editing
		if (key == "operationalStatus") {
			while (assetStatus.find(input) == assetStatus.end()) {
				cout << "Invalid asset status entered. Please enter a valid asset status from the list:" << endl << "available" << endl << "reserved" << endl << "out of service" << endl;
				getline(cin, input);
			}
		}

		// Validate clearance level if editing
		if (key == "clearanceLevel") {
			while (clearanceLevels.find(input) == clearanceLevels.end()) {
				cout << "Invalid access level entered. Please enter a valid access level from the list:\t\t1, 2, or 3" << endl;
				getline(cin, input);
			}
		}

		// If editing stock or minimumThreshold, store as integer
		if (key == "stock" || key == "minimumThreshold" || key == "seatCount") {  //things that should be ints
			while (true) {
				try {
					int v = stoi(input);
					(*assetToUpdate)[key] = v;
					break;
				} catch (...) {
					cout << "Invalid integer for " << key << ". Please enter a numeric value: ";
					getline(cin, input);
					if (input.empty()) {
						cout << "Keeping existing value for " << key << "." << endl;
						break;
					}
				}
			}
		} else {
			// Update JSON value for other fields (store as string)
			(*assetToUpdate)[key] = input;
		}

		// If operationalStatus was changed, ask for expected return date and reason
		if (key == "operationalStatus") {
			if (input == "out of service") {
				string expectedReturn;
				string reason;
				// Prompt until non-empty expected return date
				while (true) {
					cout << "Enter expected return date (e.g. 2025-12-31): ";
					getline(cin, expectedReturn);
					if (!expectedReturn.empty()) break;
					cout << "Expected return date is required when setting status to out o	f service." << endl;
				}
				// Prompt until non-empty reason
				while (true) {
					cout << "Enter reason for out of service: ";
					getline(cin, reason);
					if (!reason.empty()) break;
					cout << "A reason is required when setting status to out of service." << endl;
				}
				(*assetToUpdate)["expectedReturnDate"] = expectedReturn;
				(*assetToUpdate)["outOfServiceReason"] = reason;
			} else {
				// If changing away from out of service, remove OOS metadata if present
				if (assetToUpdate->contains("expectedReturnDate")) assetToUpdate->erase("expectedReturnDate");
				if (assetToUpdate->contains("outOfServiceReason")) assetToUpdate->erase("outOfServiceReason");
			}
		}
	}

	// Save updated JSON back to file
	ofstream outFile(assetsFile);
	outFile << assets.dump(4);
	outFile.close();

	cout << "\nAsset updated successfully!\n";
	if (sysController) sysController->update_usage_log("User updated an asset");
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
    cout << "Enter Asset ID to return(type \"0\" to return to menu): ";
    cin >> assetID;
    cin.ignore();

		if(assetID == 0) {
			cout << "Return asset cancelled" << endl;
			if (sysController) sysController->update_usage_log("Return asset cancelled");
			return false;
		}

		string assetName, status, endDate;
    bool foundReservation = false;
    
    for (const auto& res : (*userAccount)["reservations"]) {
			if (res["assetID"].get<int>() == assetID) {
				assetName = res["assetName"].get<string>();
				status = res["status"].get<string>();
				endDate = res["endDate"].get<string>();
				foundReservation = true;
				break;
			}
    }

		if (!foundReservation) {
			cout << "Error: You don't have this asset reserved" << endl;
			return false;
    }

    if (status == "overdue") {
        if (sysController) sysController->update_usage_log("OVERDUE RETURN: User " + email + 
                                       " returned Asset ID " + to_string(assetID) + 
                                       " (" + assetName + ")" +
                                       " - Was due: " + endDate);
        cout << "\nReturned an overdue asset\n" << endl;
    }

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
		if (sysController) sysController->update_usage_log("User returned an asset");
    return true;
}

// Helper to parse int from json field that may be string or number
static int parse_int_field(const json &j, const std::string &key1, const std::string &key2, int defaultVal) {
	if (j.contains(key1)) {
		if (j[key1].is_number()) return j[key1].get<int>();
		if (j[key1].is_string()) {
			try { return std::stoi(j[key1].get<std::string>()); } catch(...) { return defaultVal; }
		}
	}
	if (!key2.empty() && j.contains(key2)) {
		if (j[key2].is_number()) return j[key2].get<int>();
		if (j[key2].is_string()) {
			try { return std::stoi(j[key2].get<std::string>()); } catch(...) { return defaultVal; }
		}
	}
	return defaultVal;
}

bool Assets::decrementConsumable(int assetID, int amount, bool &becameLow) {
	becameLow = false;
	std::string assetsFile = "../../data/assets.json";
	json assets;
	std::ifstream inFile(assetsFile);
	if (!inFile.is_open()) return false;
	try { inFile >> assets; } catch(...) { inFile.close(); return false; }
	inFile.close();

	bool found = false;
	for (auto& asset : assets) {
		if (asset.contains("id") && asset["id"].get<int>() == assetID) {
			found = true;
			std::string category = asset.value("category", "");
			if (category != "consumable") return false;

			int q = parse_int_field(asset,"stock", "", 0);
			int thresh = parse_int_field(asset,"minimumThreshold", "", 0);

			q -= amount;
			if (q < 0) q = 0;

			asset["stock"] = q;
			// set lowStock flag
			bool low = (thresh > 0 && q <= thresh);
			asset["lowStock"] = low;
			if (low) becameLow = true;
			// If stock hit zero, mark out of stock; if refilled above zero and status was out of stock, set to available
			if (q == 0) {
				asset["operationalStatus"] = "out of stock";
			} else {
				if (asset.value("operationalStatus", string("")) == "out of stock") {
					asset["operationalStatus"] = "available";
				}
			}
			break;
		}
	}

	if (!found) return false;

	std::ofstream outFile(assetsFile);
	if (!outFile.is_open()) return false;
	outFile << std::setw(4) << assets << std::endl;
	outFile.close();
	if (sysController) sysController->update_usage_log("Consumable amount decrememted");
	return true;
}
bool Assets::incrementSeatCount(int assetID, int amount) {
	std::string assetsFile = "../../data/assets.json";
	json assets;
	std::ifstream inFile(assetsFile);
	if (!inFile.is_open()) return false;
	try { inFile >> assets; } catch(...) { inFile.close(); return false; }
	inFile.close();

	bool found = false;
	for (auto& asset : assets) {
		if (asset.contains("id") && asset["id"].get<int>() == assetID) {
			found = true;
			std::string category = asset.value("category", "");
			if (category != "software") return false;

			int seats = parse_int_field(asset,"seatCount", "", 0);
			seats += amount;
			asset["seatCount"] = seats;
			break;
		}
	}

	if (!found) return false;

	std::ofstream outFile(assetsFile);
	if (!outFile.is_open()) return false;
	outFile << std::setw(4) << assets << std::endl;
	outFile.close();
	if (sysController) sysController->update_usage_log("Seat count incremented");
	return true;
}

bool Assets::setLowStockThreshold(int assetID, int threshold) {
	std::string assetsFile = "../../data/assets.json";
	json assets;
	std::ifstream inFile(assetsFile);
	if (!inFile.is_open()) return false;
	try { inFile >> assets; } catch(...) { inFile.close(); return false; }
	inFile.close();

	bool found = false;
	for (auto& asset : assets) {
		if (asset.contains("id") && asset["id"].get<int>() == assetID) {
			found = true;
			std::string category = asset.value("category", "");
			if (category != "consumable") return false;

			asset["minimumThreshold"] = threshold;

			// recompute lowStock flag
			int q = parse_int_field(asset, "stock", "", 0);
			asset["lowStock"] = (threshold > 0 && q <= threshold);
			break;
		}
	}

	if (!found) return false;

	std::ofstream outFile(assetsFile);
	if (!outFile.is_open()) return false;
	outFile << std::setw(4) << assets << std::endl;
	outFile.close();
	if (sysController) sysController->update_usage_log("Low threshold set");
	return true;
}

bool Assets::adjustSeatUsage(int assetID, int amount, bool &becameFull) {
	becameFull = false;
	std::string assetsFile = "../../data/assets.json";
	json assets;
	std::ifstream inFile(assetsFile);
	if (!inFile.is_open()) return false;
	try { inFile >> assets; } catch(...) { inFile.close(); return false; }
	inFile.close();

	bool found = false;
	for (auto& asset : assets) {
		if (asset.contains("id") && asset["id"].get<int>() == assetID) {
			found = true;
			std::string category = asset.value("category", "");
			if (category != "software") return false;

			int seatCount = parse_int_field(asset, "seatCount", "", 0);
			int inUse = parse_int_field(asset, "seatsInUse", "", 0);

			inUse += amount;
			if (inUse < 0) inUse = 0;
			if (inUse > seatCount) inUse = seatCount;

			asset["seatsInUse"] = inUse;

			// If we've reached capacity, mark as no seats available
			if (seatCount > 0 && inUse >= seatCount) {
				asset["operationalStatus"] = "no seats available";
				becameFull = true;
			} else {
				// if we freed seats and status was no seats available, restore to available
				if (asset.value("operationalStatus", string("")) == "no seats available") {
					asset["operationalStatus"] = "available";
				}
			}
			break;
		}
	}

	if (!found) return false;

	std::ofstream outFile(assetsFile);
	if (!outFile.is_open()) return false;
	outFile << std::setw(4) << assets << std::endl;
	outFile.close();
	if (sysController) sysController->update_usage_log("Seat usage adjusted");
	return true;
}