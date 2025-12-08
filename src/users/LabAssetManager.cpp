#include "LabAssetManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <ctime>
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"
#include "../resources/Documents.h"
#include "../resources/Dashboard.h"
static Assets a;
using namespace std;
using json = nlohmann::json;

LabAssetManager::LabAssetManager(const string& email,
			   SystemController* sys)
	: User(email, sys) ,
	  system(sys) {}


//Destructor
LabAssetManager::~LabAssetManager() {}

//Override getRole
string LabAssetManager::getRole() const {
	return "lab asset manager";
}

void LabAssetManager::main(){
	cout << "\n=============================================" << endl;
	cout << "Welcome " << getEmail() << endl;
	cout << "=============================================\n" << endl;
	while(true){
		cout << endl << "---Lab Asset Manager Main Menu---" << endl; //add greeting of user
		cout << "1. Create Account" << endl;
		cout << "2. Update Account" << endl;
		cout << "3. Delete Account" << endl;
		cout << "4. List Accounts" << endl;
		cout << "5. Add Asset" << endl;
		cout << "6. Update Asset" << endl;
		cout << "7. Remove Asset" << endl;
		cout << "8. List Assets" << endl;
		cout << "9. List Documents" << endl;
		cout << "10. Upload Document" << endl;
		cout << "11. View Logs" << endl;
		cout << "12. Set Consumable Low-Stock Threshold" << endl;
		cout << "13. Search/Filter Assets" << endl;
		cout << "14. Display Dashboard" << endl;
		cout << "15. Cancel Reservation" << endl;
		cout << "16. List Reservations" << endl;
		cout << "17. Logout" << endl;
		
		cout << "Please enter your choice: ";
		string choice;
		getline(cin, choice);
		cout << endl;

		if (choice == "1") {
			if(createAccount()) {
				cout << "Account created successfully." << endl;
				system->update_usage_log("Account created by Lab Asset Manager");
			} else {
				cout << "Failed to create account." << endl;
			}
		}
		else if (choice == "2") {
			if(updateAccount()) {
				cout << "Account updated successfully." << endl;
				system->update_usage_log("Account updated by Lab Asset Manager");
			} else {
				cout << "Failed to update account." << endl;
			}
		}
		else if (choice == "3") {
			if(deleteAccount()) {
				cout << "Account deleted successfully." << endl;
				system->update_usage_log("Account deleted by Lab Asset Manager");
			} else {
				cout << "Failed to delete account." << endl;
			}
		}
		else if (choice == "4") {
			if(listAccounts()) {
				cout << "Accounts listed successfully." << endl;
			} else {
				cout << "Failed to list accounts." << endl;
			}
		}
		else if (choice == "5") {
			if(a.addAsset()) {
				cout << "Asset added successfully." << endl;
				system->update_usage_log("Asset added by Lab Asset Manager");
			} else {
				cout << "Failed to add asset." << endl;
			}
		}
		else if (choice == "6") {
			if(a.updateAsset()) {
				cout << "Asset updated successfully." << endl;
				system->update_usage_log("Asset updated by Lab Asset Manager");
			} else {
				cout << "Failed to update asset." << endl;
			}
		}
		else if (choice == "7") {
			if(a.removeAsset()) {
				cout << "Asset removed successfully." << endl;
				system->update_usage_log("Asset removed by Lab Asset Manager");
			} else {
				cout << "Failed to remove asset." << endl;
			}
		}
		else if (choice == "8") {
			if(a.listAssets()) {
				cout << "Assets listed successfully." << endl;
			} else {
				cout << "Failed to list assets." << endl;
			}
		}
		else if (choice == "9") {
			Documents d;
			d.listDocuments();
		}
		else if (choice == "10") {
			Documents d;
			if(d.uploadDocument()){
				cout << "Document uploaded successfully." << endl;
				system->update_usage_log("Document uploaded by Lab Asset Manager");
			} else {
				cout << "Failed to upload document." << endl;
			}
		}
		else if (choice == "11") {
			viewLogs();
		}
		else if (choice == "12") {
			if (setConsumableThreshold()){
				cout << "Threshold updated." << endl;
				system->update_usage_log("Low-stock threshold updated by Lab Asset Manager");
			} else {
				cout << "Failed to update threshold." << endl;
			}
		}
		else if (choice == "13") {
			if (!a.searchAssets("", "")) {
				cout << "Failed to search/filter assets." << endl;
			}
		}
		else if (choice == "14") {
			if(displayDashboard()) {
				cout << "Dashboard displayed successfully." << endl;
			} else {
				cout << "Failed to display dashboard." << endl;
			}
		}
		else if (choice == "15"){
			if(cancelReservation()) {
				cout << "Reservation cancelled successfully." << endl;
				system->update_usage_log("Reservation cancelled by Lab Asset Manager");
			} else {
				cout << "Failed to cancel reservation." << endl;
			}
		}
		else if (choice == "16") {
			if(listReservations()) {
				cout << "Reservations listed successfully." << endl;
			} else {
				cout << "Failed to list reservations." << endl;
			}
		}
		else if (choice == "17") {
			cout << "Exiting Lab Asset Manager." << endl;
			system->update_usage_log("Lab Asset Manager logged out");
			break;
		}
		else {
			cout << "Invalid choice. Please try again." << endl;
		}
	}
}

bool LabAssetManager::setConsumableThreshold() {
	int assetID;
	cout << "Enter asset ID of consumable to set threshold: ";
	cin >> assetID;
	cin.ignore();

	int threshold;
	cout << "Enter new low-stock threshold (integer): ";
	cin >> threshold;
	cin.ignore();

	bool ok = a.setLowStockThreshold(assetID, threshold);
	if (ok) {
		if (system) system->update_usage_log("Low-stock threshold updated for asset ID " + to_string(assetID));
	}
	return ok;
}


bool LabAssetManager::createAccount() {
	json j;
	json account;
	ifstream inFile(accountsFile);
	if (inFile.is_open()) {
		inFile >> j;
		inFile.close();
	}

	//asks user for account info
	string role, email, password;
	cout << "Creating a new account." << endl;
	cout << "Enter email: ";
	getline(cin, email);
	cout << "Enter role: ";
	getline(cin, role);

	//role validation
	while (validRoles.find(role) == validRoles.end()) {
		cout << "Invalid role entered. Please enter a valid role from the list (research student, faculty researcher, lab manager, lab asset manager):" << endl;
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
	account["email"] = email;
	account["clearanceLevel"] = "1"; // Default clearance level, can be updated later
	account["role"] = role;
	account["password"] = password;
	// Initialize reservations and notifications as empty arrays so each account
	// can hold multiple reservation and notification objects later.
	account["reservations"] = json::array();
	account["notifications"] = json::array();

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
				cout << "Invalid role. Please enter one of the following (research student, faculty researcher, lab manager, lab asset manager): ";
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
	} catch (const exception& e) {
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

bool LabAssetManager::viewLogs() {
	json logs;
	ifstream inFile(usageLogFile);

	if (!inFile.is_open()) {
		cerr << "Error: Could not open " << usageLogFile << endl;
		return false;
	}

	try {
		inFile >> logs;
	} catch (const exception& e) {
		cerr << "Error reading JSON: " << e.what() << endl;
		return false;
	}

	if (!logs.contains("events") || !logs["events"].is_array()) {
		cerr << "Error: JSON does not contain 'events' array.\n";
		return false;
	}

	const auto& events = logs["events"];

	if (events.empty()) {
		cout << "No logs found." << endl;
		return true;
	}

	// We'll display two kinds of entries if present:
	// - system events (key: "events" or a top-level array of event objects)
	// - usage entries (key: "usage") created when reservations are made

	// Helper to find an array by key or top-level
	auto find_array_by_key = [&](const json& j, const string& key) -> const json* {
		if (j.is_object() && j.contains(key) && j[key].is_array()) return &j[key];
		if (j.is_object()) {
			for (auto it = j.begin(); it != j.end(); ++it) {
				if (it.key() == key && it.value().is_array()) return &it.value();
			}
		}
		return nullptr;
	};

	const json* eventsPtr = nullptr;
	// Prefer explicit "events" array
	eventsPtr = find_array_by_key(logs, "events");

	// Print events if found
	if (eventsPtr) {
		cout << "Listing system events:\n" << endl;
		for (const auto& ev : *eventsPtr) {
			string e = ev.contains("event") ? ev["event"].get<string>() : "(no event)";
			string ts = ev.contains("timestamp") ? ev["timestamp"].get<string>() : "(no timestamp)";
			cout << "Event: " << e << endl;
			cout << "Timestamp: " << ts << endl;
		}
	} else {
		cout << "No system events found." << endl;
	}

	// Now display usage logs (reservations)
	const json* usagePtr = find_array_by_key(logs, "usage");
	if (!usagePtr && logs.is_object() && logs.contains("usage") && logs["usage"].is_array()) usagePtr = &logs["usage"];

	if (usagePtr && !usagePtr->empty()) {
		cout << "\nListing usage logs (reservations):\n" << endl;
		for (const auto& u : *usagePtr) {
			string email = u.contains("email") ? u["email"].get<string>() : "(no email)";
			string start = u.contains("start") ? u["start"].get<string>() : "(no start)";
			string end = u.contains("end") ? u["end"].get<string>() : "(no end)";
			string logged = u.contains("loggedAt") ? u["loggedAt"].get<string>() : "(no loggedAt)";
			string assetid = u.contains("assetID") ? to_string(u["assetID"].get<int>()) : "(no assetID)";
			cout << "User Email: " << email << endl;
			cout << "Asset ID: " << assetid << endl;
			cout << "Start: " << start << endl;
			cout << "End: " << end << endl;
			cout << "Logged At: " << logged << endl;
			cout << "-----------------------------------" << endl;
			
		}
			cout << "Press Enter to continue...";
			cin.ignore();
	} else {
		cout << "\nNo usage logs found." << endl;
	}

	return true;
}
bool LabAssetManager::listReservations() {
    json accounts;
    ifstream inFile(accountsFile);

    if (!inFile.is_open()) {
        cerr << "Error: Could not open " << accountsFile << endl;
        return false;
    }

    try {
        inFile >> accounts;
    } catch (exception& e) {
        cerr << "Error reading JSON: " << e.what() << endl;
        return false;
    }
    inFile.close();

    cout << "\n===== ALL RESERVATIONS =====\n";

    bool found = false;

    for (const auto& user : accounts) {
        if (!user.contains("reservations")) continue;

        for (const auto& r : user["reservations"]) {
            found = true;

            cout << "User Email: " << user["email"] << "\n"
                 << "  Asset ID: " << r["assetID"] << "\n"
                 << "  Asset Name: " << r["assetName"] << "\n"
                 << "  Start Date: " << r["startDate"] << "\n"
                 << "  End Date: " << r["endDate"] << "\n"
                 << "  Status: " << r["status"] << "\n"
                 << "------------------------------------\n";
        }
    }

    if (!found) {
        cout << "No reservations found.\n";
    }

    return true;
}

bool LabAssetManager::cancelReservation() {
    int assetID;
    cout << "Enter the Asset ID of the reservation to cancel: ";
    cin >> assetID;
    cin.ignore();

    json accounts;
    ifstream inFile(accountsFile);

    if (!inFile.is_open()) {
        cerr << "Error: Could not open " << accountsFile << endl;
        return false;
    }

    try {
        inFile >> accounts;
    } catch (exception& e) {
        cerr << "Error reading JSON: " << e.what() << endl;
        return false;
    }
    inFile.close();

    bool found = false;
    int deleteCount = 0;

    // Count how many reservations we are about to remove
    for (const auto& user : accounts) {
        if (!user.contains("reservations")) continue;

        for (const auto& r : user["reservations"]) {
            if (r.contains("assetID") && r["assetID"] == assetID) {
                found = true;
                deleteCount++;
            }
        }
    }

    if (!found) {
        cout << "No reservation found with Asset ID " << assetID << ".\n";
        return false;
    }

    // Confirmation prompt
    cout << "Are you sure you want to cancel " << deleteCount 
         << " reservation(s) for Asset ID " << assetID << "? (Y/N): ";

    char choice;
    cin >> choice;
    choice = tolower(choice);

    if (choice != 'y') {
        cout << "Cancellation aborted.\n";
        return false;
    }

	// Remove reservations
    for (auto& user : accounts) {
        if (!user.contains("reservations")) continue;

        auto& reservations = user["reservations"];

        reservations.erase(
            remove_if(reservations.begin(), reservations.end(),
                [&](const json& r) {
                    return r.contains("assetID") && r["assetID"] == assetID;
                }),
            reservations.end()
        );
    }

    // Save updated JSON back to file
    ofstream outFile(accountsFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to " << accountsFile << endl;
        return false;
    }

    outFile << setw(4) << accounts;
    outFile.close();

    cout << "Reservation(s) for Asset ID " << assetID << " canceled successfully.\n";

    return true;
}
