// CS-6: Assets.h
// Auth: McNamee, Quinn
// Supports: UR-302 UR-320, UR-321, UR-331, UR-401, UR-410, UR-411, UR-413, UR-414, UR-415, UR-421, UR-43, UR-432.
//  Reservations[0..*]  Users[0..*] 
#include <string>
#include <vector>

class Assets {
	private:
		int AssetID;
		std::string assetName;
		std::string assetType;
		std::string status;
		std::string condition;
		std::string location;
		int assetAccess;

	public:

		void addAsset(std::string assetName, std::string location);
		void deleteAsset(int AssetID);
		void updateAssetStatus(int AssetID, std::string Updatedstatus);
		void getAssetInfo(int AssetID);
		void listAvailAssets(std::string assetType);
		void updateAssetAccess(int AssetID, int newAccess);
};
