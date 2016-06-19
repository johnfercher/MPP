/*The MIT License (MIT)

Copyright (c) 2016 Johnathan Fercher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

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
	float distance(Pose a, Pose b);
};

#endif