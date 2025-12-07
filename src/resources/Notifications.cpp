#include "Notifications.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

/* Auth: Cash Limberg
 * Description: a class of 1 notification that is inside a user class
 */ 

static std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

Notifications::Notifications(std::string recipientID,
                             std::string message,
                             std::string type,
                             bool urgency)
    : recipientID(recipientID),
      message(message),
      type(type),
      urgency(urgency),
      readFlag(false),
      timeStamp(current_timestamp())
{
    notificationID =
        std::to_string(std::hash<std::string>{}(
            recipientID + message + timeStamp + std::to_string(urgency)
        ));
}

/* Function: add notification
 * Description: add a notification to a user account
 * 
 */
void Notifications::add_notification(std::string note) {
    metaData["extra_note"] = note;
}

/* Function: mark notification as read
 * Description: mark the notification from unread to read
 *
 */
void Notifications::mark_notifications_read(std::string notificationsID) {
    readFlag = true;
}

/* Function: clear notifications
 * Description: delete the notification
 *
 */
void Notifications::clear_notifications(std::string userID) {
    metaData.clear();
}

/* Function: get notification metadata
 * Description: print the metadata of the current notification
 *
 */
std::map<std::string, std::string> Notifications::get_notification_metadata() const {
    return metaData;
}

// return !readFlag
bool Notifications::is_unread() const {
    return !readFlag;
}

// return readFlag
bool Notifications::is_read() const {
    return readFlag;
}

// return urgency
bool Notifications::is_urgent() const {
    return urgency;
}

// get message
std::string Notifications::get_message() const {
    return message;
}

// Get Timestamp
std::string Notifications::get_timestamp() const {
    return timeStamp;
}

// Get ID
std::string Notifications::get_id() const {
    return notificationsID;
}
