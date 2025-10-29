// CS-7: Reservations.h
// Auth: Dunlap, Jack
// Supports: US-101, UR-201, UR-310
// Collaborators: FacultyResearcher[*], ResearchStudent[*], Asset[1], Documents[*]
#include <string>
#include <vector>
	
class Reservations {
private:
  int reservationID;
  std::string description;
  std::string startDate;
  std::string endDate;
  std::string User;
public:
  bool permissionApproved;
  bool confirmed;	

	//Makes a reservation
	bool makeReservation(bool permissionApproved, std::string description, std::string startDate, std::string endDate, std::string User); //returns bool if the reservation was successfully created or not
	//cancels a previously created reservation
	bool cancelReservation(int reservationID); //{returns bool is cancelation was successful or not}
	//retrieves a reservation and returns information about it
	std::string getReservation(int reservationID); //{returns string of information for that reservationID}
};
