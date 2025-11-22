# CLI Folder

## Purpose
The `cli/` folder contains the **Command-Line Interface (CLI)** entry point for the Lab Asset Management System.  
It provides a text-based interface for users to interact with the system, including logging in, creating accounts, and performing role-specific actions.

## Contents
- **Main.cpp**: The main entry point of the application. Starts the CLI, displays menus, and handles user input.

## Interactions
- **Main.cpp** creates an instance of **System** and calls its methods to handle all business logic, including:
  - User authentication and account management  
  - Accessing and managing assets (Equipment, Documents, Reservations, Notifications)  
  - Logging actions and notifications  

## Flow Diagram
```
Main.cpp (CLI)
      │
      ▼
SystemController
      │
      ├── Users
      │    ├── StudentResearcher.h / .cpp
      │    ├── FacultyResearcher.h / .cpp
      │    ├── LabManager.h / .cpp
      │    └── LabAssetManager.h / .cpp
      │
      └── Assets
           ├── Asset.h / .cpp
           ├── Document.h / .cpp
           ├── Reservation.h / .cpp
           └── Notification.h / .cpp
```
## Notes
- `System` handles the orchestration between the CLI and all user/asset classes.  
- Utility functions like logging or validation can be placed in separate `.h/.cpp` files in a `utils/` folder if needed.  
