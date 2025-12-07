#pragma once
#include <string>
#include <nlohmann/json.hpp>

// using json = nlohmann::json;

// class Notifications {
// public:
//     Notifications(const json& data); // this notification's JSON object

//     void send_notifications(std::string recipient, json data) const;
//     void view_notifications() const;

//     // void accept_reservation() const;  // NEED TO BE IN LAB MANAGER CLASS
//     // void deny_reservation() const;    // NEED TO BE IN LAB MANAGER CLASS

// private:
//     json data; // ← NOT a "member variable" storing state, just a copy of the JSON object
// };
