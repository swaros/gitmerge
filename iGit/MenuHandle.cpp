/*
 * File:   MenuHandle.cpp
 * Author: tziegler
 *
 * Created on 5. November 2015, 16:14
 */

#include "MenuHandle.h"
#include "StringWorker.h"

MenuHandle::MenuHandle() {
	this->prompt = "";
	this->endCommand = "q";
	this->currentCommand = "";
	this->nextUsage = "";
	this->commandLenght = 3;
	this->DescriptionLentgh = 15;

	if (this->Screen.getHeight() < 1 || this->Screen.getWidth() < 1) {
		this->simpleOutput = true;
	} else {
		this->simpleOutput = false;
	}
	this->simpleOutput = true;
}

MenuHandle::MenuHandle(const MenuHandle& orig) {
}

MenuHandle::~MenuHandle() {
}

void MenuHandle::console(string message) {
	if (this->simpleOutput) {
		//cout << message << endl;
		ScreenHandle::outln(message, ScreenHandle::WHITE);
	} else {
		cursor(this->Screen.getHeight() - 2, 2);
		ScreenHandle::outln(message, ScreenHandle::WHITE);
		cout << message << endl;
	}
}

void MenuHandle::setPromot(string prmt) {
	this->prompt = prmt;
}

void MenuHandle::addMenu(TMenuEntry entry) {
	this->MenuEntires.push_back(entry);
}

void MenuHandle::fitLenght(string name, string option) {
	if (name.length() > this->DescriptionLentgh) {
		this->DescriptionLentgh = name.length();
	}
	if (option.length() > this->commandLenght) {
		this->commandLenght = option.length();
	}

	if (this->Screen.getWidth() > 0) {
		if (this->commandLenght + this->DescriptionLentgh + 6 > this->Screen.getWidth()) {
			this->DescriptionLentgh = this->Screen.getWidth() - 6 - this->commandLenght;
			if (this->DescriptionLentgh < 1) {
				this->DescriptionLentgh = 4;
				this->commandLenght = 3;
			}
		}
	}
}

void MenuHandle::addMenu(string name, string option) {

	this->fitLenght(name, option);

	TMenuEntry tmpAdd;
	tmpAdd.name = name;
	tmpAdd.cmd = option;
	tmpAdd.show = true;
	this->MenuEntires.push_back(tmpAdd);
}

void MenuHandle::setCommandString(string cmd) {
	string limiter = ";";
	StringWorker strHelper;
	this->commandList = strHelper.split(cmd, limiter);
}

string MenuHandle::showMenu() {
	//cout << "[" << this->prompt << "] | enter command:";

	if (this->commandList.empty() && this->commandList.size() < 1) {
		viewMenu();
		if (!this->simpleOutput) {
			cursor(this->Screen.getHeight() - 2, 5);
		}
		ScreenHandle::out("[", ScreenHandle::LIGHT_BLUE);
		ScreenHandle::out(this->prompt, ScreenHandle::LIGHT_CYAN);
		ScreenHandle::out("] | enter command:", ScreenHandle::LIGHT_BLUE);
		string inputCommand;
		string limiter = ";";
		cin >> inputCommand;

		//inputCommand = getInputString("command:");


		StringWorker strHelper;
		this->commandList = strHelper.split(inputCommand, limiter);
	}

	if (commandList.size() > 0) {
		this->currentCommand = this->commandList[0];
		ScreenHandle::out("command ", ScreenHandle::BROWN);
		ScreenHandle::outln(this->currentCommand, ScreenHandle::MAGENTA);
		vector<string> buff = this->commandList;
		this->commandList.clear();
		for (int i = 1; i < buff.size(); i++) {
			this->commandList.push_back(buff[i]);
		}

	}
	this->currentCommand = this->getCmdParam(this->currentCommand);

	//cin >> this->currentCommand;

	//cout << endl;
	ScreenHandle::outln("", ScreenHandle::NORMAL);
	return this->currentCommand;
}

string MenuHandle::getCmdParam(string cmd) {
	string result;
	string splitby = "=";


	StringWorker strHelp;
	vector<string> strBuff = strHelp.split(cmd, splitby);
	if (strBuff.size() > 1) {
		result = strBuff[0];
		this->nextUsage = strBuff[1];
	} else {
		result = cmd;
	}
	return result;
}

string MenuHandle::getInputString(string label) {
	string result;

	if (this->nextUsage != "") {
		result = this->nextUsage;
		this->nextUsage = "";
	} else {
		string getCommand;
		string splitby = "=";
		ScreenHandle::out(">>.... " + label + ":", ScreenHandle::YELLOW);
		cin >> getCommand;

		StringWorker strHelp;
		vector<string> strBuff = strHelp.split(getCommand, splitby);
		if (strBuff.size() > 1) {
			result = strBuff[0];
			this->nextUsage = strBuff[1];
		} else {
			result = getCommand;
		}

		ScreenHandle::outln("", ScreenHandle::NORMAL);
	}
	return result;
}

string MenuHandle::getCurrentCommand() {
	return this->currentCommand;

}

bool MenuHandle::done() {
	return this->currentCommand == this->endCommand;
}

bool MenuHandle::notDone() {
	return this->currentCommand != this->endCommand;
}

void MenuHandle::setMenuVisibile(string command, bool state) {
	for (int t = 0; t < this->MenuEntires.size(); t++) {
		TMenuEntry current = this->MenuEntires.at(t);
		if (current.cmd == command) {
			this->MenuEntires.at(t).show = state;
		}
	}
}

void MenuHandle::updateMenu(string command, string label) {

	for (int t = 0; t < this->MenuEntires.size(); t++) {
		TMenuEntry current = this->MenuEntires.at(t);
		if (current.cmd == command) {
			this->fitLenght(label, command);
			this->MenuEntires.at(t).name = label;
		}
	}
}

TMenuEntry MenuHandle::getCommandByInput(string cmd) {
	for (int t = 0; t < this->MenuEntires.size(); t++) {
		TMenuEntry current = this->MenuEntires.at(t);
		if (current.cmd == cmd) {
			return current;
		}
	}
	TMenuEntry dummy;
	dummy.cmd = "#";
	dummy.name = "ERROR";

	return dummy;
}

void MenuHandle::viewMenu() {
	StringWorker strHelper;
	if (this->simpleOutput) {
		int strWidth = this->commandLenght + this->DescriptionLentgh + 4;
		ScreenHandle::outln("+" + strHelper.fillString("-", strWidth) + "+", ScreenHandle::BLUE);
		for (int t = 0; t < this->MenuEntires.size(); t++) {
			TMenuEntry current = this->MenuEntires.at(t);
			if (current.show == true) {
				ScreenHandle::out("|  ", ScreenHandle::BLUE);
				ScreenHandle::out(strHelper.strFitRight(current.cmd, this->commandLenght), ScreenHandle::YELLOW);
				ScreenHandle::out("|", ScreenHandle::BLUE);
				ScreenHandle::out(strHelper.strFit(" " + current.name, this->DescriptionLentgh), ScreenHandle::WHITE);
				ScreenHandle::outln("|", ScreenHandle::BLUE);

			}

		}
		ScreenHandle::outln("+" + strHelper.fillString("-", strWidth) + "+", ScreenHandle::BLUE);

		ScreenHandle::out("| ", ScreenHandle::BLUE);
		ScreenHandle::out("type ", ScreenHandle::LIGHT_BLUE);
		ScreenHandle::out(this->endCommand, ScreenHandle::YELLOW);
		ScreenHandle::outln(" to exit menu ", ScreenHandle::LIGHT_BLUE);
		ScreenHandle::outln("+" + strHelper.fillString("-", strWidth), ScreenHandle::BLUE);



	} else {
		int Top = 4;
		int Left = 3;
		this->Screen.clearScreen();

		int w = 10;
		int h = 0;
		for (int t = 0; t < this->MenuEntires.size(); t++) {
			TMenuEntry current = this->MenuEntires.at(t);
			if (current.show == true) {
				this->Screen.print(current.cmd, Left + 5, Top + h, 2);
				this->Screen.print(current.name, Left + 15, Top + h, 3);

				if (w < current.name.length()) {
					w = current.name.length();
				}
				h++;
			}
		}
		this->Screen.printBox(Top, Left, Left + w + 20, h + 1);

		this->Screen.printScreen();
	}

}