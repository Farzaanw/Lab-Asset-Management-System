#include "Documents.h"

bool Documents::uploadDocument() {
	string filePath;
	string assetId;
	json assets;
	json newDocument;
	json documentFile = json::array();
	bool assetFound = false;
	// Load Documents file
	ifstream inFileDocuments(documentsFile);
	if (!inFileDocuments.is_open()) {
		cerr << "Error: Could not open " << documentsFile << endl;
		return false;
	}
	try {
		inFileDocuments >> documentFile;
	} catch (const std::exception& e) {
		cerr << "Error reading JSON: " << e.what() << endl;
		inFileDocuments.close();
		return false;
	}
	inFileDocuments.close();
	if (!documentFile.is_array()) {
		if (documentFile.is_null()) {
			documentFile = json::array();
		} else {
			json tmp = documentFile;
			documentFile = json::array();
			documentFile.push_back(tmp);
		}
	}

	// Load Assets file
	ifstream inFileAssets(assetsFile);
	if (!inFileAssets.is_open()) {
		cerr << "Error: Could not open " << assetsFile << endl;
		return false;
	}
	try {
		inFileAssets >> assets;
	} catch (const std::exception& e) {
		cerr << "Error reading JSON: " << e.what() << endl;
		inFileAssets.close();
		return false;
	}
	inFileAssets.close();

	// Ask for file path
	cout << "Enter the full path of the PDF you want to upload: ";
	getline(cin, filePath);

	// File validation
	if (filePath.size() == 0) {
		cout << "No file path entered.\n";
		return false;
	}
	else if (!fs::exists(filePath)) {
		cout << "Error: File not found.\n";
		return false;
	}
	else if (fs::path(filePath).extension() != ".pdf") {
		cout << "Error: File must be a PDF (.pdf).\n";
		return false;
	}

	// Ask for asset ID
	cout << "Enter the asset ID this document belongs to: ";
	getline(cin, assetId);
	while (assetFound == false) {
		if (assetId == "quit") {
			return false;
		}
		try {
			stoi(assetId);
		}
		catch (...) {
			cout << "Invalid input. Please enter a numeric asset ID or type 'quit' to exit: ";
			getline(cin, assetId);
			if (assetId == "quit") {
				return false;
			}
			continue;
		}
		for (auto& asset : assets) {
			if (asset["id"].get<int>() == stoi(assetId)) {
				assetFound = true;
				break;
			}
		}
		if (assetFound == false) {
			cout << "Asset ID not found. Please try again or type 'quit' to exit: ";
			getline(cin, assetId);
			if (assetId == "quit") {
				return false;
			}
		}
	}

	// Destination folder
	fs::path destFolder = documentsFolder + assetId;
	if (!fs::exists(destFolder)) {
		fs::create_directories(destFolder);
	}

	// Destination file path
	fs::path destPath = destFolder / fs::path(filePath).filename();

	try {
		// Copy the file
		fs::copy_file(filePath, destPath, fs::copy_options::overwrite_existing);
		cout << "Document successfully uploaded to: " << destPath << endl;
	}
	catch (const fs::filesystem_error& error) {
		cout << "Failed to copy file: " << error.what() << endl;
		return false;
	}


	string title, type, accessLevel;
	cout << "What type of document is this? (manual, warranty, calibration log, maintenance log): ";
	getline(cin, type);
	while (documentTypes.find(type) == documentTypes.end()) {
		cout << "Invalid document type entered. Please enter a valid document type from the list: " << endl << "manual" << endl << "warranty" << endl << "calibration log" << endl << "maintenance log" << endl;
		getline(cin, type);
	}

	cout << "Enter document security access level (numeric): ";
	getline(cin, accessLevel);
	while (clearenceLevels.find(accessLevel) == clearenceLevels.end()) {
		cout << "Invalid access level entered. Please enter a valid access level from the list: " << endl << "Research student: (1)" << endl << "Faculty researcher: (2)" << endl << "Lab manager: (3)" << endl;
		getline(cin, accessLevel);
	}

	// Generate unique document ID
	int maxID = 0;
	for (const auto& docEntry : documentFile) {
		if (docEntry.contains("documentId") && docEntry["documentId"].is_number()) {
			int id = docEntry["documentId"];
			if (id > maxID) maxID = id;
		}
	}
	int uniqueID = maxID + 1;
	// Get time
	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);

	// Create document entry
	newDocument["documentId"] = uniqueID;
	newDocument["assetId"] = stoi(assetId);
	newDocument["type"] = type;
	newDocument["filePath"] = destPath.string();
	newDocument["uploader"] = "Lab Asset Manager";
	newDocument["clearanceLevel"] = accessLevel;
	newDocument["timestamp_utc"] = ctime(&now_c);  // The way this time is recorded a \n will be included at the end. 
	 												//	We can change this by parsing if needed but i think its fine

	documentFile.push_back(newDocument);

	// Save back to file
	ofstream outFileDocuments(documentsFile);
	outFileDocuments << setw(4) << documentFile << endl;
	outFileDocuments.close();
	return true;
}


bool Documents::listDocuments(){
	json documents;

	ifstream inFile(documentsFile);
	if (!inFile.is_open()) {
		cerr << "Error: Could not open " << documentsFile << endl;
		return false;
	}

	try {
		inFile >> documents;
	} catch (const std::exception& error) {
		cerr << "Error reading JSON: " << error.what() << endl;
		inFile.close();
		return false;
	}
	inFile.close();

	if (documents.empty()) {
		cout << "No documents found." << endl;
		return true;
	}

	cout << "Listing all documents:\n" << endl;
	for (const auto& doc : documents) {
		cout << "Document ID: " << doc["documentId"] << endl;
		cout << "Asset ID: " << doc["assetId"] << endl;
		cout << "Type: " << doc["type"] << endl;
		cout << "File Path: " << doc["filePath"] << endl;
		cout << "Uploader: " << doc["uploader"] << endl;
		cout << "Clearance Level: " << doc["clearanceLevel"] << endl;
		cout << "Timestamp (UTC): " << doc["timestamp_utc"] << endl;
		cout << "-----------------------------------" << endl;
	}
	return true;
}