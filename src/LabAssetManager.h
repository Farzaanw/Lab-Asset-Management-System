// CS-4:LabAssetManager.h
// Auth: Kai
// Supports: UR-400 to UR-431
// Collaborators: <accounts>[1..*], <Assets>[1..*], <document>[0..*], <PI>[1]
#include <string>
#include <vector>
#include <chrono>
#include "Assets.h"
#include "Documents.h"
#pragma once

class LabAssetManager {
private:
    std::vector<Account> accountList;
    std::vector<Assets> assetInventory;
    std::vector<Documents> documentRepository;
    std::chrono::system_clock::time_point lastInventoryCheck;
    std::string assetManagerID;
    bool isAdmin;
    bool permissions;
    

public:
    //constructor
    LabAssetManager(const std::string& id);

    //account Management
    Account createAccount(const std::string& name, const std::string& role, const std::string& email);
    bool updateAccount(const std::string& accountID, const Account& updatedInfo);
    bool deleteAccount(const std::string& accountID);

    //asset Management
    Assets addAsset(const Asset& assetInfo);
    bool updateAsset(const std::string& assetID, const Assets& updatedInfo);
    bool removeAsset(const std::string& assetID);
    bool flagAssetOutOfService(const std::string& assetID);

    //inventory & Documents
    Documents runInventoryCheck();
    Documents uploadDocument(const Documents& document);
};
