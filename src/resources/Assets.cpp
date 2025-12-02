#include "Assets.h"

// ==========================================
// Base Asset Implementation
// ==========================================
Assets::Assets(int id, std::string name, std::string loc, int access) 
    : assetID(id), assetName(name), location(loc), assetAccessLevel(access) {}

void Assets::displayInfo() const {
    std::cout << "[ID: " << assetID << "] " << assetName 
              << " | Loc: " << location 
              << " | Access Lvl: " << assetAccessLevel << "\n";
}

void Assets::addDocument(int id, std::string title) {
    documents.push_back({id, title});
}

std::vector<Documents> Assets::getDocuments() const {
    return documents;
}

// ==========================================
// Equipment Implementation
// ==========================================
Equipment::Equipment(int id, std::string name, std::string loc, int access, std::string cond)
    : Assets(id, name, loc, access), condition(cond), outOfService(false) {}

bool Equipment::isAvailable() const {
    // Available if NOT out of service
    return !outOfService;
}

void Equipment::setOutOfService(bool status, std::string reason) {
    outOfService = status;
    if(status) condition = "Maintenance: " + reason;
    else condition = "Operational";
}

void Equipment::displayInfo() const {
    Assets::displayInfo();
    std::cout << "   Type: Equipment | Status: " << (outOfService ? "OUT OF SERVICE" : "Available") 
              << " | Condition: " << condition << "\n";
}

// ==========================================
// Consumable Implementation
// ==========================================
Consumable::Consumable(int id, std::string name, std::string loc, int access, int qty, int thresh)
    : Assets(id, name, loc, access), stock(qty), minThreshold(thresh) {}

bool Consumable::isAvailable() const {
    // Available if we have at least 1 item
    return stock > 0;
}

void Consumable::reduceStock(int quantity) {
    if (quantity > stock) {
        std::cout << "Error: Not enough stock.\n";
        return;
    }
    stock -= quantity;
    // Note: Per requirement, we do NOT create a reservation here.
}

void Consumable::restock(int quantity) {
    stock += quantity;
}

bool Consumable::isLowStock() const {
    return stock <= minThreshold;
}

void Consumable::displayInfo() const {
    Assets::displayInfo();
    std::cout << "   Type: Consumable | Stock: " << stock 
              << (isLowStock() ? " [LOW STOCK]" : "") << "\n";
}
