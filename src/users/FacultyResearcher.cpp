#include <iostream>
#include "FacultyResearcher.h"
using namespace std;

//Destructor
FacultyResearcher::~FacultyResearcher() {
  std::cout << "Destructor Called" << std::endl;
}

//BASIC
//Navigation, displays the dashboard and the user actions
void display_page() {

}
void user_actions() {

}

//ASSETS
//make a reservation for an asset, returns bool - was it successful in making reservation}
//same as use asset, when they reserve it, we are assuming they are using it
bool reserveAsset(int assetID, bool permissions) {
  cout << "--- Reserve Asset ---\n" << endl;
  //Asset Information
}
//Return an asser
bool return_asset(int reservationID) {
  cout << "--- Return Asset ---\n" << endl;
  //Return Information
}
// view a student's list of assets they have checked out
std::vector<Assets> viewStudentAssets(int studentID) {
  cout << "--- Student Assets ---\n" << endl;
  //Enter studentID
  //List the current assets the have
}

//GROUP SECTION
//view the groups list of assets they have checked out
std::vector<Assets> viewGroupAssets(int labGroupID);
//view group usage records(their usage log)
void viewGroupUsage(int labGroupID);
//display a record of the group the faculty is in charge of and their work they have done
void viewGroup(int LabGroupID);

//SOFTWARE LICENSE
//View the available software licenses
std::vector<Documents> viewAvailableLicenseSeats();
//Request software licenses for self, returns a bool for if its successful or not
bool requestSoftwareLicense(int licenseID, const std::string& startDate, const std::string& endDate);
//Request software licesnes for group
bool requestSoftwareLicenseGroup(int licenseID, int labGroupID, const std::string& startDate, const std::string& endDate);
//view their own licenses
std::vector<Documents> viewLicenses();
//view a groups licenses
std::vector<Documents> viewGroupLicenses(int labGroupID);

//Reservations
//View Group reservations
std::vector<Reservations*> viewGroupReservations(int labGroupID);
//Manage Group reservations
bool manageGroupReservations(int labGroupID, int reservationID /*Add Info needed to manage reservation*/);

//DOCUMENTS
//produce a usage report for resource usage by a group
Documents generateUsageReport(Assets assetUsed, Documents dataRecords);