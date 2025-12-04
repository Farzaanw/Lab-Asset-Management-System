// CS-7: Reservations.h
// Auth: Dunlap, Jack, Rustic
// Supports: US-101, UR-201, UR-310
// Collaborators: FacultyResearcher[*], ResearchStudent[*], Asset[1], Documents[*]
#pragma once
#include <string>

// Reservation lifecycle states
enum ReservationStatus { PENDING = 0, APPROVED = 1, CONFIRMED = 2, CANCELLED = 3 };

class Reservations {
public:
    int reservationID{0};
    int assetID{0};                
    std::string User;               
    std::string description;
    std::string startDate;          
    std::string endDate;            
    bool permissionApproved{false};
    bool confirmed{false};
    bool overdueFlag{false};
    ReservationStatus status{PENDING};

    // Creates a reservation record (pushes into in-memory DB in .cpp)
    bool makeReservation(bool permissionApproved,
                         std::string description,
                         std::string startDate,
                         std::string endDate,
                         std::string User);

    // Lookup & manage by id (static so UIs don’t need an instance)
    static bool cancelReservation(int reservationID);
    static std::string getReservation(int reservationID);

    // Optional persistence so both roles see the same data
    static bool loadFromJson(const std::string& path);  // e.g., "data/reservations.json"
    static bool saveToJson(const std::string& path);

private:
    // Adjusts stock count on confirm/cancel (implemented in .cpp)
    static void update_asset_stock_count(std::string assetID, int quantity);
};
