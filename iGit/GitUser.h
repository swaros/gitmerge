/*
 * File:   GitUser.h
 * Author: tziegler
 *
 * Created on 9. November 2015, 10:37
 */

#ifndef GITUSER_H
#define	GITUSER_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <unistd.h>
#include <vector>

#include "BaseTypes.h"
#include "StringWorker.h"
#include "GitHandler.h"




class GitUser {
public:
	GitUser();
	GitUser(const GitUser& orig);
	virtual ~GitUser();

	void addGitUser(TCommitInfo info);
	TGitUser getGitUser(string name);

	vector<string> listUserNames();
	vector<TGitUser> getUserNames();

private:
	vector<TGitUser> users;
};

#endif	/* GITUSER_H */

