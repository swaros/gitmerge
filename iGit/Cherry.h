/*
 * File:   Cherry.h
 * Author: tziegler
 *
 * Created on 9. November 2015, 18:41
 */

#ifndef CHERRY_H
#define	CHERRY_H

#include "BaseTypes.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "GitHandler.h"
#include "StringWorker.h"

using namespace std;

class Cherry {
public:
	bool showMessages;

	Cherry();
	Cherry(const Cherry& orig);
	virtual ~Cherry();
	bool hashExists(string hash);
	int hashPosition(string hash);
	bool missingHash(string hash);
	bool existingHash(string hash);
	void setBaseBranch(string branch);
	void setCompareBranch(string branch);
	void read();
	vector<string> getCherryList();
	string getBaseBranch();
	string getCompareBranch();

	string getHashFromLine(string line);

	int size();
private:
	unordered_map<string,TCherry> hashes;
	vector<string> plainList;
	string baseBranch;
	string compareBranch;
};

#endif	/* CHERRY_H */

