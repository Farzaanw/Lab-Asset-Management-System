// CS-7: Assets.h
// Supports: US-101, UR-201, UR-310
// Description: Defines the reservation functions

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../library/nlohmann/json.hpp"
#include "../users/ResearchStudent.h"
#include <sstream>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <set>
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;


class Reservations {
private:
    // empty
    SystemController* sysController;
public:

    Reservations(SystemController* sc) : sysController(sc) {}

    bool reserveAsset(const std::string& email);

    bool reserveMultipleAssets(const std::string& email);

    bool viewMyReservations(const std::string& email);

    bool cancelReservation(const std::string& email);



};



