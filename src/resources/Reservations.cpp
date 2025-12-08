#include "Reservations.h"
#include "../users/ResearchStudent.h"
#include "Assets.h"
#include "../SystemController.h"

//ASSETS
//reserve an asset, returns a int
int Reservations::reserveAsset(const std::string& email) {
    cout << "--- Reserve Asset ---\n" << endl;
    
    // Get all available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        sysController->update_usage_log("Error opening json file");
        return -1; 
    }
    inFile >> assets;
    inFile.close();

    // ---------- get all accounts ----------
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        sysController->update_usage_log("Error opening json file");
        return -1;
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
        sysController->update_usage_log("Reservation canceled, no available assets");
        return -1;
    }

    cout << "\n-----------------------------------\n" << endl;
    int assetID;
    string startInput, endInput, reason, startDate, endDate;
    
    cout << "Please enter the AssetID you would like to reserve(type \"0\" to return to menu): ";
    cin >> assetID;
    cin.ignore();

    if(assetID == 0) {
        cout << "Reserve asset cancelled" << endl;
        sysController->update_usage_log("Reserve asset cancelled");
        return -1;
	}

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
        sysController->update_usage_log("Reservation canceled, asset not found");
        return -1;
    }

    //check if its available
    string status = (*targetAsset)["operationalStatus"].get<string>();
    if (status != "available") {
        cout << "Error: Asset is not available!" << endl;
        cout << "Current Status: " << status << endl;
        sysController->update_usage_log("Reservation canceled, asset not availble");
        return -1;
    }
    // New Validation: Stock/Seat Availability Check
    std::string category = (*targetAsset).value("category", std::string(""));

    if (category == "consumable") {
    int stock = 0;
    if ((*targetAsset).contains("stock")) {
        if ((*targetAsset)["stock"].is_number()) stock = (*targetAsset)["stock"].get<int>();
        else if ((*targetAsset)["stock"].is_string()) {
            try { stock = stoi((*targetAsset)["stock"].get<std::string>()); } catch(...) { stock = 0; }
        }
    }
    // STOP if stock is zero
    if (stock <= 0) {
        std::cout << "Error: This consumable is out of stock and cannot be reserved." << std::endl;
        sysController->update_usage_log("Reservation failed: Consumable out of stock");
        return -1;
    }
} 
    else if (category == "software") {
        int seatCount = 0, inUse = 0;
    if ((*targetAsset).contains("seatCount")) {
        if ((*targetAsset)["seatCount"].is_number()) seatCount = (*targetAsset)["seatCount"].get<int>();
        else if ((*targetAsset)["seatCount"].is_string()) {
            try { seatCount = stoi((*targetAsset)["seatCount"].get<std::string>()); } catch(...) { seatCount = 0; }
        }
    }
    if ((*targetAsset).contains("seatsInUse")) {
        if ((*targetAsset)["seatsInUse"].is_number()) inUse = (*targetAsset)["seatsInUse"].get<int>();
        else if ((*targetAsset)["seatsInUse"].is_string()) {
            try { inUse = stoi((*targetAsset)["seatsInUse"].get<std::string>()); } catch(...) { inUse = 0; }
        }
    }
    // STOP if seats are at capacity
    if (seatCount > 0 && inUse >= seatCount) {
        std::cout << "Error: All seats for this software are currently in use." << std::endl;
        sysController->update_usage_log("Reservation failed: Software seats full");
        return -1;
    }
}

    // Get reservation dates
    cout << "Enter Start Date and Time (YYYY-MM-DD HH:MM): ";
    getline(cin, startInput);
    cout << "Enter End Time (YYYY-MM-DD HH:MM): ";
    getline(cin, endInput);
    //This gets the time and date as strings, this is correct format

    tm startTm = {};
    tm endTm = {};
    istringstream startStream(startInput);
    istringstream endStream(endInput);

    startStream >> get_time(&startTm, "%Y-%m-%d %H:%M");
    endStream >> get_time(&endTm, "%Y-%m-%d %H:%M");

    // Validate dates
    if (startStream.fail() || endStream.fail()) {
        cout << "Error: Invalid date format!" << endl;
        sysController->update_usage_log("Reservation canceled, invaild date format");

        return -1;
    }

    //convert to actual time and date
    time_t startTimeT = mktime(&startTm);
    time_t endTimeT = mktime(&endTm);

    if (startTimeT == -1 || endTimeT == -1) {
        cout << "Error: Invalid date/time values!" << endl;
        sysController->update_usage_log("Reservation canceled, invalid date/time");
        return -1;
    }   

    std::ostringstream startOut, endOut;
    startOut << std::put_time(&startTm, "%Y-%m-%d %H:%M:%S");
    endOut << std::put_time(&endTm, "%Y-%m-%d %H:%M:%S");
    startDate = startOut.str();
    endDate = endOut.str();

    // --------- find the user's account info
    json* targetUser = nullptr;
    for (auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            targetUser = &account;
            break;
        }
    }

    // -----------------------------------------

    // MAKING RESERVATION OR SENDING FOR APPROVAL

    // -----------------------------------------
    
    // Get user's clerance level (ADDED HERE)
    string userClearanceLevel = (*targetUser)["clearanceLevel"].get<string>();

    // Get asset clearance level
    string assetClearanceLevel = (*targetAsset)["clearanceLevel"].get<string>();
    
    // USER NOT MEET CLEARANCE LEVEL -> REQUIRES APPROVAL
    if (std::stoi(userClearanceLevel) < std::stoi(assetClearanceLevel)) {
        cout << "...You do not meet the clearance level required to reserve this asset." << endl;
        cout << "Enter reason for reservation: ";
        getline(cin, reason);
        cout << "This asset requires approval. You will be notified once reviewed." << endl;

        // Create reservation entry with "pending" status
        // have asset be incremented from previous asset id
        int newReservationID = 1;
        if (!(*targetUser)["reservations"].empty()) {
            newReservationID = (*targetUser)["reservations"].back()["reservationID"].get<int>() + 1;
        }
        json reservation = {
            {"reservationID", newReservationID},
            {"assetID", assetID},  
            {"assetName", (*targetAsset)["name"]},
            {"startDate", startDate},
            {"endDate", endDate},
            {"status", "pending"},
            {"reason", reason}
        };

        // update the asset's operational status to "pending"
        (*targetAsset)["operationalStatus"] = "pending";
        ofstream outAssetFile("../../data/assets.json");
        outAssetFile << setw(4) << assets << endl;
        outAssetFile.close();
        ////////////////////////////////////////////////////

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
            sysController->update_usage_log("Error: Account not found");
            return -1;
        }

        // Save updated accounts
        ofstream accountsOut("../../data/accounts.json");
        accountsOut << setw(4) << accounts << endl;
        accountsOut.close();
        sysController->update_usage_log("Reservation pending, requires approval");

        // ----- SEND NOTIFICATION TO ALL LAB ASSET MANAGERS -----
        std::cout << "Sending notification to Lab Asset Manager for approval..." << std::endl;
        Notifications notif;  // Create notification object

        // Build notification JSON payload
        json notifData = {
            {"type", "reservation_request"},
            {"message", "Reservation request requires approval for asset: " + 
                        (*targetAsset)["name"].get<string>() +
                        " (ID: " + std::to_string(assetID) + ") by user: " + email},
            {"reason", reason},
            {"timeStamp", startDate},
            {"requester", email},
            {"assetID", assetID},
            {"startDate", startDate},
            {"endDate", endDate}
        };

        // Call existing notification method
        notif.send_notifications("", "lab manager", notifData);   // sends notification to all lab managers
        sysController->update_usage_log("Notification sent to Lab Manager for approval for asset ID: " + std::to_string(assetID) + " by user: " + email);
        std::cout << "Notification sent." << std::endl;
        return 2; // Indicate that approval is needed
    }

    // ------------------------------------------------------------------------------------   
    // ELSE --- APPROVE RESERVATION IMMEDIATELY
    std::cout << "You meet the clearance level required to reserve this asset." << std::endl;
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
        sysController->update_usage_log("Error: Account not found");
        return -1;
    }

    // Save updated accounts
    ofstream accountsOut("../../data/accounts.json");
    accountsOut << setw(4) << accounts << endl;
    accountsOut.close();
    
    // Update asset usage/status depending on type
    Assets a(sysController);
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

   sysController->update_usage_log("Asset reserved by " + email + " " + "Asset ID: " + std::to_string(assetID) + " Start Date: " + startDate + " End Date: " + endDate);

    cout << "Asset reserved successfully!" << endl;
    return 0;
}

//Reserve multiple assets at once
bool Reservations::reserveMultipleAssets(const std::string& email) {
    cout << "--- Reserve Multiple Assets ---\n" << endl;
    
    // Show available assets
    json assets;
    ifstream inFile("../../data/assets.json");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open assets.json" << endl;
        sysController->update_usage_log("Error opening json file");
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
        sysController->update_usage_log("Reservation canceled, no available assets");
        return false;
    }

    cout << "\n-----------------------------------\n" << endl;
    
    cout << "How many assets would you like to reserve(type \"0\" to return to menu)?";
    int numAssets;
    cin >> numAssets;
    cin.ignore();

    if(numAssets == 0) {
        cout << "Reserve assets cancelled" << endl;
        sysController->update_usage_log("Reserve assets cancelled");
        return false;
	}
    
    if (numAssets < 0) {
        cout << "Invalid number of assets." << endl;
        sysController->update_usage_log("Reservation canceled, invalid input");        
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
        sysController->update_usage_log("Error opening json file");
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
            sysController->update_usage_log("Reservation canceled, assetID not found");
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
                Assets a(sysController);
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

    sysController->update_usage_log("Multiple assets reserved");
    cout << "All " << numAssets << "Multiple assets reserved successfully!" << endl;
    return true;
}

//RESERVATION MANAGEMENT
bool Reservations::viewMyReservations(const std::string& email) {
    cout << "--- My Reservations ---\n" << endl;
    
    json accounts;
    ifstream accountsIn("../../data/accounts.json");
    if (!accountsIn.is_open()) {
        cerr << "Error: Could not open accounts.json" << endl;
        sysController->update_usage_log("Error opening json file");
        return false;
    }
    accountsIn >> accounts;
    accountsIn.close();

    bool hasReservations = false;
    for (const auto& account : accounts) {
        if (account["email"].get<string>() == email) {
            if (account["reservations"].empty()) {
                cout << "No active reservations found." << endl;
                sysController->update_usage_log("User viewed their reservations");
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

    sysController->update_usage_log("User viewed their reservations");
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
        sysController->update_usage_log("Error opening json file");
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
        sysController->update_usage_log("User tried to cancel a reservation");
        return false;
    }

    cout << "Your Reservations:\n" << endl;
    int index = 1;
    for (const auto& res : (*userAccount)["reservations"]) {
        cout << index << ". Asset ID: " << res["assetID"] << " | Name: " << res["assetName"] 
             << " | Status: " << res["status"] << endl;
        index++;
    }

    cout << "\nEnter the number of the reservation to cancel(type \"0\" to return to menu): ";
    int choice;
    cin >> choice;
    cin.ignore();

    if(choice == 0) {
        cout << "Cancel asset cancelled" << endl;
        sysController->update_usage_log("Cancel asset cancelled");
        return false;
	}

    if (choice < 1 || choice > (*userAccount)["reservations"].size()) {
        cout << "Invalid selection." << endl;
        sysController->update_usage_log("User tried to cancel a reservation");
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
                Assets a(sysController);
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
    
    sysController->update_usage_log("User canceled reservation");
    cout << "Reservation cancelled successfully!" << endl;
    
    return true;
}