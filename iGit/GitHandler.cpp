/*
 * File:   GitHandler.cpp
 * Author: tziegler
 *
 * Created on 5. November 2015, 12:51
 */

#include "GitHandler.h"

GitHandler::GitHandler() {
	this->currentBranch = "***";
}

GitHandler::GitHandler(const GitHandler& orig) {
}

GitHandler::~GitHandler() {
}

string GitHandler::getCurrentBranch() {
	if (this->currentBranch == "***") {
		return this->readCurrentBranch();
	}
	return this->currentBranch;
}

string GitHandler::readCurrentBranch() {
	string cmd = "git branch | grep '* '";
	this->currentBranch = "unknow";
	vector<string> result = this->execute(cmd);
	StringWorker StrWorker;
	if (result.size() == 1) {
		vector<string> part = StrWorker.split(result.at(0), " ");
		if (part.size() == 2) {
			this->currentBranch = part.at(1);
		}
	}
	return this->currentBranch;
}

vector<string> GitHandler::execute(string command) {

	//cout << "exec[" << command << "]" << endl;

	FILE* stream = popen(command.c_str(), "r");
	std::ostringstream output;
	//unordered_map<std::string, int> resultList;
	while (!feof(stream) && !ferror(stream)) {
		char buf[128];
		int bytesRead = fread(buf, 1, 128, stream);
		output.write(buf, bytesRead);
	}

	std::string result = output.str();
	//std::cout << "<RESULT>" << std::endl << result << "</RESULT>" << std::endl;

	StringWorker StrWorker;
	/*
	vector<string> listing = StrWorker.split(result, "\n");

	for (int i=0; i<listing.size(); i++){
		string entry = listing.at(i);
		cout << "GOT: " << entry << endl;
	}
	 */

	return StrWorker.split(result, "\n");
}

