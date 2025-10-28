// CS-7: Reservations.cpp
// Auth: Dunlap, Jack
// Supports: US-101, UR-201, UR-310
// Collaborators: FacultyResearcher[*], ResearchStudent[*], Asset[1], Documents[*]
		
Class Reservations {
Private:
  Int reservationID;
  Std::string description;
  Std::string startDate;
  Std::string endDate;
  Std::string User;
Public:
  Bool permissionApproved;
  Bool confirmed;	

	//Makes a reservation
	Bool makeReservation(permissionApproved, description, startDate, endDate, User) {returns bool if the reservation was successfully created or not}
	//cancels a previously created reservation
	Bool cancelReservation(reservationID) {returns bool is cancelation was successful or not}
	//retrieves a reservation and returns information about it
	Std::string getReservation(reservationID) {returns string of information for that reservationID}
}
