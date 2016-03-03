/*
 * File:   CommitList.cpp
 * Author: tziegler
 *
 * Created on 6. November 2015, 12:23
 */

#include "CommitList.h"

CommitList::CommitList() {
}

CommitList::CommitList(const CommitList& orig) {
}

CommitList::~CommitList() {
}

TCommitInfo CommitList::getCommitInfo(string hash){
	//git show --pretty=format:%an%n%H%n%ad%n%s 19253e278513cdf8324e52d56ac4b8d1e041c90b
	// [START]%n%an%n%H%n%ad%n%s%n[END]

	TCommitInfo info = getExistingCommit(hash);

	if (info.listed){
		return info;
	}

	GitHandler check;
	vector<string> commitInfos;
	commitInfos = check.execute("git show --pretty=format:%an%n%H%n%ad%n%s " + hash);
	/*
	Thomas Ziegler
	19253e278513cdf8324e52d56ac4b8d1e041c90b
	Mon Nov 2 10:53:35 2015 +0100
	Ranking ..add new switch for Navigation that depends on app.conf setting
	diff --git a/code/presentation/website/include/Sflib/Navigation/Container/Creator.php b/code/presentation/website/include/Sflib/Navigation/Container/Creator.php
	index bf6e0f9..2b1a73d 100644

	 */

	if (commitInfos.size() > 5){
		info.valid = true;
		info.Author = commitInfos.at(0);
		info.Date = commitInfos.at(2);
		info.Topic = commitInfos.at(3);
		info.listed = true;
		info.hitCount = 0;

		this->GitUserList.addGitUser(info);

		this->commitInfos.push_back(info);
	}


	return info;

}

vector<TCommitInfo> CommitList::getCommitInfo(vector<string> hashes){

	vector<TCommitInfo> resultList;
	string hashGet = "";
	string add = "";
	bool needCall = false;

	for (int i=0; i < hashes.size();i++){
		TCommitInfo info = getExistingCommit(hashes.at(i));
		cout << " " << hashes.at(i);

		if (info.listed == false){
			hashGet += add + hashes.at(i);
			add = " ";
			needCall = true;

		} else {
			resultList.push_back(info);
		}
	}
	cout << endl;
	// if some infos not stored we will get them
	if (needCall== true){
		GitHandler check;
		vector<string> commitInfos;
		commitInfos = check.execute("git show --pretty=format:[START]%n%an%n%H%n%ad%n%s%n[END] " + hashGet);

		bool infoBlock = false;
		int currentLine = 0;

		string author;
		string hash;
		string date;
		string topic;


		for(int i=0; i < commitInfos.size(); i++){
			string textLine = commitInfos.at(i);

			if (textLine == "[END]"){
				if (infoBlock == true){
					TCommitInfo secondInfo = getExistingCommit(hash);
					if (secondInfo.listed == false){
						secondInfo.Author = author;
						secondInfo.Hash = hash;
						secondInfo.Date = date;
						secondInfo.Topic = topic;
						secondInfo.listed = true;
						secondInfo.hitCount = 0;
						secondInfo.valid = true;
						this->GitUserList.addGitUser(secondInfo);
						this->commitInfos.push_back(secondInfo);

					} else {
						secondInfo.hitCount++;
					}
					resultList.push_back(secondInfo);

				}
				infoBlock = false;
			}

			if (infoBlock == true){
				currentLine++;

				if (currentLine == 1) author = textLine;
				if (currentLine == 2) hash = textLine;
				if (currentLine == 3) date = textLine;
				if (currentLine == 4) topic = textLine;


			}


			if (textLine == "[START]"){
				infoBlock = true;
				currentLine = 0;
			}

		}


	}
	return resultList;
}


vector<string> CommitList::getCommiter(){
	return this->GitUserList.listUserNames();
}

TCommitInfo CommitList::getExistingCommit(string hash){
	for (int i=0; i < this->commitInfos.size();i++){
		if (this->commitInfos.at(i).Hash == hash){
			this->commitInfos.at(i).hitCount++;
			return this->commitInfos.at(i);
		}
	}
	TCommitInfo info;
	info.Hash = hash;
	info.marked= false;
	info.valid = false;
	info.listed = false;
	return info;
}
