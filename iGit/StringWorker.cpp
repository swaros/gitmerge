/*
 * File:   StringWorker.cpp
 * Author: tziegler
 *
 * Created on 5. November 2015, 13:27
 */

#include <X11/Xlib.h>

#include "StringWorker.h"

StringWorker::StringWorker() {
}

StringWorker::StringWorker(const StringWorker& orig) {
}

StringWorker::~StringWorker() {
}

string StringWorker::fillString(string text, int len) {
	string result;
	for (int i = 0; i < len; i++) {
		result += text;
	}
	return result;
}

string StringWorker::strFit(string text, int len) {
	if (text.length() > len) {
		return text.substr(0, len);
	}

	if (text.length() < len) {
		string added = text;
		for (int i = text.length(); i <= len; i++) {
			added += " ";
		}
		return added;
	}
	return text;
}

string StringWorker::strFitRight(string text, int len) {
	if (text.length() > len) {
		return text.substr(0, len);
	}

	if (text.length() < len) {
		string added = text;
		for (int i = text.length(); i < len; i++) {
			added = " " + added;
		}
		return added;
	}
	return text;
}

vector<string> StringWorker::split(const string& s, const string& delim) {
	Bool keep_empty = false;
	vector<string> result;
	if (delim.empty()) {
		result.push_back(s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);
		if (keep_empty || !temp.empty()) {
			result.push_back(temp);
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return result;
}