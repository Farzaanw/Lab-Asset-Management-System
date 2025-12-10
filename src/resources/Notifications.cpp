#include "Notifications.h"
#include <iostream>
#include <fstream>
#include <iomanip>

// /* Auth: Cash Limberg
//  * Description: a class of 1 notification that is inside a user class
//  */ 

Notifications::Notifications() {}

/**
 * Save JSON to file:
 *
 * Writes the provided json object to the given filepath with pretty-print
 * formatting (indent=4). Ensures the file can be opened before writing.
 *
 * Returns true on successful save, false if the file cannot be opened or written.
 */
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

/**
 * Save JSON to file:
 *
 * Writes the provided json object to the given filepath with pretty-print
 * formatting (indent=4). Ensures the file can be opened before writing.
 *
 * Returns true on successful save, false if the file cannot be opened or written.
 */
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

/**
 * Display a single notification:
 *
 * Prints a human-readable view of one notification json object. Safely checks
 * for fields (notificationID, message, type, timeStamp, reason) before printing.
 *
 * Returns nothing (prints to std::cout).
 */
void Notifications::display_notification(const json& notif) const {
    std::cout << "------------------------------";
    if (notif.contains("notificationID") && !notif["notificationID"].is_null())
        std::cout << "Notification ID: " << notif["notificationID"] << "\n";
    if (notif.contains("message") && !notif["message"].is_null())
        std::cout << "Message: " << notif["message"] << "\n";
    if (notif.contains("type") && !notif["type"].is_null())
        std::cout << "Type: " << notif["type"] << "\n";
    if (notif.contains("timeStamp") && !notif["timeStamp"].is_null())
        std::cout << "Timestamp: " << notif["timeStamp"] << "\n";
    if (notif.contains("reason") && !notif["reason"].is_null())
        std::cout << "Reason: " << notif["reason"] << "\n";
    std::cout << "------------------------------\n";
}

/**
 * Get current time (string):
 *
 * Produces a timestamp string in the format "YYYY-MM-DD HH:MM:SS" using the
 * local system time. Intended for stamped notifications and audit messages.
 *
 * Returns the formatted timestamp string.
 */
std::string Notifications::get_current_time() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/**
 * Send notifications:
 *
 * Sends a notification to either a specific user (by recipientEmail) or to all
 * users with a given role (e.g., "lab manager", "lab asset manager"). The
 * notification payload is provided in 'data' and a per-user notificationID is assigned.
 *
 * Behavior:
 *  - If recipientEmail is non-empty, sends to that user only.
 *  - Else if role is non-empty, sends the same notification to all users with that role.
 *  - Updates ../../data/accounts.json in place.
 *
 * Returns nothing (prints a success message on save). If accounts cannot be loaded, exits early.
 */
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

/**
 * View notifications for a user:
 *
 * Loads accounts.json, finds the user by email, and prints all notifications
 * using display_notification(). If the viewer is a "lab manager", collects any
 * "reservation_request" notifications and offers an approval workflow.
 *
 * Returns nothing (prints results). Exits early if accounts cannot be loaded or no notifications exist.
 */
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

/**
 * Handle lab manager approvals:
 *
 * Provides an interactive review flow for restricted reservation requests
 * addressed to lab managers. Displays the list of requests, prompts for a
 * Notification ID to process, and delegates to approval/rejection logic.
 *
 * Inputs:
 *  - managerEmail: the lab manager’s email (viewer).
 *  - reserve_requests: array of reservation_request notifications to review.
 *  - accounts: full accounts json (modified in-place on approval).
 *
 * Returns nothing. On approval, updates accounts and assets; on rejection, removes the reservation and notifies the user.
 */
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

/**
 * Approve a reservation:
 *
 * Marks the requester's reservation as "reserved" (matching assetID, startDate, endDate),
 * removes the corresponding notification from all lab managers, updates the asset’s
 * operationalStatus to "reserved", and sends a confirmation notification to the requester.
 *
 * Saves changes to ../../data/accounts.json and ../../data/assets.json.
 *
 * Returns nothing (prints a confirmation message).
 */
void Notifications::approve_reservation(const std::string& managerEmail,
                                        const std::string& requesterEmail,
                                        const json& notification,
                                        json& accounts) const {
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
                        res["status"] = "reserved";            // <--- main update
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

    // Update asset status and get asset name
    std::string assetName = "";
    json assets = load_json("../../data/assets.json");
    if (!assets.is_null()) {
        for (auto& asset : assets) {
            if (asset["id"] == assetID) {
                asset["operationalStatus"] = "reserved";
                assetName = asset["name"];
                break;
            }
        }
        save_json("../../data/assets.json", assets);
    }

    // Save updated accounts
    save_json("../../data/accounts.json", accounts);
    
    // Send notification to the user AFTER all file saves are complete
    send_notifications(requesterEmail, "", {
        {"message", "Your reservation request for " + assetName + " (ID: " + std::to_string(assetID) + ") has been approved."},
        {"type", "reservation_update"},
        {"timeStamp", get_current_time()}
    });
    
    std::cout << "Reservation request approved.\n";
}

/**
 * Reject a reservation:
 *
 * Marks the asset back to "available", removes the matching reservation from the
 * requester's account, deletes the corresponding notification from all lab managers,
 * and sends a rejection notification to the requester including the asset name.
 *
 * Saves changes to ../../data/accounts.json and ../../data/assets.json.
 *
 * Returns nothing (prints a confirmation message).
 */
void Notifications::reject_reservation(const std::string& requesterEmail,
                                       const json& notification) const {
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
    
    // Get asset name for notification
    std::string assetName = "";
    for (const auto& asset : assets) {
        if (asset["id"] == assetID) {
            assetName = asset["name"];
            break;
        }
    }
    
    // send notification to the user that their reservation was rejected
    send_notifications(requesterEmail, "", {
        {"message", "Your reservation request for " + assetName + " (ID: " + std::to_string(assetID) + ") has been rejected."},
        {"type", "reservation_update"},
        {"timeStamp", get_current_time()}
    });
    std::cout << "Reservation request rejected.\n";
}

/**
 * Handle restricted reservation requests (prompt):
 *
 * Prompts the lab manager for an action on the selected notification:
 *  - 'a' / 'A' to approve (returns 1)
 *  - 'r' / 'R' to reject  (returns -1)
 *  - otherwise go back     (returns 0)
 *
 * Returns an integer code: 1 for approve, -1 for reject, 0 for back/other.
 */
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

/**
 * Renewal date alert:
 *
 * Scans assets.json for software assets with a renewalDate (YYYY-MM-DD). If the
 * renewalDate has passed relative to the current system time, sends a "renewal_alert"
 * notification to all users with the role "lab asset manager".
 *
 * Returns nothing. Exits early if assets cannot be loaded.
 */
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