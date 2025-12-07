#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Notifications {
public:
    Notifications(const json& data); // this notification's JSON object

    std::string get_message() const;
    std::string get_ID() const;

    void accept_reservation() const;
    void deny_reservation() const;

private:
    json data; // ← NOT a "member variable" storing state, just a copy of the JSON object
};
