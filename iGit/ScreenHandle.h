/*
 * File:   ScreenHandle.h
 * Author: tziegler
 *
 * Created on 24. November 2015, 10:44
 */

#ifndef SCREENHANDLE_H
#define	SCREENHANDLE_H

#include <cstdlib>
#include <string>
#include "TermHandle.h"

using namespace std;

namespace ScreenHandle {

	const string BLACK = "[0;30m";
	const string RED = "[0;31m";
	const string GREEN = "[0;32m";
	const string BROWN = "[0;33m";
	const string BLUE = "[0;34m";
	const string CYAN = "[0;36m";
	const string YELLOW = "[1;33m";
	const string MAGENTA = "[1;35m";
	const string WHITE = "[1;37m";
	const string NORMAL = "[0m";
	const string BOLD = "[1m";
	const string UNDERSCORE = "[4m";
	const string LIGHT_RED = "[1;31m";
	const string LIGHT_GREEN = "[1;32m";
	const string LIGHT_BLUE = "[1;34m";
	const string LIGHT_CYAN = "[1;36m";
	const string REVERSE = "[7m";

const int MAXCOLORS = 8;
void enableColor(bool onoff);
void out(string message, string colorCode);
void outln(string message, string colorCode);
void echo(string message, int left, int top, string color);

};

#endif	/* SCREENHANDLE_H */

