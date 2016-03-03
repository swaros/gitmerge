/*
 * File:   TermHandle.h
 * Author: tziegler
 *
 * Created on 16. November 2015, 11:58
 */

#ifndef TERMHANDLE_H
#define	TERMHANDLE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <sstream>
#include <vector>

#include "TermInfo.h"
#include "TermChars.h"

#define   ESC             27
#define   cls()           printf("%c[2J",ESC)
#define   cursor(a,b)     printf("%c[%d;%dH",ESC,a,b)

using namespace std;

struct TOutPutString{
	string message;
	int x;
	int y;
	int Color;
};


struct TOutPutLine{
	vector<TOutPutString> Content;
};

class TermHandle {
public:



	TermHandle();
	TermHandle(const TermHandle& orig);
	virtual ~TermHandle();

	void print(string message, int x, int y, int Color);
	void printBox(int left, int top, int width, int height);

	void clearScreen();
	void printScreen();
	int getWidth();
	int getHeight();
private:


	bool usePlainOutPut;
	void printLine(int nr);
	string fillString(string with, int lenght);

	TermInfo TerminalInfo;
	vector<TOutPutLine> Screen;

	TermChars Characters;

};

#endif	/* TERMHANDLE_H */

