// CS-5: Documents.h
// Auth: Dunlap, Jack and Quinn McNamee
// Supports: UR-102, UR-320, UR-330, UR-430, UR-431, UR-432
// Collaborators:FacultyResearcher[1..* creates -> * documents], ResearchStudent[1..* creates -> * documents], LabManager[1..* creates -> * documents], LabAssetManager[1..* creates -> * documents], Assets[* -> *] 
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <filesystem>
#include "../library/nlohmann/json.hpp"

// Forward declarations to avoid circular includes
class SystemController;
class User;

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

class Documents {
private:

  const string documentsFile = "../../data/documents.json";
  	const string assetsFile = "../../data/assets.json";
	const string documentsFolder = "../../data/documents/";

  	set<string> documentTypes = {
		"manual",
		"warranty",
		"calibration log",
		"maintenance log"
	};

		set<string> clearenceLevels = {
		"1", //base clearance level
		"2"  //higher clearance
	};

public:

 // bool deleteDocument(int documentID); //returns a bool if the document was deleted or not

  bool listDocuments(); //prints document to program

  bool uploadDocument(); //returns bool if edition to document was successful or not


};
