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

	vector<ob::PathPtr> *paths;
	vector<RuntimePath> *runtimePaths;
	vector<int> idDones, dists;
	vector<Pose> robots;
	Workspace *workspace;

	bool done;

	int sign(float signal);
	void attractiveForce();
	void repulsiveForceRobotRobot();
	void repulsiveForceRobotObjects();
	float angulation(Pose a, Pose b);
	float distance(Pose a, Pose b);

public:
	Goodrich();

	void init();
	void setPaths(vector<ob::PathPtr>*);
	void setWorkspace(Workspace*);
	void allocateRuntimePaths(vector<RuntimePath>*);
	void moveObstacles();
};

#endif