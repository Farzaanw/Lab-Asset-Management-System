// CS-2: FacultyResearcher.cpp
// Auth: Dunlap, Jack
// Supports: UR-101, UR-102, UR-103, UR-104
// Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*] 

#include <string>
#include <vector>

class Document; //placeholder class so it compiles

class FacultyResearcher {
private:
  int FacultyID;
  int labGroupID;
public:
  std::string Fname;
  std::string Lname;
  std::string email;
  bool permissions;
	
	//make a reservation for an asset
	bool reserveAsset(int assetID, bool permissions) //returns bool - was it successful in making reservation}
	//produce a usage report for resource usage by a group
	Document generateUsageReport(assetUsed, dataRecords) //returns document structure
	//display a record of the group the faculty is in charge of and their work they have done
	void viewGroup(LabGroupID);
};
