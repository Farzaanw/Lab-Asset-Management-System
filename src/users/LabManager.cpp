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
		cout << "4. Logout" << endl;
		cout << "Please enter your choice: ";
		string choice;
		getline(cin, choice);
		cout << endl;

		if (choice == "1") {
			listPolicies();
		}
		else if (choice == "2") {
			changePolicies();
		}
		else if (choice == "3") {
			displayDashboard();
		}
		else if (choice == "4") {
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