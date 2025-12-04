//Assets.h
//Jacob Munly

#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Reservations.h" // Include the existing Reservations class

// Forward declaration for nlohmann::json
namespace nlohmann {
    template <typename T, typename U, typename V> class basic_json;
    using json = basic_json<std::map, std::vector, std::string>;
}

class Assets {
protected:
    int id;
    std::string name;
    std::string category; // e.g., "equipment", "consumable", "software"
    std::string operationalStatus; // e.g., "available", "reserved", "out of service" (FR-02)
    std::string location;
    std::string description;
    int clearanceLevel; // Mechanism to "enforce role-based access" for documents (FR-17)

public:
    // Constructor
    Assets(int id, 
           const std::string& name, 
           const std::string& category, 
           const std::string& status, 
           const std::string& location,
           const std::string& description,
           int level);

    virtual ~Assets() = default;

    // --- Pure Virtual Methods (Must be implemented by derived classes) ---
    virtual void displayDetails() const = 0;
    virtual std::string toJsonString() const = 0; 
    
    // --- Common Getters ---
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    std::string getOperationalStatus() const { return operationalStatus; }
    std::string getLocation() const { return location; }
    std::string getDescription() const { return description; }
    int getClearanceLevel() const { return clearanceLevel; }

    // --- Common Setters / Mutators (FR-03) ---
    void setOperationalStatus(const std::string& status);
    void setLocation(const std::string& loc) { location = loc; }
    void setDescription(const std::string& desc) { description = desc; }

    // --- Reservation Status Check (FR-05, FR-11) ---
    bool isAvailableForReservation() const { 
        return operationalStatus == "available"; 
    }
    
    // --- Utility ---
    virtual void loadFromJson(const nlohmann::json& j);
};

// =================================================================
// 1. Equipment (Default Asset Type)
// =================================================================
class Equipment : public Assets {
private:
    std::string condition; 
    bool outOfService;     // (FR-13)
    std::string oosReason; // (FR-13)
    std::string expectedReturnDate; // (FR-13)

public:
    Equipment(int id, 
              const std::string& name, 
              const std::string& status, 
              const std::string& location, 
              const std::string& description,
              int level,
              const std::string& condition);
    
    // Overridden virtual methods
    void displayDetails() const override;
    std::string toJsonString() const override; 
    void loadFromJson(const nlohmann::json& j) override;

    // Specific Getters
    std::string getCondition() const { return condition; }
    bool isOutOfService() const { return outOfService; }
    std::string getOosReason() const { return oosReason; }
    std::string getExpectedReturnDate() const { return expectedReturnDate; }

    // Specific Mutators (FR-13)
    void setCondition(const std::string& cond) { condition = cond; }
    void setOutOfService(bool oos, const std::string& reason = "", const std::string& returnDate = "");
};

// =================================================================
// 2. Consumable
// =================================================================
class Consumable : public Assets {
private:
    int quantityOnHand;    // (FR-08, FR-14)
    int minimumThreshold;  // (FR-08, FR-14)

public:
    Consumable(int id, 
               const std::string& name, 
               const std::string& status, 
               const std::string& location, 
               const std::string& description,
               int level,
               int quantity, 
               int threshold);

    // Overridden virtual methods
    void displayDetails() const override;
    std::string toJsonString() const override; 
    void loadFromJson(const nlohmann::json& j) override;

    // Specific Getters (FR-08, FR-14)
    int getQuantityOnHand() const { return quantityOnHand; }
    int getMinimumThreshold() const { return minimumThreshold; }
    bool isLowStock() const { return quantityOnHand < minimumThreshold; }

    // Specific Mutators
    void adjustQuantity(int amount); 
    void setMinimumThreshold(int threshold) { minimumThreshold = threshold; }
};

// =================================================================
// 3. Software
// =================================================================
class Software : public Assets {
private:
    int seatCount;             // Total number of licenses/seats (FR-19)
    std::string renewalDate;   // License renewal date (FR-19)

public:
    Software(int id, 
             const std::string& name, 
             const std::string& status, 
             const std::string& location, 
             const std::string& description,
             int level,
             int seats, 
             const std::string& renewal);

    // Overridden virtual methods
    void displayDetails() const override;
    std::string toJsonString() const override; 
    void loadFromJson(const nlohmann::json& j) override;

    // Specific Getters (FR-19)
    int getSeatCount() const { return seatCount; }
    std::string getRenewalDate() const { return renewalDate; }
    
    // Simplified check for software license availability (FR-19)
    bool checkSeatAvailability(const std::string& startTime, const std::string& endTime, int seatsNeeded) const;

    // Specific Mutators
    void setSeatCount(int seats) { seatCount = seats; }
    void setRenewalDate(const std::string& date) { renewalDate = date; }
};

#endif // ASSETS_H
