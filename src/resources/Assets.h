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
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;


class Assets {
protected:
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




};



