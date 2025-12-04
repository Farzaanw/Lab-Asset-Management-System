// CS-6: Assets.h
// Auth: Jacob Munly
// Supports: UR-302 to UR-432
// Description: Defines the Asset hierarchy (Inheritance) and the Manager class.

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Documents.h"

// ==========================================
// 1. Parent asset class
// ==========================================
class Assets {
protected:
    int assetID;
    std::string assetName;
    std::string location;
    int assetAccessLevel; // e.g., 1=Student, 2=Manager
    std::vector<Documents> documents;

public:
    Assets(int id, std::string name, std::string loc, int access);
    virtual ~Assets() {}

    // Pure virtual function: Children MUST implement this
    virtual std::string getType() const = 0; 
    virtual bool isAvailable() const = 0;
    virtual void displayInfo() const;

    // Getters/Setters
    int getID() const { return assetID; }
    std::string getName() const { return assetName; }
    
    // Document Management
    void addDocument(int id, std::string title);
    std::vector<Documents> getDocuments() const;
};

// ==========================================
// 2. Child Class: Equipment (e.g., Microscope)
// ==========================================
class Equipment : public Assets {
private:
    bool outOfService; // OOS Flag
    std::string condition; // e.g., "Good", "Needs Calibration"

public:
    Equipment(int id, std::string name, std::string loc, int access, std::string cond);

    // Overrides
    std::string getType() const override { return "Equipment"; }
    bool isAvailable() const override; // Checks OOS status
    void displayInfo() const override;

    // Specific Methods
    void setOutOfService(bool status, std::string reason = "");
};

// ==========================================
// 3. Child Class: Consumable (e.g., Gloves)
// ==========================================
class Consumable : public Assets {
private:
    int stock;
    int minThreshold;

public:
    Consumable(int id, std::string name, std::string loc, int access, int qty, int thresh);

    // Overrides
    std::string getType() const override { return "Consumable"; }
    bool isAvailable() const override; // Checks if stock > 0
    void displayInfo() const override;

    // Specific Methods
    void reduceStock(int quantity); // Decreases stock (does NOT reserve)
    void restock(int quantity);
    bool isLowStock() const;
};
