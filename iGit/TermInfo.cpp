/*
 * File:   TermInfo.cpp
 * Author: tziegler
 *
 * Created on 16. November 2015, 11:45
 */

#include "TermInfo.h"



TermInfo::TermInfo() {
	this->ReadInfo();
}

TermInfo::TermInfo(const TermInfo& orig) {
}

TermInfo::~TermInfo() {
}

void TermInfo::ReadInfo(){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	this->width = w.ws_col;
	this->height = w.ws_row;

}
