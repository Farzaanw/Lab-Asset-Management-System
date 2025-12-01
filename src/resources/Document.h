// CS-5: Documents.h
// Auth: Dunlap, Jack
// Supports: UR-102, UR-320, UR-330, UR-430, UR-431, UR-432
// Collaborators:FacultyResearcher[1..* creates -> * documents], ResearchStudent[1..* creates -> * documents], LabManager[1..* creates -> * documents], LabAssetManager[1..* creates -> * documents], Assets[* -> *] 
#include <string>
#include <vector>
#pragma once

class Documents {
private:
  int documentID;
  int authorID;
  std::string title;
  std::string description;
  std::string type;
public:
	//Creates a document
  bool createDocument(std::string title, std::string type, int authorID, int documentID, std::string description); //returns bool if document was successfully created or not
	//Edits a previously created document
  bool editDocument(int documentID, std::string description); //returns bool if edition to document was successful or not
  //Deletes a document
  bool deleteDocument(int documentID); //returns a bool if the document was deleted or not
  //Retrieves a document a prints it
  void getDocument(int documentID);  //prints document to program

  bool uploadDocument(std::string pathToDoc); //returns bool if edition to document was successful or not


};
