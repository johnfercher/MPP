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

#include "mpp.h"

MPP::MPP(int argc, char** argv){
	this->argc = argc;
	this->argv = argv;

	simulation_on();
	//grsim_on(); // need grsim on
	pathWorkspace = "src/data/workspaces/random7";
}

void MPP::init(){
	findPaths();

	goodrich.setWorkspace(&workspace);

	for(int i = 0 ; i < workspace.start.size() ; i++){
		Path p;
		p.path.push_back(workspace.start.at(i));
		runtimePaths.push_back(p);

		idDone.push_back(1); // cause the robot start on position(0) of path and should go position(i)
	}

	goodrich.setRuntimePaths(&runtimePaths);

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
	}else{
		net.setIpPortReceive();   
	    usleep(1000000);

	    while(net.hasNewPacket()){
	        workspace = packetToWorkspace(net.receivePacket());
	    }
	}

	//workspace.show();
	
    offline.setBounds(0.0, 1000.0);        // (0, 0) -> (640, 640)
    offline.setTimeToResolve(1);           // 1 segundo no máximo para offline o problema
    offline.setPlanner(PLANNER_RRTSTAR);
    offline.setWorkspace(&workspace);

    vector<ob::PathPtr> paths;
    paths = offline.solvePaths();

    for(int i = 0 ; i < paths.size() ; i++){
    	offlinePaths.push_back(PathPrtToPath(paths.at(i)));
    }
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
	draw.setPaths(offlinePaths);
    draw.allocateRuntimePaths(&runtimePaths);
    draw.setObjects(&workspace.objects);
    draw.setSize(1000.0, 1000.0); // pode dar problema
    draw.init(argc, argv);
}

void MPP::net_thread(){

}

void MPP::simulation_thread(){
    while(1){
    	for(int i = 0 ; i < runtimePaths.size() ; i++){
    		Pose result;
    		float dist = distance(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), offlinePaths.at(i).path.at(idDone.at(i)));

    		if(dist > 20){
    			result = goodrich.calcResult(i, offlinePaths.at(i).path.at(idDone.at(i)));
    			//cout << "teste" << endl;
    			runtimePaths.at(i).path.push_back(
	    			sum(
	    				runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1),
	    				result
	    				)
	    			);
    		}else{
    			//cout << offlinePaths.at(i).path.size() << endl;
    			if(idDone.at(i) < offlinePaths.at(i).path.size()-1){
    				idDone.at(i)++;
    			}
    		}

    		usleep(25000);
    	}
    }
}

Workspace MPP::packetToWorkspace(SSL_WrapperPacket packet){
	Workspace ws;

    Pose start(packet.detection().robots_blue(0).x(), packet.detection().robots_blue(0).y(), packet.detection().robots_blue(0).orientation());
    Pose goal(packet.detection().balls(0).x(), packet.detection().balls(0).y(), packet.detection().robots_blue(0).orientation());

    ws.start.push_back(handlePosition(start));
    ws.goal.push_back(handlePosition(goal));

    for(int i = 1 ; i < 6 ; i++){ // i = 1
        Object object(packet.detection().robots_blue(i).x(), packet.detection().robots_blue(i).y(), 20);
        ws.objects.push_back(handlePosition(object));
    }

    for(int i = 0 ; i < 6 ; i++){ // i = 0
        Object object(packet.detection().robots_yellow(i).x(), packet.detection().robots_yellow(i).y(), 20);
        ws.objects.push_back(handlePosition(object));
    }

    //ws.show();

    return ws;
}

Pose MPP::handlePosition(Pose poseGR){
	Pose poseMPP;
    
    poseMPP.x = (poseGR.x + 3000)/6.0;
    poseMPP.y = (poseGR.y + 2000)/6.0;
    poseMPP.yaw = poseGR.yaw;

    return poseMPP;
}

Object MPP::handlePosition(Object objectGR){
	Object objectMPP;

    objectMPP.x = (objectGR.x + 3000)/6.0;
    objectMPP.y = (objectGR.y + 2000)/6.0;
    objectMPP.radius = objectGR.radius;

    return objectMPP;
}

Path MPP::PathPrtToPath(ob::PathPtr pathPtr){
	Path path;

	vector<ob::State*> states = boost::static_pointer_cast<og::PathGeometric>(pathPtr)->getStates();

	for(int i = 0 ; i < states.size() ; i++){
		ob::SE2StateSpace::StateType* state = states.at(i)->as<ob::SE2StateSpace::StateType>();
		Pose p(state->getX(), state->getY(), state->getYaw());
		path.path.push_back(p);
	}

	return path;
}

Pose MPP::sum(Pose a, Pose b){
	return Pose(a.x + b.x, a.y + b.y, a.yaw + b.yaw);
}

float MPP::angulation(Pose a, Pose b){
	return (atan2(a.y - b.y, a.x - b.x) * (180/M_PI));
}

float MPP::distance(Pose a, Pose b){
	return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}