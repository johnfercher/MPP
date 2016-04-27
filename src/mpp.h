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

	vector<ob::PathPtr> paths;
	vector<RuntimePath> runtimePaths;
	
	Workspace workspace;
	grSim_Packet packet_grSim;
	Command command;

	bool simulation, grsim;
	string pathWorkspace;

	int idDone;
	int argc;
	char **argv;

	void draw_thread();
	void net_thread();
	void simulation_thread();
	void init_threads();
	void finalize_threads();

public:
	MPP(int argc, char** argv);

	void init();
	void simulation_on();
	void grsim_on();
	void findPaths();

	Workspace packetToWorkspace(SSL_WrapperPacket);
	Pose handlePosition(Pose poseGR);
	Object handlePosition(Object objectGR);	
};

#endif // _MPP_H_