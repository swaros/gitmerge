/*
 * File:   GitUser.cpp
 * Author: tziegler
 *
 * Created on 9. November 2015, 10:37
 */

#include "GitUser.h"




GitUser::GitUser() {
}

GitUser::GitUser(const GitUser& orig) {
}

GitUser::~GitUser() {
}

void GitUser::addGitUser(TCommitInfo info){
	TGitUser newUser = this->getGitUser(info.Author);
	if (newUser.initialized == false){
		newUser.Name = info.Author;
		newUser.commitCount = 0;
		newUser.initialized = true;
		this->users.push_back(newUser);
	}
}

TGitUser GitUser::getGitUser(string name){
	for (int i=0; i < this->users.size(); i++){
		if (this->users.at(i).Name == name){
			return this->users.at(i);
		}
	}
	TGitUser newUser;
	newUser.initialized = false;
	return newUser;
}

vector<string> GitUser::listUserNames(){
	vector<string> returnUser;
	for (int i=0; i < this->users.size(); i++){
		returnUser.push_back(this->users.at(i).Name);
	}
	return returnUser;

}
vector<TGitUser> GitUser::getUserNames(){
	return this->users;
}