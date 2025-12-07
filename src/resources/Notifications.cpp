#include "Notifications.h"
#include <iostream>
#include <fstream>

// // Private add_notification(notification: string): void
// // Private get_unread_notifications(userID: string, readFlag: bool): vector<string>
// // Private get_read_notifications(userID: string, readFlag: bool): vector<string>
// // Private mark_notifications_read(notificationID: string): void
// // Private clear_notifications(userID: string): void
// // Private search_notifications(notificationID: string): string
// // Private get_notification_metadata(notificationID: string): map<string, string>
// // Private get_urgent_notifications(urgency: bool): vector<string>


// // /* Auth: Cash Limberg
// //  * Description: a class of 1 notification that is inside a user class
// //  */ 

Notifications::Notifications() {}

void Notifications::send_notifications(std::string recipient, json data) const {
    std::cout << "Notification Being Sent to " << recipient << "\n";
    nlohmann::json accounts;
    std::ifstream inFile("../../data/accounts.json");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open accounts.json" << std::endl;
        return;
    }
    inFile >> accounts;
    inFile.close();

    ////// ----- NEED TO IMPLMENET --> 3 CASES OF SENDING NOTIFICATIONS (cash!!) /////
    std::cout << "Cash : need to know the 3 cases of sending notifications and include them here" << std::endl;
    return;
    // 1. if sending to a specific user --> send to that user only



    // 2. if sending to all users of a specific role --> send to all users with that role



    // 3. if sending to lab asset manager --> sends to all of them
    // for (auto& account : accounts) {
    //     if (account["role"] == recipient) {
    //         // Ensure notifications array exists (else create it)
    //         if (!account.contains("notifications")) {
    //             account["notifications"] = json::array();
    //         }

    //         // Generate unique notification ID based on current array size
    //         int nextID = account["notifications"].size() + 1;

    //         // Add to user's notifications
    //         account["notifications"].push_back(notification);
    //     }
    // }

    // Save updated accounts
    std::ofstream outFile("../../data/accounts.json");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not save accounts.json" << std::endl;
        return;
    }
    outFile << std::setw(4) << accounts << std::endl;
    outFile.close();
}

void Notifications::view_notifications() const {
    std::cout << "Notification ID: " << data["notificationID"] << "\n";
    std::cout << "Message: " << data["message"] << "\n";
    std::cout << "Type: " << data["type"] << "\n";
    std::cout << "Timestamp: " << data["timeStamp"] << "\n";
}

// std::string Notifications::get_message() const {
//     return data["message"];
// }

// std::string Notifications::get_ID() const {
//     return data["notificationID"];
// }

// // Mark a reservation as accepted
// void Notifications::accept_reservation() const {
//     std::cout << "Reservation accepted for notification ID "
//               << data["notificationID"] << "\n";
// }

// // Mark a reservation as denied
// void Notifications::deny_reservation() const {
//     std::cout << "Reservation denied for notification ID "
//               << data["notificationID"] << "\n";
// }
