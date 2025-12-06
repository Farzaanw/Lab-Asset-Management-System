#include "Reservations.h"

//ASSET MANAGEMENT
//Reserve a single asset
bool Reservations::reserveAsset(const std::string& email) {
    cout << "--- Reserve Asset ---\n" << endl;
    
    // First, show available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

    cout << "Available Assets:\n" << endl;
    bool hasAvailable = false;
    for (const auto& asset : assets) {
        if (asset["operationalStatus"] == "available") {
            cout << "ID: " << asset["id"] << " | Name: " << asset["name"] 
                 << " | Category: " << asset["category"] << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No assets currently available for reservation." << endl;
        return false;
    }

    cout << "\n-----------------------------------\n" << endl;
    
    int assetID;
    string startDate, endDate, reason;
    
    cout << "Please enter the AssetID you would like to reserve: ";
    cin >> assetID;
    cin.ignore();

    //find the right asset
    json* targetAsset = nullptr;
    for (auto& asset : assets) {
        if (asset["id"].get<int>() == assetID) {
            targetAsset = &asset;
            break;
        }
    }

    if (!targetAsset) {
        cout << "Error: Asset ID " << assetID << " not found!" << endl;
        return false;
    }

    //check if its available
    string status = (*targetAsset)["operationalStatus"].get<string>();
    if (status != "available") {
        cout << "Error: Asset is not available!" << endl;
        cout << "Current Status: " << status << endl;
        return false;
    }

    // Get reservation dates
    cout << "Enter Start Date (YYYY-MM-DD): ";
    getline(cin, startDate);
    cout << "Enter End Date (YYYY-MM-DD): ";
    getline(cin, endDate);
    
    // Get asset type/category to determine if approval needed
    string assetCategory = (*targetAsset)["category"].get<string>();
    
    // For certain asset types, may need reason/justification
    if (assetCategory == "high-value" || assetCategory == "restricted") {
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
    }
    
    // Validate dates
    if (startDate.empty() || endDate.empty()) {
        cout << "Error: Invalid date format!" << endl;
        return false;
    }
    
    // Create reservation entry
    json reservation = {
        {"assetID", assetID},
        {"assetName", (*targetAsset)["name"]},
        {"startDate", startDate},
        {"endDate", endDate},
        {"status", "confirmed"},
        {"reason", reason}
    };

    // Update user's reservations in accounts.json
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool userFound = false;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            account["reservations"].push_back(reservation);
            userFound = true;
            break;
        }
    }

    if (!userFound) {
        cout << "Error: User account not found!" << endl;
        return false;
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();
    
    // Update asset status to reserved
    (*targetAsset)["operationalStatus"] = "reserved";
    
    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();
    
    cout << "Asset reserved successfully!" << endl;
    return true;
}