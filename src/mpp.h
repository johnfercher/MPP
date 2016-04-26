#ifndef _MPP_H_
#define _MPP_H_

#include "iostream"
#include "sstream"
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
	Net net;
	Goodrich goodrich;
	PID pid;

	pthread_t thread[4];

	vector<ob::PathPtr> paths;
	vector<RuntimePath> runtimePaths;
	
	Workspace workspace;
	grSim_Packet packet_grSim;
	Command command;

	int idDone;
	int argc;
	char** argv;
	
public:
	MPP();

	void init(int argc, char** argv);

	void *draw_thread(void *arg);
	void *exec_thread(void *arg);
	void *receive_thread(void *arg);
	void *send_thread(void *arg);

	Workspace packetToWorkspace(SSL_WrapperPacket);
	Pose handlePosition(Pose poseGR);
	Object handlePosition(Object objectGR);	
};

#define // _MPP_H_