// CS-5: Documents.h
// Auth: Dunlap, Jack and Quinn McNamee
// Supports: UR-102, UR-320, UR-330, UR-430, UR-431, UR-432
// Collaborators:FacultyResearcher[1..* creates -> * documents], ResearchStudent[1..* creates -> * documents], LabManager[1..* creates -> * documents], LabAssetManager[1..* creates -> * documents], Assets[* -> *] 
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <filesystem>
#include "../library/nlohmann/json.hpp"
#include "../SystemController.cpp"
#include "User.h"
#pragma once

class Documents {
private:
  int documentID;
  int authorID;
  const string documentsFile = "../../data/documents.json";
	const string documentsFolder = "../../data/documents/";
  std::string title;
  std::string description;
  std::string type;
  	set<string> documentTypes = {
		"manual",
		"warranty",
		"calibration log",
		"maintenance log"
	};

public:

 // bool deleteDocument(int documentID); //returns a bool if the document was deleted or not

  bool listDocuments(); //prints document to program

  bool uploadDocument(); //returns bool if edition to document was successful or not


};
