#include "mpp.h"

MPP::MPP(int argc, char** argv){
	this->argc = argc;
	this->argv = argv;

	simulation_on();
	pathWorkspace = "src/data/workspaces/random7";
}

void MPP::init(){
	findPaths();
	
	init_threads();
	finalize_threads();
}

void MPP::init_threads(){
	if(simulation){
		thread_draw = new thread(bind(&MPP::draw_thread, this));
		thread_simulation = new thread(bind(&MPP::simulation_thread, this));
	}else{
		thread_draw = new thread(bind(&MPP::draw_thread, this));
		thread_net = new thread(bind(&MPP::net_thread, this));
	}
}

void MPP::finalize_threads(){
	if(simulation){
		thread_draw->join();
		thread_simulation->join();
	}else{
		thread_draw->join();
		thread_net->join();
	}
}

void MPP::findPaths(){
	if(simulation){
		workspace = db.loadWorkspace(pathWorkspace);
		workspace.show();
	}else{
		net.setIpPortReceive();   
	    usleep(1000000);

	    while(net.hasNewPacket()){
	        workspace = packetToWorkspace(net.receivePacket());
	    }
	}
	
    offline.setBounds(0.0, 1000.0);        // (0, 0) -> (640, 640)
    offline.setTimeToResolve(1);           // 1 segundo no máximo para offline o problema
    offline.setPlanner(PLANNER_RRTSTAR);
    offline.setWorkspace(&workspace);

    paths = offline.solvePaths();
}

void MPP::simulation_on(){
	simulation = true;
	grsim = false;
}

void MPP::grsim_on(){
	simulation = false;
	grsim = true;
}

void MPP::draw_thread(){
	draw.setPaths(paths);
    draw.allocateRuntimePaths(&runtimePaths);
    draw.setObjects(&workspace.objects);
    draw.setSize(1000.0, 1000.0); // pode dar problema
    draw.init(argc, argv);
}

void MPP::net_thread(){

}

void MPP::simulation_thread(){
    goodrich.setPaths(&paths);
    goodrich.setWorkspace(&workspace);
    goodrich.allocateRuntimePaths(&runtimePaths);
    goodrich.init();
}

Workspace MPP::packetToWorkspace(SSL_WrapperPacket){

}

Pose MPP::handlePosition(Pose poseGR){

}

Object MPP::handlePosition(Object objectGR){

}