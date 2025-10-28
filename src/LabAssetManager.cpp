// CS-4:LabAssetManager.cpp
// Auth: Kai
// Supports: UR-400 to UR-431
// Collaborators: <accounts>[1..*], <Assets>[1..*], <document>[0..*], <PI>[1]
class LabAssetManager {
private:
    std::vector<Account> accountList;
    std::vector<Asset> assetInventory;
    std::vector<Document> documentRepository;
    std::chrono::system_clock::time_point lastInventoryCheck;
    std::string managerID;

public:
    // Constructor
    LabAssetManager(const std::string& id);

    // Account Management
    Account createAccount(const std::string& name, const std::string& role, const std::string& email);
    bool updateAccount(const std::string& accountID, const Account& updatedInfo);
    bool deleteAccount(const std::string& accountID);

    // Asset Management
    Asset addAsset(const Asset& assetInfo);
    bool updateAsset(const std::string& assetID, const Asset& updatedInfo);
    bool removeAsset(const std::string& assetID);
    bool flagAssetOutOfService(const std::string& assetID);

    // Inventory & Documents
    InventoryReport runInventoryCheck();
    Document uploadDocument(const Document& document);
};
