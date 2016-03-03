/*
 * File:   TermInfo.h
 * Author: tziegler
 *
 * Created on 16. November 2015, 11:45
 */

#ifndef TERMINFO_H
#define	TERMINFO_H

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>



class TermInfo {
public:
	TermInfo();
	TermInfo(const TermInfo& orig);
	virtual ~TermInfo();
	void ReadInfo();

	int width;
	int height;


private:
	struct winsize w;

};

#endif	/* TERMINFO_H */

