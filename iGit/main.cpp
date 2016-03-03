/*
 * File:   main.cpp
 * Author: tziegler
 *
 * Created on 5. November 2015, 12:24
 */

#include <cstdlib>
#include "GetFileDiff.h"
#include <unistd.h>
#include <X11/Xlib.h>
#include "MenuHandle.h"
#include "TermInfo.h"
#include "TermHandle.h"
#include "ScreenHandle.h"

using namespace std;

void splash() {
	cout << "======================" << endl;
	cout << "Git Cherrypicking Tool" << endl;
	cout << "======================" << endl;

}

/*
 *
 */
int main(int argc, char** argv) {
	//string path = "/home/tziegler/Remote/seafight";

	string applyCommands;
	string add = "";
	Bool argumentsGot = false;
	for (int i = 1; i < argc; i++) {
		string str(argv[i]);
		applyCommands += add + str;
		add = ";";
		argumentsGot = true;
	}

	splash();

	string path = "/home/tziegler/Bigpoint/seafight_Lokal";

	string currentBranch = "master";

	string compareToBrach = "release/release_16.3.1";

	string basePath = "code/presentation/website/";

	TermInfo Terminal;

	string pushCommand = "P";
	string pullCommand = "R";
	string cherryListSchowCommand = "L";
	string markForCherry = "m";

	ScreenHandle::enableColor(false);

	MenuHandle Menu;

	GetFileDiff GitWorker;
	GitWorker.setWorkingDir(path);
	GitWorker.setCheckBranch(compareToBrach);

	Menu.addMenu("change Working directory [" + path + "]", "w");
	Menu.addMenu("Current Branch " + currentBranch, "o");
	Menu.addMenu("Compare Branch " + compareToBrach, "n");
	Menu.addMenu("GIT pull rebase (booth)", pullCommand);
	Menu.addMenu("GIT push (Target)", pushCommand);
	Menu.addMenu("add Author ", "u");
	Menu.addMenu("set Filter", "b");
	Menu.addMenu("...Clear Difflist", "clear");
	Menu.addMenu("...Show Diff Files", "l");
	Menu.addMenu("...Show Cherry List", cherryListSchowCommand);
	Menu.addMenu("...Get Missing Commits", "h");
	Menu.addMenu("...Get Missing and affected", "hh");
	Menu.addMenu("...Mark for CherryPick", markForCherry);
	Menu.addMenu("...Mark All for CherryPick", "ma");
	Menu.addMenu("...Cherrypick", "C");
	Menu.addMenu("...Just Show Cherrypick", "c");
	Menu.addMenu("...Cherrypick marked", "cm");
	Menu.addMenu("Start Reading", "r");
	Menu.addMenu("color on", "coloron");
	Menu.addMenu("color off", "coloroff");


	Menu.setPromot(GitWorker.getCurrentGitBranch());

	Menu.setMenuVisibile("l", false);
	Menu.setMenuVisibile("L", false);
	Menu.setMenuVisibile("h", false);
	Menu.setMenuVisibile("hh", false);
	Menu.setMenuVisibile("c", false);
	Menu.setMenuVisibile("C", false);
	Menu.setMenuVisibile("cm", false);
	Menu.setMenuVisibile("ma", false);
	Menu.setMenuVisibile("m", false);
	Menu.setMenuVisibile("clear", false);

	if (argumentsGot) {
		Menu.setCommandString(applyCommands);
	}


	while (Menu.notDone()) {


		Menu.setPromot(GitWorker.getCurrentGitBranch());
		string option = Menu.showMenu();

		if (option == "coloron") {
			ScreenHandle::enableColor(true);
		}

		if (option == "coloroff") {
			ScreenHandle::enableColor(false);
		}

		if (option == "clear") {
			GitWorker.clear();
		}

		if (option == "ma") {
			GitWorker.markAll();
		}

		if (option == "cm") {
			GitWorker.runCherryForMarked();
		}

		if (option == "r") {
			GitWorker.getDiffFiles();

		}

		if (option == cherryListSchowCommand) {
			GitWorker.showCherryList();
		}

		if (option == markForCherry) {
			GitWorker.printMissingDiffFiles();
			string number = Menu.getInputString("Insert Number of file:");
			int nr = std::stoi(number);
			GitWorker.markForCherryPick(nr);
		}

		if (option == pullCommand) {
			GitWorker.pullRebase();
		}

		if (option == pushCommand) {
			GitWorker.pushTartget();
		}

		if (option == "l") {
			GitWorker.printDiffFiles();
		}

		if (option == "u") {
			GitWorker.addAuthor();
			GitWorker.getHashes();
			Menu.updateMenu("u", "(" + GitWorker.GetAuthorsAsOnlineString() + ") add Author");

		}

		if (option == "h") {
			GitWorker.getHashes();
			Menu.setMenuVisibile("c", true);
			Menu.setMenuVisibile("C", true);
		}
		if (option == "hh") {
			GitWorker.getHashes(true);
			Menu.setMenuVisibile("c", true);
			Menu.setMenuVisibile("C", true);
		}

		if (option == "C") {
			GitWorker.printMissingDiffFiles();
			string number = Menu.getInputString("Insert Number of file:");
			int nr = std::stoi(number);
			GitWorker.cherryPick(nr);
			GitWorker.clear();
			GitWorker.getDiffFiles();
			GitWorker.getHashes();


			//GitWorker.printCherryPick(nr);
			/*
			Menu.setMenuVisibile("l", false);
			Menu.setMenuVisibile("h", false);
			Menu.setMenuVisibile("c", false);
			 */
		}

		if (option == "c") {
			GitWorker.printMissingDiffFiles();
			string number = Menu.getInputString("Insert Number of file:");
			int nr = std::stoi(number);
			GitWorker.printCherryPick(nr);

		}


		if (option == "w") {
			GitWorker.clear();
			GitWorker.setWorkingDir(Menu.getInputString("working on path"));
			Menu.updateMenu(option, "Working Path is: " + GitWorker.getWorkingDir());
			GitWorker.getDiffFiles();
		}

		if (option == "b") {
			GitWorker.setBasePath(Menu.getInputString("set the base path"));
			Menu.updateMenu(option, "filter: " + GitWorker.getBasePath());
			GitWorker.getDiffFiles();
		}

		if (option == "o") {
			GitWorker.clear();
			GitWorker.setCurrentBranch(Menu.getInputString("set current Branch"));
			Menu.updateMenu(option, "Current Branch: " + GitWorker.getCurrentBranch());
			GitWorker.getDiffFiles();
		}

		if (option == "n") {
			GitWorker.clear();
			GitWorker.setCheckBranch(Menu.getInputString("set Branch for compare"));
			Menu.updateMenu(option, "Compare Branch: " + GitWorker.getCheckBranch());
			GitWorker.getDiffFiles();
		}


		// display options
		if (GitWorker.size() > 0) {
			Menu.setMenuVisibile("l", true);
			Menu.setMenuVisibile("L", true);
			Menu.setMenuVisibile("h", true);
			Menu.setMenuVisibile("hh", true);
			Menu.setMenuVisibile("cm", true);
			Menu.setMenuVisibile("ma", true);
			Menu.setMenuVisibile("clear", true);

		} else {
			Menu.setMenuVisibile("ma", false);
			Menu.setMenuVisibile("cm", false);
			Menu.setMenuVisibile("l", false);
			Menu.setMenuVisibile("L", false);
			Menu.setMenuVisibile("h", false);
			Menu.setMenuVisibile("hh", false);
			Menu.setMenuVisibile("c", false);
			Menu.setMenuVisibile("C", false);
			Menu.setMenuVisibile("clear", false);
		}

	}




	return 0;
}

