/*
 * File:   BaseTypes.h
 * Author: tziegler
 *
 * Created on 9. November 2015, 11:26
 */

#ifndef BASETYPES_H
#define	BASETYPES_H

#include <string>
#include <vector>

using namespace std;

struct TGitUser {
	string Name;
	string Email;
	bool marked;
	int commitCount;
	bool initialized;
	vector<string> hashes;
};


struct TCommitInfo {
	bool valid;
	bool marked;
	bool listed;
	string Hash;
	string Author;
	string Date;
	string Topic;
	int hitCount;
};


struct TMenuEntry {
	string name;
	string cmd;
	bool show;
};

struct TCherry {
	string flag;
	string hash;
	string message;
	int position;
};

#endif	/* BASETYPES_H */

