/*
 * File:   TermChars.cpp
 * Author: tziegler
 *
 * Created on 16. November 2015, 14:27
 */

#include "TermChars.h"

TermChars::TermChars() {

	TCharCodes singleUtf8;

	singleUtf8.horizontalLine = "\u2500";
	singleUtf8.verticalLine = "\u2502";
	singleUtf8.topLeft = "\u250C";
	singleUtf8.topRight = "\u2510";
	singleUtf8.bottomLeft = "\u2514";
	singleUtf8.bottomRight = "\u2518";

	this->boxCodes.push_back(singleUtf8);

	TCharCodes simple;
	simple.horizontalLine = "-";
	simple.verticalLine = "|";
	simple.topRight = "+";
	simple.topLeft = "+";
	simple.bottomLeft = "+";
	simple.bottomRight = "+";

	this->boxCodes.push_back(simple);

	TCharCodes simple2;
	simple2.horizontalLine = char(196);
	simple2.verticalLine = char(179);
	simple2.topRight = "+";
	simple2.topLeft = "+";
	simple2.bottomLeft = "+";
	simple2.bottomRight = "+";

	this->boxCodes.push_back(simple2);

	this->currentCode =0;


}

TermChars::TermChars(const TermChars& orig) {
}

TermChars::~TermChars() {
}

TCharCodes TermChars::getBoxChars(){
	return this->boxCodes.at(this->currentCode);
}
