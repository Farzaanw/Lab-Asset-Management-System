#pragma once
#include <string>
#include <iostream>
#include "../library/nlohmann/json.hpp"

using json = nlohmann::json;

class Notifications {
public:
    Notifications(); 

    void send_notifications(std::string recipientEmail, std::string role, json data) const;
    void view_notifications(std::string personEmaeil) const;

private:
    json data; // ← NOT a "member variable" storing state, just a copy of the JSON object
};
