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

#ifndef GOODRICH_H_
#define GOODRICH_H_

#include "iostream"
#include "../utils/includes/ompl.h"
#include "../utils/commons.h"
#include "unistd.h"

using namespace common;

class Goodrich{
private:
	float alpha;
	float beta;
	float INF;
	float radiusRobot;
	float areaRobot;

	Workspace *workspace;
	vector<Path> *runtimePaths;
	Pose goal, result;
	int id;
	bool is_last;

	int sign(float signal);
	void attractiveForce();
	void repulsiveForceRobotRobot();
	void repulsiveForceRobotObjects();

public:
	Goodrich();

	Pose calcResult(int, Pose, bool);
	void setWorkspace(Workspace*);
	void setRuntimePaths(vector<Path>*);
	float angulation(Pose a, Pose b);
	float distance(Pose a, Pose b);
};

#endif