#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "../library/nlohmann/json.hpp"

using json = nlohmann::json;

class Notifications {
public:
    Notifications(); 

    // Send notification to specific user (by email) or all users of a role
void send_notifications(std::string recipientEmail, std::string role, json data) const;
    
    // View all notifications for a user
    void view_notifications(std::string personEmail) const;
    
    // Handle approve/reject for restricted reservations
    int handleRestrictedReservationRequests() const;
    
    // Get current timestamp as string
    std::string get_current_time() const;

private:
    // Helper: Load JSON from file
    json load_json(const std::string& filepath) const;
    
    // Helper: Save JSON to file
    bool save_json(const std::string& filepath, const json& data) const;
    
    // Helper: Display a single notification
    void display_notification(const json& notif) const;
    
    // Helper: Handle lab manager reservation approvals
    void handle_lab_manager_approvals(const std::string& managerEmail, 
                                      const json& reserve_requests,
                                      json& accounts) const;
    
    // Helper: Approve a reservation
    void approve_reservation(const std::string& managerEmail,
                            const std::string& requesterEmail,
                            const json& notification,
                            json& accounts) const;
    
    // Helper: Reject a reservation
    void reject_reservation(const std::string& requesterEmail,
                           const json& notification) const;
};
