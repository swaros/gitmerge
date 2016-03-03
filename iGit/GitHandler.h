/*
 * File:   GitHandler.h
 * Author: tziegler
 *
 * Created on 5. November 2015, 12:51
 */

#ifndef GITHANDLER_H
#define	GITHANDLER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <sstream>

#include "StringWorker.h"

using namespace std;


class GitHandler {
public:
	GitHandler();
	GitHandler(const GitHandler& orig);
	virtual ~GitHandler();
	string readCurrentBranch();
	string getCurrentBranch();
	vector<string> execute(string command);
private:
	string currentBranch;
};

#endif	/* GITHANDLER_H */

