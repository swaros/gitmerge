/*
 * File:   TermHandle.cpp
 * Author: tziegler
 *
 * Created on 16. November 2015, 11:58
 */

#include "TermHandle.h"
#include "TermChars.h"

TermHandle::TermHandle() {

}

TermHandle::TermHandle(const TermHandle& orig) {
}

TermHandle::~TermHandle() {
}

int TermHandle::getHeight(){
	return this->TerminalInfo.height;
}

int TermHandle::getWidth(){
	return this->TerminalInfo.width;
}


void TermHandle::print(string message,int x, int y, int Color){
	if (this->Screen.size() > y){

		TOutPutString newString;
		newString.Color = Color;
		newString.x = x;
		newString.y = y;
		newString.message = message;

		this->Screen.at(y).Content.push_back(newString);

	}
}

void TermHandle::printBox(int left, int top, int width, int height){
	int len = width-1;

	TCharCodes CharCode = Characters.getBoxChars();

	string topstr ="";
	string bottomstr ="";

	topstr = CharCode.topLeft;
	topstr += this->fillString(CharCode.horizontalLine ,len);
	topstr += CharCode.topRight;

	bottomstr = CharCode.bottomLeft;
	bottomstr += this->fillString(CharCode.horizontalLine ,len);
	bottomstr += CharCode.bottomRight;


	for (int i=top; i < top + height; i++){
		this->print(CharCode.verticalLine,left,i,1);
		this->print(CharCode.verticalLine,left+width,i,1);
	}


	this->print(topstr,left,top,1);
	this->print(bottomstr,left,top+height,1);

}


void TermHandle::clearScreen(){
	cout << "\033[2J\033[1;1H";
	this->Screen.clear();
	for (int i = 0; i < TerminalInfo.height; i++){
		TOutPutLine newLIne;
		this->Screen.push_back(newLIne);
	}
}

void TermHandle::printScreen(){
	for (int i = 0; i < TerminalInfo.height; i++){
		this->printLine(i);
	}
}
string TermHandle::fillString(string with, int lenght){

	string out= "";
	for (int i=0; i < lenght; i++){
		out+=with;
	}

	return out;
}
void TermHandle::printLine(int nr){

	if (nr < this->Screen.size()){
		for (int i = 0; i < this->Screen.at(nr).Content.size(); i++){
			TOutPutString currentString = this->Screen.at(nr).Content.at(i);
			cursor(currentString.y,currentString.x);
			cout << currentString.message;
		}

	}

}
