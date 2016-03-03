/*
 * File:   ScreenHandle.cpp
 * Author: tziegler
 *
 * Created on 24. November 2015, 10:44
 */

#include "ScreenHandle.h"

#define   colorNormal()			printf("%c[0;0m",ESC)

namespace ScreenHandle {




	TermHandle Screen;
	bool simpleMode = true;
	bool colorMode = true;

	void enableColor(bool onoff){
		colorMode = onoff;
	}

	void out(string message, string colorCode){
		if (colorMode == true){
			printf("%c%s%s",ESC,colorCode.c_str(),message.c_str());
			//printf("%s",message.c_str());
			colorNormal();
		} else {
			printf("%s",message.c_str());

		}
	}
	void outln(string message, string colorCode){
		if (colorMode == true){
			printf("%c%s",ESC,colorCode.c_str());
			printf("%s\n",message.c_str());
			colorNormal();
		} else {
			printf("%s\n",message.c_str());

		}
	}

	void echo(string message, int left, int top, string color){
		printf("%c%s%s",ESC,color.c_str(),message.c_str());
	}


}
