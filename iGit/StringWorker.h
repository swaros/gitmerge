/*
 * File:   StringWorker.h
 * Author: tziegler
 *
 * Created on 5. November 2015, 13:27
 */

#ifndef STRINGWORKER_H
#define	STRINGWORKER_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

class StringWorker {
public:
	StringWorker();
	StringWorker(const StringWorker& orig);
	virtual ~StringWorker();
	string fillString(string text,int len);
	string strFit(string text,int len);
	string strFitRight(string text,int len);
	vector<string> split(const string& s, const string& delim);
private:

};

#endif	/* STRINGWORKER_H */

