#include "Notifications.h"
#include <iostream>

Notifications::Notifications(const json& data) : data(data) {}

std::string Notifications::get_message() const {
    return data["message"];
}

std::string Notifications::get_ID() const {
    return data["notificationID"];
}

// Mark a reservation as accepted
void Notifications::accept_reservation() const {
    std::cout << "Reservation accepted for notification ID "
              << data["notificationID"] << "\n";
}

// Mark a reservation as denied
void Notifications::deny_reservation() const {
    std::cout << "Reservation denied for notification ID "
              << data["notificationID"] << "\n";
}
