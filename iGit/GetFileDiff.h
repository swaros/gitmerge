/*
 * File:   GetFileDiff.h
 * Author: tziegler
 *
 * Created on 5. November 2015, 12:26
 */

#ifndef GETFILEDIFF_H
#define	GETFILEDIFF_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <unistd.h>

#include "StringWorker.h"
#include "GitHandler.h"
#include "CommitList.h"
#include "Cherry.h"
#include "ScreenHandle.h"
using namespace std;




struct TGitFile {
	string Filename;
	string state;
	vector<string> currentHashes;
	vector<string> compareHashes;
	vector<string> diffs;
	bool pickThis;
	bool marked;
	int positionInDiff;
};



class GetFileDiff {
public:

	bool executeCm = false;

	GetFileDiff();
	GetFileDiff(const GetFileDiff& orig);
	virtual ~GetFileDiff();

	void setWorkingDir(string path);
	void setCurrentBranch(string branch);
	void setCheckBranch(string branch);
	void setBasePath(string path);
	void addAuthor();
	bool findAuthor(string name);
	bool fileExists(string name);
	string getStateColor(string state);
	string getWorkingDir();
	string getCurrentBranch();
	string getCheckBranch();
	string getBasePath();
	void getHashes();
	void getHashes(bool showAffectedFiles);
	void getDiffFiles();
	int size();
	void printDiffFiles();
	void printMissingDiffFiles();
	void cherryPick(int number);
	void clear();
	void markAll();
	string getMarkedCherryHashes();
	void cherryPickMarked();
	void printCherryPick(int number);
	void cherryPickAllMarked();
	void runCherryForMarked();
	void markForCherryPick(int number);
	int getNumberByFilename(string filename);
	void pullRebase();
	void pushTartget();
	void printVectorList(vector<string> buff);
	void gitExec(string cmd);
	void showCherryList();
	void showCherryList(bool onlyAdded);
	void printCherryAssignedList();
	vector<string> getAffectedFilesByHash(string hash, string originName);
	string GetAuthorsAsOnlineString();
	string getCurrentGitBranch();

	vector<string> getDiffHashes(string filename, string branch);

private:

	void applyDiffHash(string hash);
	bool inPicDiffList(int nr);

	string workingDirectory;
	string currentBranch;
	string checkBranch;
	string basedir;
	unordered_map<std::string, std::string> fileList;
	vector<TGitFile> gitFiles;
	vector<string> includedAuthor;
	GitHandler GitExex;

	CommitList Commit;
	Cherry CherryList;

	// the position numbers in the cherrylist
	vector<int> picPositions;

	// the maximum number of picks in range.
	// depends on the numbers in picPositions
	int picUntilTo;

};

#endif	/* GETFILEDIFF_H */

