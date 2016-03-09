/*
 * File:   Cherry.cpp
 * Author: tziegler
 *
 * Created on 9. November 2015, 18:41
 */

#include <X11/Xutil.h>

#include "Cherry.h"

Cherry::Cherry() {
	this->baseBranch = "master";
	this->showMessages = false;
}

Cherry::Cherry(const Cherry& orig) {
}

Cherry::~Cherry() {
}

void Cherry::setBaseBranch(string branch) {
	this->baseBranch = branch;
}

void Cherry::setCompareBranch(string branch) {
	this->compareBranch = branch;
}

vector<string> Cherry::getCherryList() {
	return this->plainList;
}

string Cherry::getBaseBranch() {
	return this->baseBranch;
}

string Cherry::getCompareBranch() {
	return this->compareBranch;
}

int Cherry::size() {
	return this->hashes.size();
}

bool Cherry::missingHash(string hash) {
	if (this->hashExists(hash)) {
		return this->hashes[hash].flag == "+";
	}
	return false;
}

TCherry Cherry::getCherrybyhash(string hash) {
	if (this->hashExists(hash)) {
		return this->hashes[hash];
	}
	TCherry dummy;
	dummy.flag = '#';
	dummy.hash = "dummyhash";
	dummy.message = "just a dummy";
	dummy.position = -1;
	return dummy;
}

bool Cherry::existingHash(string hash) {
	if (this->hashExists(hash)) {
		return this->hashes[hash].flag == "-";
	}
	return false;
}

bool Cherry::hashExists(string hash) {
	unordered_map<string, TCherry>::const_iterator found = this->hashes.find(hash);
	if (found == this->hashes.end()) {
		return false;
	} else {
		return true;
	}
}

int Cherry::hashPosition(string hash) {
	if (this->hashExists(hash)) {
		return this->hashes[hash].position;
	}
	return -1;
}

string Cherry::getHashFromLine(string line) {

	StringWorker Strings;
	vector<string> parts = Strings.split(line, " ");
	int partSize = parts.size();
	if (partSize >= 3) {
		string key = parts.at(1);
		string modif = parts.at(0);
		string message = parts.at(2);
		return key;
	}
	return "";
}

void Cherry::read() {


	GitHandler gitExec;
	this->plainList.clear();
	string cmd = "git cherry -v " + this->compareBranch + " " + this->baseBranch;
	this->plainList = gitExec.execute(cmd);
	StringWorker Strings;
	this->hashes.clear();
	if (this->showMessages) cout << "entries " << this->plainList.size() << endl;
	for (int i = 0; i < this->plainList.size(); i++) {
		string entry = this->plainList.at(i);
		if (this->showMessages) cout << entry << endl;
		vector<string> parts = Strings.split(entry, " ");
		int partSize = parts.size();
		if (partSize >= 3) {
			string key = parts.at(1);
			string modif = parts.at(0);
			string message = parts.at(2);

			for (int t = 3; t < partSize; t++) {
				message += " " + parts.at(t);
			}

			if (this->showMessages) {
				cout << "\t" << key << endl;
				cout << "\t" << modif << endl;
				cout << "\t" << message << endl;
			}

			TCherry newCherry;
			newCherry.flag = modif;
			newCherry.hash = key;
			newCherry.message = message;
			newCherry.position = i;
			if (this->hashes.size() + 1 >= this->hashes.max_bucket_count()) {
				cout << "\t\t\tERROR: limit reached " << this->hashes.max_bucket_count() << endl;
			} else {
				unordered_map<string, TCherry>::const_iterator found = this->hashes.find(key);
				if (found == this->hashes.end()) {
					this->hashes[key] = newCherry;
				} else {
					cout << "\t\t\tERROR: Hash already stored " << key << endl;
				}
			}

		}

	}

}