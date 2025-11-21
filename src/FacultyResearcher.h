// CS-1: FacultyResearcher.h
// Auth: Dunlap, Jack
// Supports: UR-101, UR-102, UR-103, UR-104
// Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*] 
//test
#include <string>
#include <vector>
#include "Assets.h"
#include "Documents.h"
#pragma once

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
	bool reserveAsset(int assetID, bool permissions); //returns bool - was it successful in making reservation}
	//produce a usage report for resource usage by a group
	Documents generateUsageReport(Assets assetUsed, Documents dataRecords);//returns document structure
	//display a record of the group the faculty is in charge of and their work they have done
	void viewGroup(int LabGroupID);
};
