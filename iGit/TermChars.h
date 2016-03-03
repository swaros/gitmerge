/*
 * File:   TermChars.h
 * Author: tziegler
 *
 * Created on 16. November 2015, 14:27
 */

#ifndef TERMCHARS_H
#define	TERMCHARS_H


#include <string>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>
#include "TermChars.h"

using namespace std;

struct TCharCodes {
	string horizontalLine;
	string verticalLine;
	string topLeft;
	string topRight;
	string bottomLeft;
	string bottomRight;
};


class TermChars {
public:
	TermChars();
	TermChars(const TermChars& orig);
	virtual ~TermChars();

	TCharCodes getBoxChars();

private:
	int currentCode = 0;
	vector<TCharCodes> boxCodes;

};

#endif	/* TERMCHARS_H */

