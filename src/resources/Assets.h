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

public:



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

    bool return_asset(const std::string& email);

};



