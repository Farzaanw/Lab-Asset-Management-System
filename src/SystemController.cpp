// SystemController.cpp
// Auth: Farzaan Wadiwala

#include <iostream>
#include <vector>
#include "SystemController.h"
// #include "User.h"
// #include "../resources/Asset.h"
// #include "../resources/Reservation.h"
// #include "../resources/Notification.h"

SystemController::SystemController() {
    currentUser = nullptr;
    isOpen = true;
    // vectors like users, assets, reservations are automatically default-initialized
}

// -------------------------- //
// heart of the CLI interface //
// -------------------------- //
void SystemController::run() {
    std::cout << "Welcome to the Lab Asset Management System!" << std::endl;

    // Main loop for the CLI
    int choice = 0;

    while (isOpen) {
        // Display menu options
        std::cout << "1. Create Account\n2. Log In\n3. Exit\n";
        std::cout << "Select (# - ex: 1): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Handle account creation
                std::cout << "account creation!" << std::endl;
                break;
            case 2:
                // Handle login
                std::cout << "login!" << std::endl;
                break;
            case 3:
                isOpen = false;
                std::cout << "Exiting the system. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }

        // continue here ............






        
    }
    
}

  