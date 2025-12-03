#include "LabManager.h"
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

	//Max booking duration
	cout << "Current max booking duration is " << policiesJson["MAXBOOKINGDURATION"] << " hours." << endl;
	cout << "Enter new max booking duration (hours): ";
	cin >> newDuration;
	while(newDuration <= 0){
		cerr << "Error: Invalid duration." << endl;
		cout << "Please enter a max booking duration (hours): ";
		cin >> newDuration;
	}
	policiesJson["MAXBOOKINGDURATION"] = newDuration;
	cout << "Max booking duration updated to " << newDuration << " hours." << endl;

	//Advance booking horizon
	cout << "Current advance booking horizon is " << policiesJson["ADVANCEBOOKINGHORIZON"] << " days." <<  endl;
	cout << "Enter new advance booking horizon (days): ";
	int newHorizon;
	cin >> newHorizon;
	while(newHorizon <= 0){
		cerr << "Error: Invalid horizon." << endl;
		cout << "Please enter a new advance booking horizon (days): ";
		cin >> newHorizon;
	}
	policiesJson["ADVANCEBOOKINGHORIZON"] = newHorizon;

	cout << "Advance booking horizon updated to " << newHorizon << " days." << endl;

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