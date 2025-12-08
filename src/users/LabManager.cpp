#include "LabManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <ctime>
#include "../SystemController.h"
#include "../library/nlohmann/json.hpp"
#include "../resources/Dashboard.h"
using namespace std;
using json = nlohmann::json;

LabManager::LabManager(const std::string& email,
			   SystemController* sys)
	: User(email, sys) ,
      system(sys) {}

void LabManager::main(){
	cout << "\n=============================================" << endl;
    cout << "Welcome " << getEmail() << endl;
    cout << "=============================================\n" << endl;
	while(true){
		cout << endl << "---Lab Manager Main Menu---" << endl;
		cout << "1. List Policies" << endl;
		cout << "2. Change Policies" << endl;
		cout << "3. Display Dashboard" << endl;
		cout << "4. Change User Privilege" << endl;
		cout << "5. Logout" << endl;
		cout << "Please enter your choice: ";
		string choice;
		getline(cin, choice);
		cout << endl;

		if (choice == "1") {
			if(listPolicies()){
				cout << "Policies listed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " listed policies.");
			} else {
				cout << "Failed to list policies." << endl;
			}
		}
		else if (choice == "2") {
			if(changePolicies()){
				cout << "Policies changed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " changed policies.");
			} else {
				cout << "Failed to change policies." << endl;
			}
		}
		else if (choice == "3") {
			if(displayDashboard()){
				cout << "Dashboard displayed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " displayed the dashboard.");
			} else {
				cout << "Failed to display dashboard." << endl;
			}
		}
		else if (choice == "4") {
			if(changeUserPrivilege()){
				cout << "User privilege changed successfully." << endl;
				// Log updated in changeUserPrivilege() no need to add here
			} else {
				cout << "Failed to change user privilege." << endl;
			}
		}
		else if (choice == "5") {
			cout << "Exiting Lab Manager." << endl;
			break;
		}
		else {
			cout << endl << "Invalid choice. Please try again." << endl;
		}
	}
}

//Destructor
LabManager::~LabManager() {}

//Override getRole
std::string LabManager::getRole() const {
    return "lab manager";
}


int LabManager::getPolicy(string policyType){
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open policies.json" << endl;
		return -1;
	}
	inFile >> policiesJson;
	inFile.close();
	if(policiesJson.contains(policyType)){
		return (policiesJson[policyType].get<int>());
	} else {
		return -1;
	}
}

bool LabManager::changePolicies(){
	//Load current policies from JSON file
	int newDuration;
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open policies.json" << endl;
		return false;
	}
	inFile >> policiesJson;
	inFile.close();

	// Max booking duration (press Enter to keep current)
	cout << "Current max booking duration is " << policiesJson["MAXBOOKINGDURATION"] << " hours." << endl;
	cout << "Enter new max booking duration (hours) (press Enter to keep current): ";
	string input;
	getline(cin, input);
	if (!input.empty()) {
		while (true) {
			try {
				newDuration = stoi(input);
				if (newDuration <= 0) throw std::invalid_argument("non-positive");
				break;
			} catch (...) {
				cout << "Invalid input. Enter a positive integer or press Enter to keep current: ";
				getline(cin, input);
				if (input.empty()) break;
			}
		}
		if (!input.empty()) {
			std::string fixedint = std::to_string(newDuration);
			policiesJson["MAXBOOKINGDURATION"] = fixedint;
			cout << "Max booking duration updated to " << newDuration << " hours." << endl;
		} else {
			cout << "Keeping current max booking duration." << endl;
		}
	} else {
		cout << "Keeping current max booking duration." << endl;
	}

	// Advance booking horizon (press Enter to keep current)
	cout << "Current advance booking horizon is " << policiesJson["ADVANCEBOOKINGHORIZON"] << " days." <<  endl;
	cout << "Enter new advance booking horizon (days) (press Enter to keep current): ";
	string input2;
	getline(cin, input2);
	if (!input2.empty()) {
		int newHorizon;
		while (true) {
			try {
				newHorizon = stoi(input2);
				if (newHorizon <= 0) throw std::invalid_argument("non-positive");
				break;
			} catch (...) {
				cout << "Invalid input. Enter a positive integer or press Enter to keep current: ";
				getline(cin, input2);
				if (input2.empty()) break;
			}
		}
		if (!input2.empty()) {
			std::string fixedint = std::to_string(newHorizon);
			policiesJson["ADVANCEBOOKINGHORIZON"] = fixedint;
			cout << "Advance booking horizon updated to " << newHorizon << " days." << endl;
		} else {
			cout << "Keeping current advance booking horizon." << endl;
		}
	} else {
		cout << "Keeping current advance booking horizon." << endl;
	}

	ofstream outFile(policiesFile);
	outFile << policiesJson.dump(4);
	outFile.close();
	
	return true;
}

bool LabManager::listPolicies(){
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open policies.json" << endl;
		return false;
	}
	inFile >> policiesJson;
	inFile.close();

	cout << "Current Lab Policies:" << endl;
	cout << "Max Booking Duration (hours): " << policiesJson["MAXBOOKINGDURATION"] << endl;
	cout << "Advance Booking Horizon (days): " << policiesJson["ADVANCEBOOKINGHORIZON"] << endl;

	return true;
}

bool LabManager::changeUserPrivilege() {
	int accountId;
    bool found = false;
    cout << "Enter the Account ID you want to modify: ";
    cin >> accountId;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cerr << "Invalid input. Must be a number.\n";
        return false;
    }
    cin.ignore();

    // Load accounts.json
    json accounts;
    ifstream inFile(accountsFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open " << accountsFile << endl;
        return false;
    }

    try {
        inFile >> accounts;
        inFile.close();
    } catch (...) {
        cerr << "Error reading JSON from " << accountsFile << endl;
        return false;
    }

    // Find the account
    for (auto& acct : accounts) {
        if (acct["id"] == accountId) {
            found = true;

            cout << "Current privilege (clearanceLevel): "
                 << (acct["clearanceLevel"].is_string() ? acct["clearanceLevel"].get<string>() : "None")
                 << "\n";

            int newLevel;
            cout << "Enter new privilege level (1, 2, 3): ";
            cin >> newLevel;

			while (cin.fail() || newLevel < 1 || newLevel > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Invalid privilege level. Must be between 1 and 3.\n";
				cout << "Enter new privilege level (1, 2, 3): ";
				cin >> newLevel;
            }
            cin.ignore();

            // Confirm
            char confirm;
            cout << "Are you sure you want to change the privilege for account ID "
                 << accountId << " to level " << newLevel << "? (y/n): ";
            cin >> confirm;
            cin.ignore();

            if (tolower(confirm) != 'y') {
                cout << "Operation canceled.\n";
                return false;
            }

            // Update clearanceLevel
            acct["clearanceLevel"] = to_string(newLevel);

            // Save file
            ofstream outFile(accountsFile);
            if (!outFile.is_open()) {
                cerr << "Error: Could not save to " << accountsFile << endl;
                return false;
            }

            outFile << accounts.dump(4);
            outFile.close();

            cout << "Privilege successfully updated! Please state the reason for this change: ";
            string reason;
            getline(cin, reason);
            system->update_usage_log("Lab Manager " + getEmail() + " changed privilege of account ID " 
									 + to_string(accountId) + " to level " + to_string(newLevel) 
									 + ". Reason: " + reason);
            return true;
        }
    }

    if (!found) {
        cout << "No account with ID " << accountId << " found.\n";
    }
	return true;
}