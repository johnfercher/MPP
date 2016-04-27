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

#ifndef _MPP_H_
#define _MPP_H_

#include "iostream"
#include "sstream"
#include "thread"
#include "utils/includes/boost.h"
#include "utils/includes/ompl.h"
#include "utils/commons.h"
#include "utils/net.h"

#include "control/goodrich.h"
#include "control/pid.h"
#include "offline_planning.h"
#include "pthread.h"

using namespace std;

class MPP{
private:
	Draw draw;
	Database db;
	Offline_planning offline;
	Net net;
	Goodrich goodrich;
	PID pid;

	thread *thread_draw;
	thread *thread_net;
	thread *thread_simulation;

	vector<Path> offlinePaths;
	vector<Path> runtimePaths;
	vector<int> idDone;
	
	Workspace workspace;
	grSim_Packet packet_grSim;
	Command command;

	bool simulation, grsim;
	string pathWorkspace;

	int argc;
	char **argv;

	void draw_thread();
	void net_thread();
	void simulation_thread();
	void init_threads();
	void finalize_threads();

	Path PathPrtToPath(ob::PathPtr);
	Workspace packetToWorkspace(SSL_WrapperPacket);
	Pose handlePosition(Pose poseGR);
	Object handlePosition(Object objectGR);	
	void findPaths();

public:
	MPP(int argc, char** argv);

	void init();
	void simulation_on();
	void grsim_on();
	Pose sum(Pose, Pose);
	float angulation(Pose a, Pose b);
	float distance(Pose a, Pose b);
};

#endif // _MPP_H_