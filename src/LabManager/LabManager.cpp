class LabManager {
private:
    int labManagerID;
    std::string privilage_changes_log[];
public:
	// prints LabAssetManager info
    LabAssetManager return_lab_asset_manager_info(LabAssetManager obj);
	// Grants lab asset manager rights
    void grant_lab_asset_manager_rights();
    // revokes lab asset manager rights
    void revoke_lab_asset_manager_rights();
    // prints all lab asset managers
    std::string view_all_lab_asset_managers();
    // allows access to reservations
    void access_reservations();
    // shows user activity
    void access_user_activity();
    // summarizes data and outputs it
    std::string summarize_data();
    // generates reports and outputs them
    std::string generate_reports();
};
