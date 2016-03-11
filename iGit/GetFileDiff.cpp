/*
 * File:   GetFileDiff.cpp
 * Author: tziegler
 *
 * Created on 5. November 2015, 12:26
 */


#include <X11/Xlib.h>

#include "GetFileDiff.h"

#define CHERRY_PICK_EXIT_CODE_MERGE_CONFLICT 127

GetFileDiff::GetFileDiff() {
	this->currentBranch = "master";
	this->basedir = "";
	this->picUntilTo = -1;
}

GetFileDiff::GetFileDiff(const GetFileDiff& orig) {
}

GetFileDiff::~GetFileDiff() {
}

int GetFileDiff::size() {
	return this->gitFiles.size();
}

void GetFileDiff::pullRebase() {
	string cmd = "git checkout " + this->currentBranch;
	this->gitExec(cmd);
	cmd = "git pull --rebase";
	this->gitExec(cmd);
	cmd = "git checkout " + this->checkBranch;
	this->gitExec(cmd);
	cmd = "git pull --rebase";
	this->gitExec(cmd);
	cmd = "git checkout " + this->currentBranch;
	this->gitExec(cmd);

	this->CherryList.read();

}

void GetFileDiff::pushTartget() {
	string cmd = "git checkout " + this->checkBranch;
	this->gitExec(cmd);
	cmd = "git pull --rebase";
	this->gitExec(cmd);
	cmd = "git push";
	this->gitExec(cmd);
	cmd = "git checkout " + this->currentBranch;
	this->gitExec(cmd);


}

void GetFileDiff::gitExec(string cmd) {
	int cdRes = chdir(this->workingDirectory.c_str());
	vector<string> gitHashes;
	gitHashes = GitExex.execute(cmd);
	this->printVectorList(gitHashes);

}

void GetFileDiff::showCherryList() {
	showCherryList(true);
}

void GetFileDiff::showCherryList(bool onlyAdded) {
	//this->printVectorList(this->CherryList.getCherryList());

	for (int i = 0; i < this->CherryList.getCherryList().size(); i++) {

		string fullInfoLine = this->CherryList.getCherryList().at(i);
		string cherryHash = this->CherryList.getHashFromLine(fullInfoLine);

		TCherry info = this->CherryList.getCherrybyhash(cherryHash);

		if (info.flag != "#" && (onlyAdded == false || info.flag == "+")) {
			ScreenHandle::out(info.flag, ScreenHandle::LIGHT_GREEN);

			// look if this entry already in piclist an mark them
			std::vector<int>::iterator it;
			it = find(this->picPositions.begin(), this->picPositions.end(), i);
			bool included = false;
			if (it != this->picPositions.end()) {
				if (i <= this->picUntilTo) ScreenHandle::out(info.hash, ScreenHandle::YELLOW);
				else ScreenHandle::out(info.hash, ScreenHandle::BOLD);
				included = true;
			} else {
				ScreenHandle::out(info.hash, ScreenHandle::NORMAL);
			}


			ScreenHandle::out(" ", ScreenHandle::LIGHT_CYAN);
			if (included) {
				if (i <= this->picUntilTo) ScreenHandle::outln(info.message, ScreenHandle::YELLOW);
				else ScreenHandle::outln(info.message, ScreenHandle::LIGHT_BLUE);
			} else {
				ScreenHandle::outln(info.message, ScreenHandle::BLUE);
			}
		}

	}

}

void GetFileDiff::printVectorList(vector<string> buff) {
	for (int i = 0; i < buff.size(); i++) {
		ScreenHandle::outln(buff.at(i), ScreenHandle::NORMAL);
	}

}

void GetFileDiff::setWorkingDir(string path) {
	this->workingDirectory = path;
}

void GetFileDiff::setCheckBranch(string branch) {
	this->checkBranch = branch;
	this->CherryList.setCompareBranch(branch);
}

void GetFileDiff::setCurrentBranch(string branch) {
	this->currentBranch = branch;
	this->CherryList.setBaseBranch(branch);
}

void GetFileDiff::setBasePath(string path) {
	this->basedir = path;

}

string GetFileDiff::getWorkingDir() {
	return this->workingDirectory;
}

string GetFileDiff::getCheckBranch() {
	return this->checkBranch;
}

string GetFileDiff::getCurrentBranch() {
	return this->currentBranch;
}

string GetFileDiff::getBasePath() {
	return this->basedir;
}

void GetFileDiff::addAuthor() {

	vector<string> commitUser = this->Commit.getCommiter();
	if (commitUser.size() < 1) {

		ScreenHandle::outln("no committer still found", ScreenHandle::RED);
	} else {

		for (int i = 0; i < commitUser.size(); i++) {
			ScreenHandle::out(i + "", ScreenHandle::YELLOW);
			ScreenHandle::outln("\t" + commitUser.at(i), ScreenHandle::LIGHT_BLUE);
		}
		ScreenHandle::outln("----------------------", ScreenHandle::BLUE);
		ScreenHandle::outln("input user number:", ScreenHandle::YELLOW);


		string number;
		cin >> number;
		int nr = std::stoi(number);

		if (nr >= 0 && nr < commitUser.size()) {
			if (findAuthor(commitUser.at(nr)) == false) {
				this->includedAuthor.push_back(commitUser.at(nr));
			}
		}
	}
}

bool GetFileDiff::findAuthor(string name) {
	for (int i = 0; i < this->includedAuthor.size(); i++) {
		if (this->includedAuthor.at(i) == name) {
			return true;
		}
	}
	return false;
}

bool GetFileDiff::fileExists(string name) {
	for (int i = 0; i < this->gitFiles.size(); i++) {
		if (this->gitFiles.at(i).Filename == name) {
			return true;
		}
	}
	return false;
}

string GetFileDiff::GetAuthorsAsOnlineString() {
	string infoStr = "";
	for (int i = 0; i < this->includedAuthor.size(); i++) {
		infoStr += this->includedAuthor.at(i);
	}
	return infoStr;
}

string GetFileDiff::getStateColor(string state) {
	string fileColor = ScreenHandle::LIGHT_BLUE;

	if (state == "D") {
		fileColor = ScreenHandle::LIGHT_RED;
	}

	if (state == "A") {
		fileColor = ScreenHandle::LIGHT_GREEN;
	}
	return fileColor;
}

void GetFileDiff::applyDiffHash(string hash) {
	if (this->CherryList.missingHash(hash)) {
		TCherry info = this->CherryList.getCherrybyhash(hash);
		if (info.position >= 0 && !inPicDiffList(info.position)) {
			picPositions.push_back(info.position);
		}
	}
}

bool GetFileDiff::inPicDiffList(int nr) {
	std::vector<int>::iterator it;
	it = find(this->picPositions.begin(), this->picPositions.end(), nr);
	bool included = false;
	if (it != this->picPositions.end()) {
		included = true;
	}
	return included;
}

void GetFileDiff::printCherryAssignedList() {
	if (picPositions.size() > 0) {
		std::sort(picPositions.begin(), picPositions.end());
		ScreenHandle::outln("List of affected Commits", ScreenHandle::YELLOW);
		for (int i = 0; i < picPositions.size(); i++) {
			int current = picPositions.at(i);

			string fullInfo = this->CherryList.getCherryList().at(current);
			string cherryHash = this->CherryList.getHashFromLine(fullInfo);

			TCherry info = this->CherryList.getCherrybyhash(cherryHash);

			//ScreenHandle::outln(foundHashes.at(i), ScreenHandle::CYAN);
			if (info.flag != "#") {
				ScreenHandle::out(info.flag, ScreenHandle::LIGHT_GREEN);
				if (info.position <= this->picUntilTo) {
					ScreenHandle::out(info.hash, ScreenHandle::BOLD);
				} else {
					ScreenHandle::out(info.hash, ScreenHandle::NORMAL);

				}
				ScreenHandle::out("|", ScreenHandle::LIGHT_CYAN);
				ScreenHandle::out(to_string(info.position), ScreenHandle::LIGHT_GREEN);
				ScreenHandle::out("|", ScreenHandle::LIGHT_CYAN);
				ScreenHandle::outln(info.message, ScreenHandle::NORMAL);
			}
		}
	}
}

void GetFileDiff::printDiffFiles() {
	string colorCode = ScreenHandle::NORMAL;
	//vector<string> foundHashes;
	//this->picPositions.clear();
	for (int i = 0; i < this->gitFiles.size(); i++) {
		int countOfDiffs = this->gitFiles.at(i).diffs.size();
		if (this->gitFiles.at(i).positionInDiff > -1 && this->gitFiles.at(i).positionInDiff <= this->picUntilTo) {
			colorCode = ScreenHandle::LIGHT_BLUE;
			ScreenHandle::out("+\t", ScreenHandle::LIGHT_GREEN);
		} else {
			colorCode = ScreenHandle::BLUE;
			ScreenHandle::out("-\t", ScreenHandle::LIGHT_RED);
		}

		// flag
		string fileColor = getStateColor(this->gitFiles.at(i).state);
		ScreenHandle::out(std::to_string(i) + "\t", colorCode);

		if (this->gitFiles.at(i).positionInDiff > -1) {

			ScreenHandle::out(to_string(this->gitFiles.at(i).positionInDiff) + "\t", ScreenHandle::CYAN);
		} else {
			ScreenHandle::out("?\t", ScreenHandle::WHITE);
		}

		ScreenHandle::out("[" + this->gitFiles.at(i).state + "]\t", ScreenHandle::MAGENTA);
		ScreenHandle::outln(this->gitFiles.at(i).Filename, fileColor);
	}

	printCherryAssignedList();

}

void GetFileDiff::printMissingDiffFiles() {

	for (int i = 0; i < this->gitFiles.size(); i++) {
		if (this->gitFiles.at(i).pickThis) {
			int countOfDiffs = this->gitFiles.at(i).diffs.size();
			string colorCode = ScreenHandle::NORMAL;
			//cout << i << "\t" << countOfDiffs << "\t" << this->gitFiles.at(i).state << "\t" << this->gitFiles.at(i).Filename << endl;

			string fileColor = getStateColor(this->gitFiles.at(i).state);


			ScreenHandle::out(std::to_string(i) + "\t", ScreenHandle::CYAN);
			ScreenHandle::out(to_string(countOfDiffs) + "\t", colorCode);
			ScreenHandle::out("[" + this->gitFiles.at(i).state + "]\t", colorCode);
			ScreenHandle::outln(this->gitFiles.at(i).Filename, fileColor);

		}
	}

}

vector<string> GetFileDiff::getAffectedFilesByHash(string hash, string originName) {
	vector<string> affectedFiles;

	vector<string> res;
	string cmd = "git show --pretty=\"format:\" --name-only " + hash;
	affectedFiles = GitExex.execute(cmd);

	for (int i = 0; i < affectedFiles.size(); i++) {
		if (affectedFiles.at(i) != "" && affectedFiles.at(i) != originName) {
			res.push_back(affectedFiles.at(i));
		}
	}

	return res;
}

void GetFileDiff::clear() {
	this->gitFiles.clear();
	this->picPositions.clear();
}

void GetFileDiff::getHashes() {
	this->getHashes(false);
}

void GetFileDiff::getHashes(bool showAffectedFiles) {

	// git cherry -v release/release_15.11.1 master
	for (int i = 0; i < this->gitFiles.size(); i++) {
		// no choosen for picking
		this->gitFiles.at(i).pickThis = false;

		string fileColor = getStateColor(this->gitFiles.at(i).state);
		ScreenHandle::out(std::to_string(i) + "\t", ScreenHandle::CYAN);
		ScreenHandle::outln(this->gitFiles.at(i).Filename, fileColor);

		this->gitFiles.at(i).currentHashes = getDiffHashes(this->gitFiles.at(i).Filename, this->currentBranch);


		ScreenHandle::out("\t" + this->currentBranch + "\t", ScreenHandle::BLUE);
		ScreenHandle::out("commits:", ScreenHandle::NORMAL);
		ScreenHandle::outln(to_string(this->gitFiles.at(i).currentHashes.size()), ScreenHandle::YELLOW);


		this->gitFiles.at(i).compareHashes = getDiffHashes(this->gitFiles.at(i).Filename, this->checkBranch);

		ScreenHandle::out("\t" + this->checkBranch + "\t", ScreenHandle::BLUE);
		ScreenHandle::out("commits:", ScreenHandle::NORMAL);
		ScreenHandle::outln(to_string(this->gitFiles.at(i).compareHashes.size()), ScreenHandle::YELLOW);

		if (this->gitFiles.at(i).compareHashes.size() > this->gitFiles.at(i).currentHashes.size()) {
			ScreenHandle::outln("\t\t! Need cherrypick from " + this->checkBranch, ScreenHandle::RED);

		}

		this->gitFiles.at(i).diffs.clear();

		// build diff by the committed hashes
		for (int t = 0; t < this->gitFiles.at(i).currentHashes.size(); t++) {
			// look if this hash part of the missing commits
			bool missingCommit = this->CherryList.missingHash(this->gitFiles.at(i).currentHashes.at(t));

			if (missingCommit == true) {
				bool found = false;

				for (int m = 0; m < this->gitFiles.at(i).compareHashes.size(); m++) {
					if (this->gitFiles.at(i).compareHashes.at(m) == this->gitFiles.at(i).currentHashes.at(t)) {

						found = true;
					}
				}
				if (found == false) {

					TCommitInfo info = Commit.getCommitInfo(this->gitFiles.at(i).currentHashes.at(t));
					if (info.valid) {
						if (this->includedAuthor.size() < 1 || findAuthor(info.Author)) {
							this->gitFiles.at(i).pickThis = true;

							applyDiffHash(this->gitFiles.at(i).currentHashes.at(t));


							ScreenHandle::out("\t\tmissing       ", ScreenHandle::NORMAL);
							ScreenHandle::outln(this->gitFiles.at(i).currentHashes.at(t), ScreenHandle::YELLOW);
							ScreenHandle::out("\t\tAuthor        ", ScreenHandle::NORMAL);
							ScreenHandle::outln(info.Author, ScreenHandle::WHITE);
							ScreenHandle::out("\t\tTopic         ", ScreenHandle::NORMAL);
							ScreenHandle::outln(info.Topic, ScreenHandle::WHITE);
							ScreenHandle::out("\t\tDate          ", ScreenHandle::NORMAL);
							ScreenHandle::outln(info.Date, ScreenHandle::WHITE);
							ScreenHandle::out("\t\tUsage         ", ScreenHandle::NORMAL);
							ScreenHandle::outln(to_string(info.hitCount), ScreenHandle::WHITE);

							this->gitFiles.at(i).diffs.push_back(this->gitFiles.at(i).currentHashes.at(t));
							vector<string> filesIncluded = this->getAffectedFilesByHash(this->gitFiles.at(i).currentHashes.at(t), this->gitFiles.at(i).Filename);

							int pos = this->CherryList.hashPosition(this->gitFiles.at(i).currentHashes.at(t));
							this->gitFiles.at(i).positionInDiff = pos;
							ScreenHandle::out("\t\tPosition     ", ScreenHandle::NORMAL);
							ScreenHandle::outln(to_string(pos), ScreenHandle::WHITE);

							ScreenHandle::out("\t\t\taffected Files", ScreenHandle::BROWN);
							ScreenHandle::outln(to_string(filesIncluded.size()), ScreenHandle::WHITE);
							ScreenHandle::outln(" ", ScreenHandle::NORMAL);
							if (showAffectedFiles) {
								for (int inc = 0; inc < filesIncluded.size(); inc++) {
									//ScreenHandle::outln("\t\t" + filesIncluded.at(inc), ScreenHandle::MAGENTA);


									int nr = this->getNumberByFilename(filesIncluded.at(inc));
									if (nr < 0) {
										string oldDir;
										ScreenHandle::outln("\t\t" + filesIncluded.at(inc), ScreenHandle::MAGENTA);
										ScreenHandle::outln("\t\tthis file is not inluded in cherrylist", ScreenHandle::YELLOW);
										ScreenHandle::outln("\t\t try to get missing commits", ScreenHandle::YELLOW);
										ScreenHandle::out("\t\t", ScreenHandle::NORMAL);
										oldDir = this->basedir;
										this->basedir = filesIncluded.at(inc);
										this->getDiffFiles();
										this->basedir = oldDir;
									}

								}
							}

						}
					} else {
						cout << "\t\tinvalid commit info " << endl;
					}

				}
			}

		}

	}


}

void GetFileDiff::cherryPickMarked() {
	for (int i = 0; i < this->gitFiles.size(); i++) {
		if (this->gitFiles.at(i).marked == true) {
			//this->cherryPick(i);
		}
	}
}

void GetFileDiff::cherryPick(int number) {
	if (number >= 0 and number < this->gitFiles.size()) {

		if (!this->gitFiles.at(number).pickThis && this->gitFiles.at(number).diffs.empty()) {
			GitHandler check;
			cout << "exec:" << "git checkout " << this->checkBranch << endl;
			check.execute("git checkout " + this->checkBranch);
			cout << "done" << endl;
			for (int i = 0; i < this->gitFiles.at(number).diffs.size(); i++) {
				string hash = this->gitFiles.at(number).diffs.at(i);
				cout << "cherrypick " << hash << endl;


				vector<string> output = check.execute("git cherry-pick " + hash);

				for (int g = 0; g < output.size(); g++) {
					cout << output.at(g) << endl;
				}
			}

			check.execute("git checkout " + this->currentBranch);
		} else {
			cout << "this file have no diffs" << endl;
		}


	}
}

void GetFileDiff::cherryPickAllMarked() {
	string hash = this->getMarkedCherryHashes();
	if (hash != "") {
		ScreenHandle::outln("pick " + hash, ScreenHandle::LIGHT_BLUE);
		GitHandler check;

		check.execute("git checkout " + this->checkBranch);
		vector<string> output = check.execute("git cherry-pick " + hash);

		for (int g = 0; g < output.size(); g++) {
			ScreenHandle::outln(output.at(g), ScreenHandle::LIGHT_GREEN);
		}
		check.execute("git checkout " + this->currentBranch);
	}
}

void GetFileDiff::printCherryPick(int number) {
	if (number >= 0 and number < this->gitFiles.size()) {

		const auto& files = this->gitFiles.at(number);
		if (!files.pickThis && files.diffs.empty()) {
			cout << "git checkout " << this->checkBranch << endl;

			for (const string& hash : files.diffs) {
				cout << "git cherry-pick " << hash << endl;
			}

			cout << "git checkout " << this->currentBranch << endl;
		} else {
			cout << "this file have no diffs" << endl;
		}


	}
}

string GetFileDiff::getMarkedCherryHashes() {
	vector<int> picRanges;
	string returnHash = "";
	for (int i = 0; i < this->gitFiles.size(); i++) {
		if (this->gitFiles.at(i).marked == true) {
			bool addthis = true;
			for (int m = 0; m < picRanges.size() and addthis; m++) {
				if (picRanges.at(m) == this->gitFiles.at(i).positionInDiff) {
					addthis = false;
				}
			}
			if (addthis) {
				picRanges.push_back(this->gitFiles.at(i).positionInDiff);
			}
		}
	}
	std::sort(picRanges.begin(), picRanges.end());

	for (int i = 0; i < picRanges.size(); i++) {
		//returnHash += " " + this->CherryList.getCherryList().at(picRanges.at(i));
		string line = this->CherryList.getCherryList().at(picRanges.at(i));
		returnHash += " " + this->CherryList.getHashFromLine(line);
	}


	return returnHash;
}

void GetFileDiff::runCherryForMarked() {

	GitHandler check;
	bool run = true;
	bool justShow = !this->executeCm;

	if (!justShow) check.execute("git checkout " + this->checkBranch);
	else ScreenHandle::outln("we are right now in the preview mode. No Cherrypicking executed", ScreenHandle::BOLD);


	for (int i = 0; i < this->picPositions.size(); i++) {
		int picInCherry = picPositions.at(i);
		if (picInCherry <= this->picUntilTo) {
			if (picInCherry > this->CherryList.getCherryList().size() || picInCherry < 0) {
				ScreenHandle::out("ERROR ", ScreenHandle::LIGHT_RED);
				ScreenHandle::outln("Invalid Cherrypick Infomation. you have to execute h (get missing commits) first", ScreenHandle::RED);
				return;
			}
			string cHash = this->CherryList.getCherryList().at(picInCherry);
			string realHash = this->CherryList.getHashFromLine(cHash);
			if (!justShow) ScreenHandle::outln(cHash, ScreenHandle::LIGHT_BLUE);
			string cmd = "git cherry-pick " + realHash;
			if (run && !justShow) {
				vector<string> output = check.execute(cmd);

				for (int g = 0; g < output.size(); g++) {
					cout << output.at(g) << endl;
				}
			} else if (justShow) {
				ScreenHandle::outln(cmd, ScreenHandle::NORMAL);
			}
		}
	}
	if (!justShow) check.execute("git checkout " + this->currentBranch);


}

void GetFileDiff::markAll() {
	for (int i = 0; i < this->picPositions.size(); i++) {
		if (this->picPositions.at(i) > this->picUntilTo) {
			this->picUntilTo = this->picPositions.at(i);
		}
	}
}

void GetFileDiff::markForCherryPick(int number) {
	this->picUntilTo = number;
}

int GetFileDiff::getNumberByFilename(string filename) {
	for (int i = 0; i < this->gitFiles.size(); i++) {
		if (this->gitFiles.at(i).Filename == filename) {
			return i;
		}
	}
	return -1;
}

vector<string> GetFileDiff::getDiffHashes(string filename, string branch) {


	string cmd = "git rev-list --reverse " + branch + " -- " + filename;

	vector<string> gitHashes;
	gitHashes = GitExex.execute(cmd);

	/*
	for (int i =0; i < gitHashes.size(); i++){
		cout << "\t" << gitHashes.at(i) << endl;
	}*/
	return gitHashes;

}

void GetFileDiff::getDiffFiles() {
	string showCmd = "git diff --name-status "
			+ this->checkBranch + ".."
			+ this->currentBranch;
	if (this->basedir != "") {
		showCmd += " | grep " + this->basedir;
	}

	int cdRes = chdir(this->workingDirectory.c_str());

	if (this->CherryList.size() < 1
			|| this->CherryList.getBaseBranch() != this->currentBranch
			|| this->checkBranch != this->CherryList.getCompareBranch()) {

		ScreenHandle::out("get cherry list..please wait ", ScreenHandle::BOLD);
		this->CherryList.read();
		ScreenHandle::out("...found " + std::to_string(this->CherryList.size()) + " unpicked commits ", ScreenHandle::BOLD);
		ScreenHandle::outln("", ScreenHandle::NORMAL);

	}

	vector<string> changedFiles;


	changedFiles = GitExex.execute(showCmd);

	StringWorker StringHelper;

	int addedFiles = 0;

	for (int i = 0; i < changedFiles.size(); i++) {
		string entry = changedFiles.at(i);
		vector<string> filepart;
		filepart = StringHelper.split(entry, "\t");

		//cout << "GOT: " << entry << " " << filepart.size() << endl;
		if (filepart.size() == 2 && this->fileExists(filepart.at(1)) == false) {
			TGitFile addMe;
			addMe.Filename = filepart.at(1);
			addMe.state = filepart.at(0);
			addMe.marked = false;
			addMe.positionInDiff = -1;
			this->gitFiles.push_back(addMe);
			addedFiles++;
		}
	}

	//cout << "diff files found: " << addedFiles << endl;
	if (addedFiles > 0) {
		ScreenHandle::out("diffs found in ", ScreenHandle::BOLD);
		ScreenHandle::out(to_string(addedFiles), ScreenHandle::YELLOW);
		ScreenHandle::out(" Files", ScreenHandle::BOLD);
	} else {
		ScreenHandle::out("No diffs found", ScreenHandle::LIGHT_GREEN);
	}
	ScreenHandle::outln("", ScreenHandle::NORMAL);

}

string GetFileDiff::getCurrentGitBranch() {
	int cdRes = chdir(this->workingDirectory.c_str());
	return this->GitExex.getCurrentBranch();
}