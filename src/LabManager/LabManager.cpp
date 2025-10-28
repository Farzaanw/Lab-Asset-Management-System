class LabManager {
private:
    int labManagerID;
    std::string privilage_changes_log[];
public:
    LabAssetManager return_lab_asset_manager_info(LabAssetManager obj);
    void grant_lab_asset_manager_rights();
    void revoke_lab_asset_manager_rights();
    std::string view_all_lab_asset_managers();
    void access_reservations();
    void access_user_activity();
    std::string summarize_data();
    std::string generate_reports();
}
