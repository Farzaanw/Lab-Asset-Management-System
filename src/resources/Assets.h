#ifndef ASSETS_H
#define ASSETS_H

// CS-6: Assets.h
// Supports: UR-302 to UR-432
// Description: Defines the Asset hierarchy (Inheritance) and the Manager class.


#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../library/nlohmann/json.hpp"
#include <fstream>
#include <filesystem>
#include <set>
class SystemController;
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;


class Assets {
private:
//the level of clearance needed to obtain each asset
	set<string> clearanceLevels = {
		"1", //low clearance level (base clearance)
		"2",  //Medium clearance level
		"3"  //High clearance level
	};

	set<string> assetTypes = {
		"equipment",
		"consumable",
		"software"
	};

	set<string> assetStatus = {
		"available",
		"reserved",
		"out of service"
	};
    // int assetID;
    // std::string assetName;
    // std::string location;
    // int assetAccessLevel; // e.g., 1=Student, 2=Manager
    // std::vector<Documents> documents;

    SystemController* sysController;

public:

    Assets() : sysController(nullptr) {}
    Assets(SystemController* sc) : sysController(sc) {}

    bool viewStudentAssets(const std::string& studentEmail);

        // Search and filter assets by category and status
    bool searchAssets(const std::string& category, const std::string& status);
        
    // View all available assets in the system
    bool viewAvailableAssets();

    // SOFTWARE LICENSE MANAGEMENT
    // View available software license seats
    bool viewAvailableLicenseSeats();
    
        // View own licenses
    bool viewLicenses();
    

      // View group licenses
    bool viewGroupLicenses(int labGroupID);

    bool viewAssets(const std::string& email); // view reserved assets by the student

    bool listAssets();

    bool removeAsset();

    bool updateAsset();

    bool addAsset();

    // Consumable management
    // Decrement the quantity on hand for a consumable asset. Returns:
    //  0 = success, not low
    //  1 = success, reached low-stock (<= threshold)
    // -1 = asset not found
    // -2 = asset not consumable
    bool decrementConsumable(int assetID, int amount, bool &becameLow);

    bool incrementSeatCount(int assetID, int amount);
    // Adjust seats in use for a software license. `amount` may be positive (use seat)
    // or negative (release seat). If operation causes the license to become full,
    // `becameFull` will be set to true. Returns false on error.
    bool adjustSeatUsage(int assetID, int amount, bool &becameFull);

    // Set the low-stock threshold for a consumable asset
    bool setLowStockThreshold(int assetID, int threshold);

    bool return_asset(const std::string& email);

    // FR-17: Attachments
    bool attachDocumentToAsset(int assetID, int documentID);
    bool viewDocumentsPerAsset(int assetID);

};



#endif