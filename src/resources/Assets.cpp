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