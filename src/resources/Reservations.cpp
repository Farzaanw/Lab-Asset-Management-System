#include "Reservations.h"
#include "ResearchStudent.h"
#include "Assets.h"

//ASSETS
//reserve an asset, returns a bool
bool Reservations::reserveAsset(const std::string& email) {
    cout << "--- Reserve Asset ---\n" << endl;
    
    // Get all available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        return false;
    }
    inFile >> assets;
    inFile.close();

    // ---------- get all accounts ----------
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();
    // --------------------------------------------------

    // First, show available assets
    cout << "Available Assets:\n" << endl;
    bool hasAvailable = false;
    for (const auto& asset : assets) {
        std::string category = asset.value("category", string(""));
        bool available = false;
        std::string status = asset.value("operationalStatus", string(""));
        if (status == "available") available = true;
        if (category == "consumable") {
            int stock = 0;
            if (asset.contains("stock")) {
                if (asset["stock"].is_number()) stock = asset["stock"].get<int>();
                else if (asset["stock"].is_string()) {
                    try { stock = stoi(asset["stock"].get<string>()); } catch(...) { stock = 0; }
                }
            }
            if (stock > 0) available = true;
        }
        if (category == "software") {
            int seatCount = 0, inUse = 0;
            if (asset.contains("seatCount")) {
                if (asset["seatCount"].is_number()) seatCount = asset["seatCount"].get<int>();
                else if (asset["seatCount"].is_string()) {
                    try { seatCount = stoi(asset["seatCount"].get<string>()); } catch(...) { seatCount = 0; }
                }
            }
            if (asset.contains("seatsInUse")) {
                if (asset["seatsInUse"].is_number()) inUse = asset["seatsInUse"].get<int>();
                else if (asset["seatsInUse"].is_string()) {
                    try { inUse = stoi(asset["seatsInUse"].get<string>()); } catch(...) { inUse = 0; }
                }
            }
            if (seatCount == 0 || inUse < seatCount) available = true;
        }
        if (available) {
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
    // Validate dates
    if (startDate.empty() || endDate.empty()) {
        cout << "Error: Invalid date format!" << endl;
        return false;
    }

    // --------- find the user's account info
    json* targetUser = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            targetUser = &account;
            break;
        }
    }
    // -----------------------------------------
    
    // Get user's clerance level (ADDED HERE)
    string userClearanceLevel = (*targetUser)["clearanceLevel"].get<string>();

    // Get asset clearance level
    string assetClearanceLevel = (*targetAsset)["clearanceLevel"].get<string>();
    
    // If user's clerance is lower than asset's, require reason (SEND REQUEST TO LAB MANAGER!!!!!!!!!1)
    if (std::stoi(userClearanceLevel) < std::stoi(assetClearanceLevel)) {
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
        cout << "This asset requires approval. You will be notified once reviewed." << endl;

        // Create reservation entry with "pending" status
        json reservation = {
            {"assetID", assetID},
            {"assetName", (*targetAsset)["name"]},
            {"startDate", startDate},
            {"endDate", endDate},
            {"status", "pending"},
            {"reason", reason}
        };

        // Update user's reservations in accounts.json
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

        // Notify Lab Manager (TODO: implement notification system)
        cout << "NEED TO IMPLMENT ---- Notification sent to Lab Manager for approval." << endl;
        return false; // Indicate that approval is needed
    }
    
    // ELSE --- Create reservation entry
    json reservation = {
        {"assetID", assetID},
        {"assetName", (*targetAsset)["name"]},
        {"startDate", startDate},
        {"endDate", endDate},
        {"status", "approved"},
        {"reason", reason}
    };

    // Update user's reservations (reuse already loaded accounts)
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
    
    // Update asset usage/status depending on type
    Assets a;
    std::string category = (*targetAsset).value("category", string(""));
    if (category == "consumable") {
        bool becameLow = false;
        a.decrementConsumable(assetID, 1, becameLow);
    } else if (category == "software") {
        bool becameFull = false;
        a.adjustSeatUsage(assetID, 1, becameFull);
    } else {
        // equipment or others -> mark as reserved
        (*targetAsset)["operationalStatus"] = "reserved";
        ofstream outAssetFile("../../data/assets.json");
        outAssetFile << setw(4) << assets << endl;
        outAssetFile.close();
    }
    
    // Append usage log entry for this reservation
    if (!ResearchStudent::appendUsageLog(email, assetID, startDate, endDate)) {
        cerr << "Warning: Failed to write usage log entry." << endl;
    }

    cout << "Asset reserved successfully!" << endl;
    return true;
}

//Reserve multiple assets at once
bool Reservations::reserveMultipleAssets(const std::string& email) {
    cout << "--- Reserve Multiple Assets ---\n" << endl;
    
    // Show available assets
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
    
    cout << "How many assets would you like to reserve? ";
    int numAssets;
    cin >> numAssets;
    cin.ignore();
    
    if (numAssets <= 0) {
        cout << "Invalid number of assets." << endl;
        return false;
    }

    string startDate, endDate;
    cout << "Enter Start Date (YYYY-MM-DD) for all assets: ";
    getline(cin, startDate);
    cout << "Enter End Date (YYYY-MM-DD) for all assets: ";
    getline(cin, endDate);

    // Load accounts
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    vector<int> assetIDs;
    for (int i = 0; i < numAssets; i++) {
        int assetID;
        cout << "Enter Asset ID #" << (i + 1) << ": ";
        cin >> assetID;
        cin.ignore();
        
        // Verify asset exists and is available
        bool found = false;
        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID && 
                asset["operationalStatus"] == "available") {
                assetIDs.push_back(assetID);
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Asset ID " << assetID << " not found or unavailable!" << endl;
            return false;
        }
    }

    // Reserve all assets
    for (int assetID : assetIDs) {
        // Find asset
        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID) {
                // Create reservation
                json reservation = {
                    {"assetID", assetID},
                    {"assetName", asset["name"]},
                    {"startDate", startDate},
                    {"endDate", endDate},
                    {"status", "confirmed"},
                    {"reason", ""}
                };

                // Add to user's reservations
                for (auto& account : accounts) {
                    if (account["email"].get<string>() == email) {
                        account["reservations"].push_back(reservation);
                        break;
                    }
                }

                // Update asset usage/status depending on type
                std::string category = asset.value("category", string(""));
                Assets a;
                if (category == "consumable") {
                    bool becameLow = false;
                    a.decrementConsumable(assetID, 1, becameLow);
                } else if (category == "software") {
                    bool becameFull = false;
                    a.adjustSeatUsage(assetID, 1, becameFull);
                } else {
                    asset["operationalStatus"] = "reserved";
                }
                break;
            }
        }
    }

    // Save files
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();

    ofstream outAssetFile("../../data/assets.json");
    outAssetFile << setw(4) << assets << endl;
    outAssetFile.close();

    cout << "All " << numAssets << " assets reserved successfully!" << endl;
    return true;
}

//RESERVATION MANAGEMENT
bool Reservations::viewMyReservations(const std::string& email) {
    cout << "--- My Reservations ---\n" << endl;
    
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool hasReservations = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            if (account["reservations"].empty()) {
                cout << "No active reservations found." << endl;
                return true;
            }

            int index = 1;
            for (const auto& res : account["reservations"]) {
                cout << "Reservation #" << index << endl;
                cout << "Asset ID: " << res["assetID"] << endl;
                cout << "Asset Name: " << res["assetName"] << endl;
                cout << "Start Date: " << res["startDate"] << endl;
                cout << "End Date: " << res["endDate"] << endl;
                cout << "Status: " << res["status"] << endl;
                if (res.contains("reason") && !res["reason"].get<string>().empty()) {
                    cout << "Reason: " << res["reason"] << endl;
                }
                cout << "-----------------------------------" << endl;
                hasReservations = true;
                index++;
            }
            break;
        }
    }

    if (!hasReservations) {
        cout << "No active reservations found." << endl;
    }
    
    return true;
}

//Cancel own reservation
bool Reservations::cancelReservation(const std::string& email) {
    cout << "--- Cancel Reservation ---\n" << endl;
    
    // Load accounts
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    // Find user and list their reservations
    json* userAccount = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            userAccount = &account;
            break;
        }
    }

    if (!userAccount || (*userAccount)["reservations"].empty()) {
        cout << "You have no reservations to cancel." << endl;
        return false;
    }

    cout << "Your Reservations:\n" << endl;
    int index = 1;
    for (const auto& res : (*userAccount)["reservations"]) {
        cout << index << ". Asset ID: " << res["assetID"] << " | Name: " << res["assetName"] 
             << " | Status: " << res["status"] << endl;
        index++;
    }

    cout << "\nEnter the number of the reservation to cancel: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (*userAccount)["reservations"].size()) {
        cout << "Invalid selection." << endl;
        return false;
    }

    // Get the asset ID before removing
    int assetID = (*userAccount)["reservations"][choice - 1]["assetID"].get<int>();

    // Remove reservation
    auto& reservations = (*userAccount)["reservations"];
    reservations.erase(reservations.begin() + (choice - 1));

    // Update asset status back to available
    json assets;
    ifstream assetFile("../../data/assets.json");
    if (assetFile.is_open()) {
        assetFile >> assets;
        assetFile.close();

        for (auto& asset : assets) {
            if (asset["id"].get<int>() == assetID) {
                std::string category = asset.value("category", string(""));
                Assets a;
                if (category == "consumable") {
                    bool dummy = false;
                    // restore one unit back to stock
                    a.decrementConsumable(assetID, -1, dummy);
                } else if (category == "software") {
                    bool becameFull = false;
                    a.adjustSeatUsage(assetID, -1, becameFull);
                } else {
                    asset["operationalStatus"] = "available";
                }
                break;
            }
        }

        ofstream outAssetFile("../../data/assets.json");
        outAssetFile << setw(4) << assets << endl;
        outAssetFile.close();
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();
    
    cout << "Reservation cancelled successfully!" << endl;
    
    return true;
}