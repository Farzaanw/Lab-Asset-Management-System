// CS-5: Documents.cpp
// Auth: Dunlap, Jack
// Supports: UR-102, UR-320, UR-330, UR-430, UR-431, UR-432
// Collaborators:FacultyResearcher[1..* creates -> * documents], ResearchStudent[1..* creates -> * documents], LabManager[1..* creates -> * documents], LabAssetManager[1..* creates -> * documents], Assets[* -> *] 

Class Documents {
Private:
  Int documentID;
  Std::string title;
  Std::string description;
  int authorID;
  Std::string type;
Public:
	//Creates a document
	bool createDocument(title, type, authorID, documentID, description) {returns bool if document was successfully created or not}
	//Edits a previously created document
  Bool editDocument(documentID, description){ returns bool if edition to document was successful or not}
  //Deletes a document
  Bool deleteDocument(documentID){ returns a bool if the document was deleted or not}
  //Retrieves a document a prints it
  void getDocument(documentID)  {prints document to program}
}

