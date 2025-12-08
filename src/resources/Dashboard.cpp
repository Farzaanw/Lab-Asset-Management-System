//Kai Johnson
//THIS FILE IS ONLY USED FOR LAB MANAGER AND LAB ASSET MANAGER
#include "Dashboard.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../library/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

bool displayDashboard() {
    const string accountsFile = "../../data/accounts.json";
    const string assetsFile = "../../data/assets.json";
    const string usageLogFile = "../../data/usage_log.json";
	json usageLogs;
	ifstream inFile(usageLogFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open " << usageLogFile << endl;
		return false;
	}
	inFile >> usageLogs;
	inFile.close();

	json assets;
	ifstream assetIn(assetsFile);
	if (!assetIn.is_open()) {
		cerr << "Error: Could not open " << assetsFile << endl;
		return false;
	}
	assetIn >> assets;
	assetIn.close();

	json accounts;
	ifstream accountIn(accountsFile);
	if (!accountIn.is_open()) {
		cerr << "Error: Could not open " << accountsFile << endl;
		return false;
	}
	accountIn >> accounts;
	accountIn.close();

	unordered_map<int, string> assetNames;
	if (!assets.is_array()) {
		cerr << "Error: assets.json is not an array!" << endl;
		return false;
	}
	for (auto& a : assets) {
		if (!a.contains("id") || !a.contains("name")) continue;
		assetNames[a["id"]] = a["name"];
	}

	cout << "Please select a dashboard view to display:" << endl;
	cout << "1. Utilization by Asset (selectable date range)" << endl;
	cout << "2. Top Assets" << endl;
	cout << "3. Low Stock Summary" << endl;
	cout << "4. Reservations per group" << endl;
	cout << "5. Overdue incidents" << endl;
	cout << "Enter your choice: ";

	string choice;
	getline(cin, choice);

	// ------------------ Utilization by Asset ------------------
	if (choice == "1") {
		string startDate, endDate;
		if (!usageLogs.contains("usage") || !usageLogs["usage"].is_array()) {
			cerr << "There are no usage logs to display." << endl;
			return false;
		}

		cout << "Enter start date (YYYY-MM-DD): ";
		getline(cin, startDate);
		cout << "Enter end date (YYYY-MM-DD): ";
		getline(cin, endDate);
		startDate += " 00:00:00";
		endDate   += " 23:59:59";

		unordered_map<int, int> usageCount;
		for (auto& entry : usageLogs["usage"]) {
			if (!entry.contains("start") || !entry.contains("assetID")) continue;
			string start = entry["start"];
			if (start >= startDate && start <= endDate) {
				usageCount[entry["assetID"]] += 1;
			}
		}

		cout << "\n===== UTILIZATION BY ASSET =====\n";
		if (usageCount.empty()) {
			cout << "No usage in this date range.\n";
			return true;
		}

		cout << left << setw(10) << "ID"
			 << setw(40) << "Name"
			 << setw(10) << "Uses" << endl;
		cout << string(60, '-') << endl;

		for (auto& p : usageCount) {
			int id = p.first;
			int uses = p.second;
			string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
			cout << left << setw(10) << id
				 << setw(40) << name
				 << setw(10) << uses << endl;
		}

		// CSV export
		string exportChoice;
		cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
		getline(cin, exportChoice);
		if (exportChoice == "y" || exportChoice == "Y") {
			ofstream outFile("utilization.csv");
			outFile << "ID,Name,Uses\n";
			for (auto& p : usageCount) {
				int id = p.first;
				int uses = p.second;
				string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
				outFile << id << ",\"" << name << "\"," << uses << "\n";
			}
			cout << "Dashboard exported successfully to utilization.csv\n";
		}

		return true;
	}

	// ------------------ Top Assets ------------------
	else if (choice == "2") {
		unordered_map<int, int> usageCount;
		if (!usageLogs.contains("usage") || !usageLogs["usage"].is_array()) {
			cerr << "There are no usage logs to display." << endl;
			return false;
		}

		for (auto& entry : usageLogs["usage"]) {
			if (entry.contains("assetID")) usageCount[entry["assetID"]]++;
		}

		if (usageCount.empty()) {
			cout << "No usage recorded.\n";
			return true;
		}

		vector<pair<int,int>> sorted(usageCount.begin(), usageCount.end());
		sort(sorted.begin(), sorted.end(), [](auto& a, auto& b){ return a.second > b.second; });

		cout << "\n===== TOP ASSETS =====\n";
		cout << left << setw(10) << "ID"
			 << setw(40) << "Name"
			 << setw(10) << "Uses" << endl;
		cout << string(60, '-') << endl;

		for (auto& p : sorted) {
			int id = p.first;
			int uses = p.second;
			string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
			cout << left << setw(10) << id
				 << setw(40) << name
				 << setw(10) << uses << endl;
		}

		// CSV export
		string exportChoice;
		cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
		getline(cin, exportChoice);
		if (exportChoice == "y" || exportChoice == "Y") {
			ofstream outFile("top_assets.csv");
			outFile << "ID,Name,Uses\n";
			for (auto& p : sorted) {
				int id = p.first;
				int uses = p.second;
				string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
				outFile << id << ",\"" << name << "\"," << uses << "\n";
			}
			cout << "Dashboard exported successfully to top_assets.csv\n";
		}

		return true;
	}

	// ------------------ Low Stock Summary ------------------
	else if (choice == "3") {
		cout << "\n===== LOW STOCK DASHBOARD =====\n";
		cout << left
			 << setw(5)  << "ID"
			 << setw(25) << "Name"
			 << setw(10) << "Stock"
			 << setw(12) << "Threshold"
			 << "\n";
		cout << "-----------------------------------------------------\n";

		bool anyLowStock = false;
		vector<json> lowStockAssets;

		for (const auto& asset : assets) {
			if (asset.contains("category") && asset["category"] == "consumable") {
				if (asset.contains("stock") && asset.contains("minimumThreshold")) {
					int stock = asset["stock"];
					int threshold = asset["minimumThreshold"];
					if (stock < threshold) {
						anyLowStock = true;
						lowStockAssets.push_back(asset);

						cout << left
							 << setw(5)  << asset["id"].get<int>()
							 << setw(25) << asset["name"].get<string>()
							 << setw(10) << stock
							 << setw(12) << threshold << endl;
					}
				}
			}
		}

		if (!anyLowStock) {
			cout << "All consumable assets are sufficiently stocked.\n";
		}

		cout << "===============================================\n";

		// CSV export
		if (anyLowStock) {
			string exportChoice;
			cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
			getline(cin, exportChoice);
			if (exportChoice == "y" || exportChoice == "Y") {
				ofstream outFile("low_stock.csv");
				outFile << "ID,Name,Stock,Threshold\n";
				for (auto& asset : lowStockAssets) {
					outFile << asset["id"].get<int>() << ",\""
							<< asset["name"].get<string>() << "\","
							<< asset["stock"].get<int>() << ","
							<< asset["minimumThreshold"].get<int>() << "\n";
				}
				cout << "Dashboard exported successfully to low_stock.csv\n";
			}
		}

		return true;
	}

	// ------------------ Reservations per Group ------------------
	else if (choice == "4") {
		string group;
		cout << "Enter group to display (research student / faculty researcher): ";
		getline(cin, group);
		while(group != "research student" && group != "faculty researcher") {
			cout << "Invalid group selected.\n";
			cout << "Enter group to display (research student / faculty researcher): ";
			getline(cin, group);
		}

		cout << "\n===== RESERVATION DASHBOARD: " << group << " =====\n";
		cout << left
			 << setw(30) << "Email"
			 << setw(10) << "Asset ID"
			 << setw(30) << "Asset Name"
			 << setw(15) << "Start Date"
			 << setw(15) << "End Date"
			 << setw(10) << "Status" << "\n";
		cout << string(110, '-') << "\n";

		struct ReservationEntry {
			string email;
			int assetID;
			string assetName;
			string startDate;
			string endDate;
			string status;
		};
		vector<ReservationEntry> reservationsToExport;

		for (const auto& account : accounts) {
			if (account.contains("role") && account["role"] == group) {
				if (account.contains("reservations") && !account["reservations"].empty()) {
					string email = account["email"];
					for (const auto& res : account["reservations"]) {
						cout << left
							 << setw(30) << email
							 << setw(10) << res["assetID"].get<int>()
							 << setw(30) << res["assetName"].get<string>()
							 << setw(15) << res["startDate"].get<string>()
							 << setw(15) << res["endDate"].get<string>()
							 << setw(10) << res["status"].get<string>() << "\n";

						reservationsToExport.push_back({
							email,
							res["assetID"].get<int>(),
							res["assetName"].get<string>(),
							res["startDate"].get<string>(),
							res["endDate"].get<string>(),
							res["status"].get<string>()
						});
					}
				}
			}
		}

		if (!reservationsToExport.empty()) {
			string exportChoice;
			cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
			getline(cin, exportChoice);
			if (exportChoice == "y" || exportChoice == "Y") {
				ofstream outFile("reservations.csv");
				outFile << "Email,Asset ID,Asset Name,Start Date,End Date,Status\n";
				for (const auto& entry : reservationsToExport) {
					outFile << "\"" << entry.email << "\","
							<< entry.assetID << ","
							<< "\"" << entry.assetName << "\","
							<< entry.startDate << ","
							<< entry.endDate << ","
							<< entry.status << "\n";
				}
				cout << "Dashboard exported successfully to reservations.csv\n";
			}
		}

		return true;
	}

	// ------------------ Overdue incidents (not implemented) ------------------
	else if (choice == "5") {
		return true;
	}

	else {
		cout << "Invalid option.\n";
		return false;
	}
}
