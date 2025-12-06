// CS-7: Assets.h
// Supports: US-101, UR-201, UR-310
// Description: Defines the reservation functions

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


class Reservations {
private:
    // empty
public:

    bool reserveAsset(const std::string& email);

};



