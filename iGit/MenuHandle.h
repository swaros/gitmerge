/*
 * File:   MenuHandle.h
 * Author: tziegler
 *
 * Created on 5. November 2015, 16:14
 */

#ifndef MENUHANDLE_H
#define	MENUHANDLE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <sstream>
#include <vector>
#include "BaseTypes.h"
#include "TermHandle.h"
#include "ScreenHandle.h"

using namespace std;




class MenuHandle {
public:
	MenuHandle();
	MenuHandle(const MenuHandle& orig);
	virtual ~MenuHandle();

	void addMenu(TMenuEntry entry);
	void addMenu(string name, string option);
	void viewMenu();
	bool done();
	bool notDone();
	string getInputString(string label);
	void updateMenu(string command, string label);
	void setMenuVisibile(string command,bool state);
	TMenuEntry getCommandByInput(string cmd);
	string getCurrentCommand();
	string showMenu();
	string getCmdParam(string cmd);
	void setCommandString(string cmd);
	void setPromot(string prmt);
	void console(string message);
private:
	void fitLenght(string name, string option);
	vector<TMenuEntry> MenuEntires;
	vector<string> commandList;

	string endCommand;
	string currentCommand;
	string prompt;
	string nextUsage;

	bool simpleOutput;

	TermHandle Screen;

	int commandLenght;
	int DescriptionLentgh;

};

#endif	/* MENUHANDLE_H */

