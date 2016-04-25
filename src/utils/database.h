#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "includes/ompl.h"
#include "commons.h"
#include "iostream"
#include "fstream"
#include "sstream"

using namespace std;
using namespace common;

class Database{
private:

public:
	Database();
	void savePath(string path, ob::PathPtr pathPtr);
	void transform3in2();
	Workspace loadWorkspace(string path = "data/workspaces/default");
	void makeRandomWorkspace();
};

#endif