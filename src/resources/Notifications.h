#pragma once
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

// // Auth: Cash Limberg
// // Class: Notifications

// using json = nlohmann::json;

// class Notifications {
// public:

//     // Create a new notification and append it to notifications.json
//     static void create_notification(const std::string& recipientID,
//                                     const std::string& message,
//                                     const std::string& type,
//                                     bool urgency,
//                                     const std::map<std::string, std::string>& metaData = {});

//     // Get all notifications from notifications.json
//     static json load_all();

//     // Write entire notifications array back to file
//     static void save_all(const json& data);

//     // Filtering utilities
//     static json get_unread(const std::string& userID);
//     static json get_read(const std::string& userID);
//     static json get_urgent();
//     static json get_for_user(const std::string& userID);

//     // Mark a notification read inside the JSON file
//     static void mark_read(const std::string& notificationID);

// private:
//     static std::string now_timestamp();
//     static std::string generate_id();
// };

