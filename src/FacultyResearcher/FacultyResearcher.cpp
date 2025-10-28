// CS-2: FacultyResearcher.cpp
// Auth: Dunlap, Jack
// Supports: UR-101, UR-102, UR-103, UR-104
// Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*] 
Class FacultyResearcher {
Private:
  Int FacultyID;
  Int labGroupID;
Public:
  Std::string Fname;
  Std::string Lname;
  Std::string email;
  Bool permissions;
	
	//make a reservation for an asset
	Bool reserveAsset(int assetID, bool permissions) {returns bool - was it successful in making reservation} 
	//produce a usage report for resource usage by a group
	Document generateUsageReport(assetUsed, dataRecords) {returns document structure}
	//display a record of the group the faculty is in charge of and their work they have done
	Void viewGroup(LabGroupID);
}
