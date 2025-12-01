// CS-9: Notifications.h
// Auth: Jacob Munly
// Supports: System Alerts, User Notifications
// Collaborators: StudentResearcher[*], FacultyResearcher[*], LabManager[*], LabAssetManager[*], Reservations[1], Assets[1]
#include <string>
#include <vector>
#include <map>
#pragma once

class Notifications {
private:
    std::string notificationID;
    std::string recipientID;
    std::string message;
    std::string type; // notificationType
    bool urgency;
    bool readFlag;
    std::string timeStamp; // zonedDateTimeStamp
    std::map<std::string, std::string> metaData;

public:
    //Adds a notification
    void add_notification(std::string notification);

    //Retrieves unread notifications for a specific user
    std::vector<std::string> get_unread_notifications(std::string userID, bool readFlag);

    //Retrieves read notifications for a specific user
    std::vector<std::string> get_read_notifications(std::string userID, bool readFlag);

    //Marks a specific notification as read
    void mark_notifications_read(std::string notificationID);

    //Clears all notifications for a specific user
    void clear_notifications(std::string userID);

    //Searches for a specific notification
    std::string search_notifications(std::string notificationID);

    //Retrieves metadata for a specific notification
    std::map<std::string, std::string> get_notification_metadata(std::string notificationID);

    //Retrieves urgent notifications
    std::vector<std::string> get_urgent_notifications(bool urgency);
};
