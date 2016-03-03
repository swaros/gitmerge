/*
 * File:   CommitList.h
 * Author: tziegler
 *
 * Created on 6. November 2015, 12:23
 */

#ifndef COMMITLIST_H
#define	COMMITLIST_H


#include <iostream>
#include <unordered_map>
#include <string>
#include <unistd.h>
#include <vector>

#include "BaseTypes.h"
#include "GitUser.h"
#include "StringWorker.h"
#include "GitHandler.h"


using namespace std;



class CommitList {
public:
	CommitList();
	CommitList(const CommitList& orig);
	virtual ~CommitList();
	TCommitInfo getCommitInfo(string hash);
	vector<TCommitInfo> getCommitInfo(vector<string> hashes);
	vector<string> getCommiter();
private:
	vector<TCommitInfo> commitInfos;
	TCommitInfo getExistingCommit(string hash);

	GitUser GitUserList;
};

#endif	/* COMMITLIST_H */

