#include "Notifications.h"
#include <iostream>
#include <fstream>
#include <iomanip>

// /* Auth: Cash Limberg
//  * Description: a class of 1 notification that is inside a user class
//  */ 

Notifications::Notifications() {}

// Helper: Load JSON from file
json Notifications::load_json(const std::string& filepath) const {
    json data;
    std::ifstream inFile(filepath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open " << filepath << std::endl;
        return json();
    }
    inFile >> data;
    inFile.close();
    return data;
}

// Helper: Save JSON to file
bool Notifications::save_json(const std::string& filepath, const json& data) const {
    std::ofstream outFile(filepath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not save " << filepath << std::endl;
        return false;
    }
    outFile << std::setw(4) << data << std::endl;
    outFile.close();
    return true;
}

// Helper: Display a single notification
void Notifications::display_notification(const json& notif) const {
    if (notif.contains("notificationID") && !notif["notificationID"].is_null())
        std::cout << "Notification ID: " << notif["notificationID"] << "\n";
    if (notif.contains("message") && !notif["message"].is_null())
        std::cout << "Message: " << notif["message"] << "\n";
    if (notif.contains("type") && !notif["type"].is_null())
        std::cout << "Type: " << notif["type"] << "\n";
    if (notif.contains("timeStamp") && !notif["timeStamp"].is_null())
        std::cout << "Timestamp: " << notif["timeStamp"] << "\n";
    std::cout << "------------------------------\n";
}

// Helper to get current time as string
std::string Notifications::get_current_time() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Notifications::send_notifications(std::string recipientEmail, std::string role, json data) const {
    json accounts = load_json("../../data/accounts.json");
    if (accounts.is_null()) return;

    bool sent = false;

    // Case 1: Send to specific user by email
    if (recipientEmail != "") {
        for (auto& account : accounts) {
            if (account["email"] == recipientEmail) {
                if (!account.contains("notifications"))
                    account["notifications"] = json::array();

                int nextID = account["notifications"].size() + 1;
                data["notificationID"] = std::to_string(nextID);
                account["notifications"].push_back(data);
                sent = true;
                break;
            }
        }
    }

    // Case 2: Send to all users of a specific role
    if (!sent && role != "") {
        for (auto& account : accounts) {
            if (account["role"] == role) {
                if (!account.contains("notifications"))
                    account["notifications"] = json::array();

                int nextID = account["notifications"].size() + 1;
                json userNotification = data;
                userNotification["notificationID"] = std::to_string(nextID);
                account["notifications"].push_back(userNotification);
                sent = true;
            }
        }
    }

    if (save_json("../../data/accounts.json", accounts)) {
        std::cout << "✔ Notification successfully sent.\n";
    }
}


void Notifications::view_notifications(std::string personEmail) const {
    json accounts = load_json("../../data/accounts.json");
    if (accounts.is_null()) return;
    
    json reserve_request_notification = json::array();
    std::string role = "";

    // Display all notifications 
    for (auto& account : accounts) {
        if (account.contains("email") && account["email"] == personEmail) {
            if (account.contains("role")) {
                role = account["role"];
            }

            if (!account.contains("notifications") || !account["notifications"].is_array()) {
                std::cout << "No notifications found.\n";
                return;
            }

            for (const auto& notif : account["notifications"]) {
                display_notification(notif);
                
                // Collect reservation request notifications for lab manager
                if (notif.contains("type") && !notif["type"].is_null() && 
                    notif["type"] == "reservation_request" && role == "lab manager") {
                    reserve_request_notification.push_back(notif);
                }
            }
        }
    }

    // Lab Manager: Handle restricted reservation requests
    if (role == "lab manager" && !reserve_request_notification.empty()) {
        handle_lab_manager_approvals(personEmail, reserve_request_notification, accounts);
    }
}

// Helper: Handle lab manager reservation approvals/rejections
void Notifications::handle_lab_manager_approvals(const std::string& managerEmail, 
                                                   const json& reserve_requests,
                                                   json& accounts) const {
    std::cout << "Do you want to review restricted reservation requests? (y/n): ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice != "y" && choice != "Y") return;

    // Display reservation requests
    std::cout << "\n--- Restricted Reservation Requests ---\n";
    for (const auto& notif : reserve_requests) {
        display_notification(notif);
    }

    // Get notification ID to process
    std::cout << "Enter the Notification ID to review: ";
    std::string notifID;
    std::getline(std::cin, notifID);

    // Validate ID
    bool validID = false;
    for (const auto& notif : reserve_requests) {
        if (notif.contains("notificationID") && !notif["notificationID"].is_null() &&
            notif["notificationID"] == notifID) {
            validID = true;
            break;
        }
    }

    if (!validID) {
        std::cout << "Invalid Notification ID. Returning to main menu.\n";
        return;
    }

    // Get requester email
    const auto& selectedNotif = reserve_requests[std::stoi(notifID) - 1];
    std::string requestSenderEmail = "";
    if (selectedNotif.contains("requester") && !selectedNotif["requester"].is_null()) {
        requestSenderEmail = selectedNotif["requester"];
    }

    // Get manager decision
    std::cout << "\n...Processing Notification ID: " << notifID << "...\n";
    int result = handleRestrictedReservationRequests();
    
    if (result == 1) {
        approve_reservation(managerEmail, requestSenderEmail, selectedNotif, accounts);
    } else if (result == -1) {
        reject_reservation(requestSenderEmail, selectedNotif);
    }
}

// Helper: Approve a reservation
void Notifications::approve_reservation(const std::string& managerEmail,
                                        const std::string& requesterEmail,
                                        const json& notification,
                                        json& accounts) const {
    // Send approval notification
    send_notifications(requesterEmail, "", {
        {"message", "Your reservation request has been approved."},
        {"type", "reservation_update"},
        {"timeStamp", get_current_time()}
    });

    // Extract notification details
    int assetID = notification["assetID"];
    std::string startDate = notification["startDate"];
    std::string endDate = notification["endDate"];
    std::string notifID = notification["notificationID"];

    // Update requester's reservation status
    for (auto& account : accounts) {
        if (account.contains("email") && account["email"] == requesterEmail) {
            if (account.contains("reservations") && account["reservations"].is_array()) {
                for (auto& res : account["reservations"]) {
                    if (res["assetID"] == assetID && 
                        res["startDate"] == startDate && 
                        res["endDate"] == endDate) {
                        res["status"] = "reserved";
                        break;
                    }
                }
            }
            break;
        }
    }

    // Remove notification from ALL lab managers' accounts
    for (auto& account : accounts) {
        if (account.contains("role") && account["role"] == "lab manager") {
            if (account.contains("notifications") && account["notifications"].is_array()) {
                auto& notifications = account["notifications"];
                for (size_t i = 0; i < notifications.size(); i++) {
                    if (notifications[i].contains("notificationID") && 
                        notifications[i]["notificationID"] == notifID) {
                        notifications.erase(notifications.begin() + i);
                        break;
                    }
                }
            }
        }
    }

    // Update asset status
    json assets = load_json("../../data/assets.json");
    if (!assets.is_null()) {
        for (auto& asset : assets) {
            if (asset["id"] == assetID) {
                asset["operationalStatus"] = "reserved";
                break;
            }
        }
        save_json("../../data/assets.json", assets);
    }

    // Save updated accounts
    save_json("../../data/accounts.json", accounts);
    std::cout << "Reservation request approved.\n";
}

// Helper: Reject a reservation
void Notifications::reject_reservation(const std::string& requesterEmail,
                                       const json& notification) const {
    // Send rejection notification
    send_notifications(requesterEmail, "", {
        {"message", "Your reservation request has been rejected."},
        {"type", "reservation_update"},
        {"timeStamp", get_current_time()}
    });

    // Extract notification details
    int assetID = notification["assetID"];
    std::string startDate = notification["startDate"];
    std::string endDate = notification["endDate"];

    // Load files once
    json assets = load_json("../../data/assets.json");
    json accounts = load_json("../../data/accounts.json");
    
    if (assets.is_null() || accounts.is_null()) return;

    // Mark asset as available
    for (auto& asset : assets) {
        if (asset["id"] == assetID) {
            asset["operationalStatus"] = "available";
            break;
        }
    }

    // Remove reservation from requester's account
    for (auto& account : accounts) {
        if (account.contains("email") && account["email"] == requesterEmail) {
            if (account.contains("reservations") && account["reservations"].is_array()) {
                auto& reservations = account["reservations"];
                for (size_t i = 0; i < reservations.size(); i++) {
                    if (reservations[i]["assetID"] == assetID && 
                        reservations[i]["startDate"] == startDate && 
                        reservations[i]["endDate"] == endDate) {
                        reservations.erase(reservations.begin() + i);
                        break;
                    }
                }
            }
            break;
        }
    }

    // Remove notification from ALL lab managers' accounts
    std::string notifID = notification["notificationID"];
    for (auto& account : accounts) {
        if (account.contains("role") && account["role"] == "lab manager") {
            if (account.contains("notifications") && account["notifications"].is_array()) {
                auto& notifications = account["notifications"];
                for (size_t i = 0; i < notifications.size(); i++) {
                    if (notifications[i].contains("notificationID") && 
                        notifications[i]["notificationID"] == notifID) {
                        notifications.erase(notifications.begin() + i);
                        break;
                    }
                }
            }
        }
    }

    // Save both files
    save_json("../../data/assets.json", assets);
    save_json("../../data/accounts.json", accounts);
    std::cout << "Reservation request rejected.\n";
}

int Notifications::handleRestrictedReservationRequests() const {
    // process the chosen notification
    std::cout << "Do you want to approve (a), reject (r), or go back to menu (b)?: ";
    std::string action;
    std::getline(std::cin, action);
    if (action == "a" || action == "A") {
        return 1;
    } else if (action == "r" || action == "R") {
        return -1;
    } else {
        std::cout << "Returning to main menu.\n";
    }
    return 0;
}

void Notifications::renewalDateAlert() const {
    json assets = load_json("../../data/assets.json");
    if (assets.is_null()) return;

    // Get current time for comparison
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    for (const auto& asset : assets) {
        // Only check software categories with a renewal date
        if (asset.contains("category") && asset["category"] == "software" && asset.contains("renewalDate")) {
            std::string rDate = asset["renewalDate"];
            
            // Parse renewalDate (Format: YYYY-MM-DD)
            std::tm tm = {};
            std::istringstream ss(rDate);
            ss >> std::get_time(&tm, "%Y-%m-%d");

            if (std::mktime(&tm) < now_c) {
                // Asset is expired, send notification to Lab Asset Manager role
                json alertData = {
                    {"message", "The license for " + asset["name"].get<std::string>() + " has passed its renewal date."},
                    {"type", "renewal_alert"},
                    {"timeStamp", get_current_time()}
                };
                // Targeted to Lab Asset Manager role 
                send_notifications("", "lab asset manager", alertData);
            }
        }
    }
}