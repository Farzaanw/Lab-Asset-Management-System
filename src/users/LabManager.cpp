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
#include "../resources/Notifications.h"
using namespace std;
using json = nlohmann::json;

LabManager::LabManager(const std::string &email,
					   SystemController *sys)
	: User(email, sys),
	  system(sys) {}

/**
 * Main interactive loop for the Lab Manager.
 *
 * Renders a CLI menu with capabilities:
 *   1) List current lab policies
 *   2) Change policies (max booking duration, advance booking horizon)
 *   3) Display dashboard views (utilization, top assets, low stock, etc.)
 *   4) View notifications
 *   5) Change a user's privilege (clearance level)
 *   6) Logout
 *
 * Most successful actions write entries to usage_log via SystemController.
 * Returns only when the user chooses Logout.
 *
 * Return void
 */
void LabManager::main()
{
	cout << "\n=============================================" << endl;
	cout << "Welcome " << getEmail() << endl;
	cout << "=============================================" << endl;
	while (true)
	{
		cout << endl
			 << "---Lab Manager Main Menu---" << endl;
		cout << "1. List Policies" << endl;
		cout << "2. Change Policies" << endl;
		cout << "3. Display Dashboard" << endl;
		cout << "4. View Notifications" << endl;
		cout << "5. Change User Privilege" << endl;
		cout << "6. Logout" << endl;
		cout << "Please enter your choice: ";
		string choice;
		getline(cin, choice);
		cout << endl;

		if (choice == "1")
		{
			if (listPolicies())
			{
				cout << "Policies listed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " listed policies.");
			}
			else
			{
				cout << "Failed to list policies." << endl;
			}
		}
		else if (choice == "2")
		{
			if (changePolicies())
			{
				cout << "Policies changed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " changed policies.");
			}
			else
			{
				cout << "Failed to change policies." << endl;
			}
		}
		else if (choice == "3")
		{
			if (displayDashboard())
			{
				cout << "Dashboard displayed successfully." << endl;
				system->update_usage_log("Lab Manager " + getEmail() + " displayed the dashboard.");
			}
			else
			{
				cout << "Failed to display dashboard." << endl;
			}
		}

		else if (choice == "4")
		{
			Notifications n;
			n.view_notifications(getEmail());
			system->update_usage_log("Lab Manager " + getEmail() + " viewed notifications.");
		}
		else if (choice == "5")
		{
			if (changeUserPrivilege())
			{
				cout << "User privilege changed successfully." << endl;
				// Log updated in changeUserPrivilege() no need to add here
			}
			else
			{
				cout << "Failed to change user privilege." << endl;
			}
		}
		else if (choice == "6")
		{
			cout << "Exiting Lab Manager." << endl;
			break;
		}
		else
		{
			cout << endl
				 << "Invalid choice. Please try again." << endl;
		}
	}
}

// Destructor
LabManager::~LabManager() {}

// Override getRole
std::string LabManager::getRole() const
{
	return "lab manager";
}

/**
 * Retrieve a numeric policy value by key.
 *
 * Opens ../../data/policies.json and returns the integer value
 * for the provided key (e.g., "MAXBOOKINGDURATION", "ADVANCEBOOKINGHORIZON").
 *
 * Policy key to read.
 * Returns the integer value if present; -1 on missing key or file I/O error.
 */
int LabManager::getPolicy(string policyType)
{
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open())
	{
		cerr << "Error: Could not open policies.json" << endl;
		return -1;
	}
	inFile >> policiesJson;
	inFile.close();
	if (policiesJson.contains(policyType))
	{
		return (policiesJson[policyType].get<int>());
	}
	else
	{
		return -1;
	}
}

/**
 * Interactively change lab policies.
 *
 * Loads ../../data/policies.json and allows the Lab Manager to update:
 *   - MAXBOOKINGDURATION (hours)
 *   - ADVANCEBOOKINGHORIZON (days)
 * Prompts permit pressing Enter to keep current values. Inputs are validated
 * as positive integers. Writes updated JSON back to disk.
 *
 * Returns true on successful read/write (even if no change was made);
 * false on file I/O or parse error.
 */
bool LabManager::changePolicies()
{
	// Load current policies from JSON file
	int newDuration;
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open())
	{
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
	if (!input.empty())
	{
		while (true)
		{
			try
			{
				newDuration = stoi(input);
				if (newDuration <= 0)
					throw std::invalid_argument("non-positive");
				break;
			}
			catch (...)
			{
				cout << "Invalid input. Enter a positive integer or press Enter to keep current: ";
				getline(cin, input);
				if (input.empty())
					break;
			}
		}
		if (!input.empty())
		{
			std::string fixedint = std::to_string(newDuration);
			policiesJson["MAXBOOKINGDURATION"] = fixedint;
			cout << "Max booking duration updated to " << newDuration << " hours." << endl;
		}
		else
		{
			cout << "Keeping current max booking duration." << endl;
		}
	}
	else
	{
		cout << "Keeping current max booking duration." << endl;
	}

	// Advance booking horizon (press Enter to keep current)
	cout << "Current advance booking horizon is " << policiesJson["ADVANCEBOOKINGHORIZON"] << " days." << endl;
	cout << "Enter new advance booking horizon (days) (press Enter to keep current): ";
	string input2;
	getline(cin, input2);
	if (!input2.empty())
	{
		int newHorizon;
		while (true)
		{
			try
			{
				newHorizon = stoi(input2);
				if (newHorizon <= 0)
					throw std::invalid_argument("non-positive");
				break;
			}
			catch (...)
			{
				cout << "Invalid input. Enter a positive integer or press Enter to keep current: ";
				getline(cin, input2);
				if (input2.empty())
					break;
			}
		}
		if (!input2.empty())
		{
			std::string fixedint = std::to_string(newHorizon);
			policiesJson["ADVANCEBOOKINGHORIZON"] = fixedint;
			cout << "Advance booking horizon updated to " << newHorizon << " days." << endl;
		}
		else
		{
			cout << "Keeping current advance booking horizon." << endl;
		}
	}
	else
	{
		cout << "Keeping current advance booking horizon." << endl;
	}

	ofstream outFile(policiesFile);
	outFile << policiesJson.dump(4);
	outFile.close();

	return true;
}

/**
 * List current lab policies.
 *
 * Reads ../../data/policies.json and prints values for:
 *   - MAXBOOKINGDURATION (hours)
 *   - ADVANCEBOOKINGHORIZON (days)
 *
 * Returns true on success; false on file I/O or parse error.
 */
bool LabManager::listPolicies()
{
	json policiesJson;
	ifstream inFile(policiesFile);
	if (!inFile.is_open())
	{
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

/**
 * Change a user's privilege (clearance level) by account ID.
 *
 * Prompts for the numeric account ID, validates presence, shows current
 * clearance level, and asks for a new level (1..3) with validation and
 * confirmation. Updates ../../data/accounts.json and logs the reason to
 * usage_log via SystemController.
 *
 * Returns true if a privilege change was confirmed and saved; false if
 *         canceled, account not found, invalid input, or file I/O error.
 */
bool LabManager::changeUserPrivilege()
{
	int accountId;
	bool found = false;
	cout << "Enter the Account ID you want to modify: ";
	cin >> accountId;

	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Invalid input. Must be a number.\n";
		return false;
	}
	cin.ignore();

	// Load accounts.json
	json accounts;
	ifstream inFile(accountsFile);
	if (!inFile.is_open())
	{
		cerr << "Error: Could not open " << accountsFile << endl;
		return false;
	}

	try
	{
		inFile >> accounts;
		inFile.close();
	}
	catch (...)
	{
		cerr << "Error reading JSON from " << accountsFile << endl;
		return false;
	}

	// Find the account
	for (auto &acct : accounts)
	{
		if (acct["id"] == accountId)
		{
			found = true;

			cout << "Current privilege (clearanceLevel): "
				 << (acct["clearanceLevel"].is_string() ? acct["clearanceLevel"].get<string>() : "None")
				 << "\n";

			int newLevel;
			cout << "Enter new privilege level (1, 2, 3): ";
			cin >> newLevel;

			while (cin.fail() || newLevel < 1 || newLevel > 3)
			{
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

			if (tolower(confirm) != 'y')
			{
				cout << "Operation canceled.\n";
				return false;
			}

			// Update clearanceLevel
			acct["clearanceLevel"] = to_string(newLevel);

			// Save file
			ofstream outFile(accountsFile);
			if (!outFile.is_open())
			{
				cerr << "Error: Could not save to " << accountsFile << endl;
				return false;
			}

			outFile << accounts.dump(4);
			outFile.close();

			cout << "Privilege successfully updated! Please state the reason for this change: ";
			string reason;
			getline(cin, reason);
			system->update_usage_log("Lab Manager " + getEmail() + " changed privilege of account ID " + to_string(accountId) + " to level " + to_string(newLevel) + ". Reason: " + reason);
			return true;
		}
	}

	if (!found)
	{
		cout << "No account with ID " << accountId << " found.\n";
	}
	return true;
}