#ifndef FACULTYRESEARCHER_H
#define FACULTYRESEARCHER_H

// CS-1: FacultyResearcher.h
// Auth: Dunlap, Jack
// Supports: UR-101, UR-102, UR-103, UR-104
// Collaborators: ResearchStudent[1..*], Assets[*], Reservation[*], Documents[*] 
//test
#include <string>
#include <vector>
#include "./resources/Assets.h"
#include "./resources/Documents.h"
#include "./resources/Reservations.h"
#include "User.h"
#include "SystemController.h"
#include "./library/nlohmann/json.hpp"
#include <fstream>

class FacultyResearcher : public User{
private:
  int labGroupID;
  std::string Fname;
  std::string Lname;
  std::string email;
  SystemController* system;
	
public:
  //FacultyResearcher(SystemController* sys) : system(sys) {}
  //FacultyResearcher(string email, SystemController* sys) : User(email, sys) {}
  //void showMenu() override;
  //void reserveAsset(int assetID);

  //Constructor
  FacultyResearcher(const std::string& firstName,
                      const std::string& lastName,
                      const std::string& email,
                      SystemController* sys);

  //Destructor
  virtual ~FacultyResearcher();
  
  //main starting point
  void main() override;

  // Override pure virtual function from User
  std::string getRole() const override;

  //Menus
  void assetManagementMenu();
  void labGroupManagementMenu();
  void softwareLicenseMenu();
  void reportsAndDocumentsMenu();
  void reservationsMenu();

  //BASIC
  //Navigation, displays the dashboard and the user actions
  void display_page();
  void user_actions();

  //ASSETS
	//make a reservation for an asset, returns bool - was it successful in making reservation}
  //same as use asset, when they reserve it, we are assuming they are using it
	bool reserveAsset();
  //Return an asser
  bool return_asset();
  //view own assets
  std::vector<Assets*> viewAssets();
  // view a student's list of assets they have checked out
  std::vector<Assets*> viewStudentAssets(const std::string& studentEmail);
  //reserve multiple assets

  // bool reserveMultipleAssets(std::vector<int> assetIDs, const std::string& startDate, const std::string& endDate);
  
  //search and filter assets 
  // std::vector<Assets*> searchAssets(const std::string& category, const std::string& status);
  //view all assets 
  // std::vector<Assets*> viewAvailableAssets();


  //GROUP SECTION
  //view the groups list of assets they have checked out
  std::vector<Assets*> viewGroupAssets(int labGroupID);
  //view group usage records(their usage log)
  void viewGroupUsage(int labGroupID);
  //display a record of the group the faculty is in charge of and their work they have done
	void viewGroup(int LabGroupID);

  //SOFTWARE LICENSE
  //View the available software licenses
  std::vector<Documents*> viewAvailableLicenseSeats();
  //Request software licenses for self, returns a bool for if its successful or not
  bool requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate);
  //Request software licesnes for group
  bool requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate);
  //view their own licenses
  std::vector<Documents*> viewLicenses();
  //view a groups licenses
  std::vector<Documents*> viewGroupLicenses(int labGroupID);

  //Reservations
  //View Group reservations
  std::vector<Reservations*> viewGroupReservations(int labGroupID);
  //Manage Group reservations
  bool cancelGroupReservations(int labGroupID, int reservationID);
  //view own reservations
  // std::vector<Reservations*> viewMyReservations();
  //cancel own reservation
  // bool cancelReservation(int reservationID);
  //make reservation
  // bool makeReservation(int reservationID, int labGroupID);

  //DOCUMENTS
	//produce a usage report for resource usage by a group
	Documents generateUsageReport(Assets* assetUsed, Documents* dataRecords);
};

#endif
