/*
 * FileUtil.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "FileUtil.h"

#include <stdio.h>
#include "../ARK2D.h"

#ifdef ARK2D_WINDOWS
	#include <direct.h>
	#include <windows.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#else
	#include <unistd.h>
	#define GetCurrentDirectoryMacro getcwd
	#define DIRECTORY_SEPARATOR "/"
#endif

bool FileUtil::file_put_contents(string filename, string contents) {
	// yarp
	fstream File;
	File.open(filename.c_str(), ios::out);
	if (File.is_open()) {
		File << contents;
		File.close();
		return true;
	}
	return false;
}

string FileUtil::getCurrentDirectory() {
	char currentPath[FILENAME_MAX];
	if(!GetCurrentDirectoryMacro(currentPath, sizeof(currentPath))) {
		return "";
	}
	currentPath[sizeof(currentPath)-1] = '\0';
	return string(currentPath);
}
string FileUtil::getSeparator() {
	return DIRECTORY_SEPARATOR;
}

string FileUtil::getOSDirectory() {
	#if defined(ARK2D_WINDOWS)
		char dir[MAX_PATH];
		dir[MAX_PATH-1] = '\0';
		GetWindowsDirectory((LPSTR) &dir, MAX_PATH);

		// never add a backslash.
		// if windows is installed to C:\ then it returns C:
		string ret(dir);
		if (ret.substr(ret.size()-1, 1) == DIRECTORY_SEPARATOR) {
			ret = ret.substr(0, ret.size()-1);
		}
		return ret;

	#elif defined(ARK2D_UBUNTU_LINUX)

	#endif
	return "whoops?";
}