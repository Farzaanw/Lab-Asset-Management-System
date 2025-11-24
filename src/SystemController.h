// SystemController.h

#pragma once
#include <iostream>
#include <vector>
#include "User.h"
// #include "../resources/Asset.h"
// #include "../resources/Reservation.h"
// #include "../resources/Notification.h"

class SystemController {
public:
    SystemController();  
    void run();         

private:
    std::vector<User> users;
    User* currentUser;
    bool isOpen;
};