// Kai Johnson
// THIS FILE IS ONLY USED FOR LAB MANAGER AND LAB ASSET MANAGER
#include "Dashboard.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../library/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

/**
 * Display Dashboard (Lab Manager / Lab Asset Manager only)
 *
 * This function loads accounts.json, assets.json, and usage_log.json,
 * then provides a menu of dashboard views:
 *
 *  1. Utilization by Asset (date-range filter)
 *  2. Top Assets (most-used assets overall)
 *  3. Low Stock Summary (for consumables)
 *  4. Reservations per Group (research student or faculty researcher)
 *  5. Overdue Incidents (placeholder)
 *
 * Each dashboard processes JSON data, prints formatted tables,
 * and optionally exports the results to CSV.
 *
 * Returns true on successful execution of a dashboard view.
 * Returns false on file I/O errors, missing data, or invalid menu selection.
 */
bool displayDashboard()
{
	const string accountsFile = "../../data/accounts.json";
	const string assetsFile = "../../data/assets.json";
	const string usageLogFile = "../../data/usage_log.json";
	json usageLogs;
	ifstream inFile(usageLogFile);
	if (!inFile.is_open())
	{
		cerr << "Error: Could not open " << usageLogFile << endl;
		return false;
	}
	inFile >> usageLogs;
	inFile.close();

	json assets;
	ifstream assetIn(assetsFile);
	if (!assetIn.is_open())
	{
		cerr << "Error: Could not open " << assetsFile << endl;
		return false;
	}
	assetIn >> assets;
	assetIn.close();

	json accounts;
	ifstream accountIn(accountsFile);
	if (!accountIn.is_open())
	{
		cerr << "Error: Could not open " << accountsFile << endl;
		return false;
	}
	accountIn >> accounts;
	accountIn.close();

	unordered_map<int, string> assetNames;
	if (!assets.is_array())
	{
		cerr << "Error: assets.json is not an array!" << endl;
		return false;
	}
	for (auto &a : assets)
	{
		if (!a.contains("id") || !a.contains("name"))
			continue;
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

	/**
	 * Utilization by Asset:
	 *
	 * Allows the user to specify a start and end date (YYYY-MM-DD).
	 * The function then scans usage_log.json for entries whose "start" timestamp
	 * falls within that range, counting the number of uses per asset.
	 *
	 * Results are displayed in a formatted table including:
	 *   - Asset ID
	 *   - Asset Name
	 *   - Number of times used
	 *
	 * The user is also prompted to export the table to a CSV file ("utilization.csv").
	 *
	 * Returns true on success (even if no usage was found in that range).
	 * Returns false if usage_log.json is missing or incorrectly formatted.
	 */
	if (choice == "1")
	{
		string startDate, endDate;
		if (!usageLogs.contains("usage") || !usageLogs["usage"].is_array())
		{
			cerr << "There are no usage logs to display." << endl;
			return false;
		}

		cout << "Enter start date (YYYY-MM-DD): ";
		getline(cin, startDate);
		cout << "Enter end date (YYYY-MM-DD): ";
		getline(cin, endDate);
		startDate += " 00:00:00";
		endDate += " 23:59:59";

		unordered_map<int, int> usageCount;
		for (auto &entry : usageLogs["usage"])
		{
			if (!entry.contains("start") || !entry.contains("assetID"))
				continue;
			string start = entry["start"];
			if (start >= startDate && start <= endDate)
			{
				usageCount[entry["assetID"]] += 1;
			}
		}

		cout << "\n===== UTILIZATION BY ASSET =====\n";
		if (usageCount.empty())
		{
			cout << "No usage in this date range.\n";
			return true;
		}

		cout << left << setw(10) << "ID"
			 << setw(40) << "Name"
			 << setw(10) << "Uses" << endl;
		cout << string(60, '-') << endl;

		for (auto &p : usageCount)
		{
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
		if (exportChoice == "y" || exportChoice == "Y")
		{
			ofstream outFile("utilization.csv");
			outFile << "ID,Name,Uses\n";
			for (auto &p : usageCount)
			{
				int id = p.first;
				int uses = p.second;
				string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
				outFile << id << ",\"" << name << "\"," << uses << "\n";
			}
			cout << "Dashboard exported successfully to utilization.csv\n";
		}

		return true;
	}

	/**
	 * Top Assets:
	 *
	 * Tallies all usage_log.json entries by assetID, producing a ranking of assets
	 * based on total usage count over all time.
	 *
	 * Output includes:
	 *   - Asset ID
	 *   - Asset Name
	 *   - Total Uses
	 *
	 * Results are sorted in descending order and displayed in a formatted table.
	 * User can optionally export results to "top_assets.csv".
	 *
	 * Returns true on success, or true if no usage exists.
	 * Returns false on I/O or JSON format errors.
	 */
	else if (choice == "2")
	{
		unordered_map<int, int> usageCount;
		if (!usageLogs.contains("usage") || !usageLogs["usage"].is_array())
		{
			cerr << "There are no usage logs to display." << endl;
			return false;
		}

		for (auto &entry : usageLogs["usage"])
		{
			if (entry.contains("assetID"))
				usageCount[entry["assetID"]]++;
		}

		if (usageCount.empty())
		{
			cout << "No usage recorded.\n";
			return true;
		}

		vector<pair<int, int>> sorted(usageCount.begin(), usageCount.end());
		sort(sorted.begin(), sorted.end(), [](auto &a, auto &b)
			 { return a.second > b.second; });

		cout << "\n===== TOP ASSETS =====\n";
		cout << left << setw(10) << "ID"
			 << setw(40) << "Name"
			 << setw(10) << "Uses" << endl;
		cout << string(60, '-') << endl;

		for (auto &p : sorted)
		{
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
		if (exportChoice == "y" || exportChoice == "Y")
		{
			ofstream outFile("top_assets.csv");
			outFile << "ID,Name,Uses\n";
			for (auto &p : sorted)
			{
				int id = p.first;
				int uses = p.second;
				string name = assetNames.count(id) ? assetNames[id] : "UNKNOWN";
				outFile << id << ",\"" << name << "\"," << uses << "\n";
			}
			cout << "Dashboard exported successfully to top_assets.csv\n";
		}

		return true;
	}

	/**
	 * Low Stock Summary:
	 *
	 * Scans assets.json for assets of category "consumable".
	 * If stock < minimumThreshold, the asset is considered low-stock.
	 *
	 * For each low-stock consumable, the dashboard prints:
	 *   - Asset ID
	 *   - Name
	 *   - Current Stock
	 *   - Minimum Threshold
	 *
	 * If any low-stock items exist, user is prompted to export the results
	 * to "low_stock.csv".
	 *
	 * Returns true on success (even if none are low-stock).
	 * Returns false on JSON errors or missing files.
	 */
	else if (choice == "3")
	{
		cout << "\n===== LOW STOCK DASHBOARD =====\n";
		cout << left
			 << setw(5) << "ID"
			 << setw(25) << "Name"
			 << setw(10) << "Stock"
			 << setw(12) << "Threshold"
			 << "\n";
		cout << "-----------------------------------------------------\n";

		bool anyLowStock = false;
		vector<json> lowStockAssets;

		for (const auto &asset : assets)
		{
			if (asset.contains("category") && asset["category"] == "consumable")
			{
				if (asset.contains("stock") && asset.contains("minimumThreshold"))
				{
					int stock = asset["stock"];
					int threshold = asset["minimumThreshold"];
					if (stock < threshold)
					{
						anyLowStock = true;
						lowStockAssets.push_back(asset);

						cout << left
							 << setw(5) << asset["id"].get<int>()
							 << setw(25) << asset["name"].get<string>()
							 << setw(10) << stock
							 << setw(12) << threshold << endl;
					}
				}
			}
		}

		if (!anyLowStock)
		{
			cout << "All consumable assets are sufficiently stocked.\n";
		}

		cout << "===============================================\n";

		// CSV export
		if (anyLowStock)
		{
			string exportChoice;
			cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
			getline(cin, exportChoice);
			if (exportChoice == "y" || exportChoice == "Y")
			{
				ofstream outFile("low_stock.csv");
				outFile << "ID,Name,Stock,Threshold\n";
				for (auto &asset : lowStockAssets)
				{
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

	/**
	 * Reservations per Group:
	 *
	 * Prompts the user to choose a group:
	 *    - "research student"
	 *    - "faculty researcher"
	 *
	 * Displays all reservations associated with accounts of that role.
	 * Output includes:
	 *   - Email
	 *   - Asset ID
	 *   - Asset Name
	 *   - Start Date
	 *   - End Date
	 *   - Status
	 *
	 * All displayed entries can be exported to "reservations.csv".
	 *
	 * Returns true regardless of how many reservations exist.
	 * Returns false only on file read errors or invalid role selection.
	 */
	else if (choice == "4")
	{
		string group;
		cout << "Enter group to display (research student / faculty researcher): ";
		getline(cin, group);
		while (group != "research student" && group != "faculty researcher")
		{
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

		struct ReservationEntry
		{
			string email;
			int assetID;
			string assetName;
			string startDate;
			string endDate;
			string status;
		};
		vector<ReservationEntry> reservationsToExport;

		for (const auto &account : accounts)
		{
			if (account.contains("role") && account["role"] == group)
			{
				if (account.contains("reservations") && !account["reservations"].empty())
				{
					string email = account["email"];
					for (const auto &res : account["reservations"])
					{
						cout << left
							 << setw(30) << email
							 << setw(10) << res["assetID"].get<int>()
							 << setw(30) << res["assetName"].get<string>()
							 << setw(15) << res["startDate"].get<string>()
							 << setw(15) << res["endDate"].get<string>()
							 << setw(10) << res["status"].get<string>() << "\n";

						reservationsToExport.push_back({email,
														res["assetID"].get<int>(),
														res["assetName"].get<string>(),
														res["startDate"].get<string>(),
														res["endDate"].get<string>(),
														res["status"].get<string>()});
					}
				}
			}
		}

		if (!reservationsToExport.empty())
		{
			string exportChoice;
			cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
			getline(cin, exportChoice);
			if (exportChoice == "y" || exportChoice == "Y")
			{
				ofstream outFile("reservations.csv");
				outFile << "Email,Asset ID,Asset Name,Start Date,End Date,Status\n";
				for (const auto &entry : reservationsToExport)
				{
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

	/**
	 * Overdue Incidents:
	 *
	 * Scans accounts.json for reservations whose status == "overdue".
	 * Displays a table with:
	 *   - Email
	 *   - Asset ID
	 *   - Asset Name
	 *   - Due (end date)
	 *   - Days Overdue (computed as floor((now - endDate)/86400))
	 *
	 * Gracefully parses endDate stored as "YYYY-MM-DD HH:MM:SS" or "YYYY-MM-DD".
	 * Prompts to export the result set to "overdue_incidents.csv".
	 *
	 * Returns true whether or not any incidents exist (false only on parse/I/O issues encountered earlier).
	 */
	else if (choice == "5")
	{
		struct OverdueRow
		{
			std::string email;
			int assetID;
			std::string assetName;
			std::string endDate;
			long long daysOverdue;
		};
		std::vector<OverdueRow> rows;

		// Current time
		std::time_t now = std::time(nullptr);

		auto parseToTimeT = [](const std::string &s) -> std::time_t
		{
			// try "YYYY-MM-DD HH:MM:SS"
			std::tm tm{};
			std::istringstream ss1(s);
			ss1 >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
			if (!ss1.fail())
				return std::mktime(&tm);
			// try "YYYY-MM-DD"
			std::tm tm2{};
			std::istringstream ss2(s);
			ss2 >> std::get_time(&tm2, "%Y-%m-%d");
			if (!ss2.fail())
				return std::mktime(&tm2);
			return static_cast<std::time_t>(-1);
		};

		for (const auto &account : accounts)
		{
			if (!account.contains("reservations") || !account["reservations"].is_array())
				continue;
			const std::string email = account.value("email", "");

			for (const auto &res : account["reservations"])
			{
				const std::string status = res.value("status", "");
				if (status != "overdue")
					continue;

				const int assetID = res.value("assetID", 0);
				const std::string assetName = res.value("assetName", "");
				const std::string endDate = res.value("endDate", "");

				std::time_t endT = parseToTimeT(endDate);
				long long days = 0;
				if (endT != -1 && now > endT)
				{
					days = static_cast<long long>((now - endT) / 86400); // floor days overdue
				}

				rows.push_back({email, assetID, assetName, endDate, days});
			}
		}

		std::cout << "\n===== OVERDUE INCIDENTS =====\n";
		if (rows.empty())
		{
			std::cout << "No overdue reservations at this time.\n";
			return true;
		}

		std::cout << std::left
				  << std::setw(30) << "Email"
				  << std::setw(10) << "Asset ID"
				  << std::setw(30) << "Asset Name"
				  << std::setw(20) << "Due"
				  << std::setw(12) << "Days Overdue" << "\n";
		std::cout << std::string(30 + 10 + 30 + 20 + 12, '-') << "\n";

		for (const auto &row : rows)
		{
			std::cout << std::left
					  << std::setw(30) << row.email
					  << std::setw(10) << row.assetID
					  << std::setw(30) << (row.assetName.empty() && assetNames.count(row.assetID) ? assetNames.at(row.assetID) : row.assetName)
					  << std::setw(20) << row.endDate
					  << std::setw(12) << row.daysOverdue
					  << "\n";
		}

		// CSV export
		std::string exportChoice;
		std::cout << "\nWould you like to export this dashboard to CSV? (y/n): ";
		std::getline(std::cin, exportChoice);
		if (exportChoice == "y" || exportChoice == "Y")
		{
			std::ofstream outFile("overdue_incidents.csv");
			outFile << "Email,Asset ID,Asset Name,Due,Days Overdue\n";
			for (const auto &row : rows)
			{
				const std::string nameToWrite =
					(!row.assetName.empty() ? row.assetName : (assetNames.count(row.assetID) ? assetNames.at(row.assetID) : "UNKNOWN"));
				outFile << "\"" << row.email << "\","
						<< row.assetID << ","
						<< "\"" << nameToWrite << "\","
						<< row.endDate << ","
						<< row.daysOverdue << "\n";
			}
			std::cout << "Dashboard exported successfully to overdue_incidents.csv\n";
		}

		return true;
	}
	cout << "Invalid option.\n";
    return false;
}