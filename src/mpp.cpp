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

	finish = false;
	simulation_on();
	//grsim_on();
	
	stringstream secure;
	secure << argv[1];
	
	report_name_of_workspace = secure.str();
	cout << report_name_of_workspace << endl;
	report_num_robots = report_num_obstacles = report_num_collisions = report_num_steps = 0;
	report_success = false;

	stringstream ss;
	ss << "src/data/workspaces/" << report_name_of_workspace;

	pathWorkspace = ss.str();;
}

void MPP::init(){
	findPaths();

	goodrich.setWorkspace(&workspace);
	report_num_robots = workspace.start.size()+1;
	report_num_obstacles = workspace.objects.size()+1;

	for(int i = 0 ; i < workspace.start.size() ; i++){
		done.push_back(false);
	}

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
		thread_simulation = new thread(bind(&MPP::simulation_thread, this));
	}else{
		thread_net = new thread(bind(&MPP::net_thread, this));
	}

	//thread_draw = new thread(bind(&MPP::draw_thread, this));
}

void MPP::finalize_threads(){
	if(simulation){
		thread_simulation->join();	
	}else{
		thread_net->join();
	}
	//thread_draw->join();
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
    draw.allocateFinish(&finish);
    draw.init(argc, argv);
}

void MPP::net_thread(){
    Net net2;
    PID pid;

    net2.setIpPortSend();        // with default values
    net2.setIpPortReceive();     // with default values

    while(1){
        if(net2.hasNewPacket()){
            Command cmd[5];

        	Workspace resp = packetToWorkspace(net2.receivePacket()); 
        	workspace.objects = resp.objects;
        	workspace.goal = resp.goal;

        	for(int i = 0 ; i < resp.start.size() ; i++){
        		runtimePaths.at(i).path.push_back(resp.start.at(i));
        	}

        	//cout << "MPP: " << runtimePaths.size() << endl;
        	for(int i = 0 ; i < runtimePaths.size() ; i++){
        		//cout << i << ": " << runtimePaths.at(i).path.size() << endl;
	    		Pose result;
	    		float dist = distance(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), offlinePaths.at(i).path.at(idDone.at(i)));

	    		if(dist > 20){
	    			if(idDone.at(i) < offlinePaths.at(i).path.size()-1){
	    				result = goodrich.calcResult(i, offlinePaths.at(i).path.at(idDone.at(i)), false);
	    			}else{
	    				result = goodrich.calcResult(i, offlinePaths.at(i).path.at(idDone.at(i)), true);
	    			}
	    			Pose newPoint = sum(
			    				runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1),
			    				result
			    			);

	    			newPoint.yaw = offlinePaths.at(i).path.at(idDone.at(i)).yaw;

	    			cmd[i] = pid.calcCommand(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), newPoint);
	    			
	    			if(i == 0){
	    				cout << "result" << endl;
	    				result.show();
	    				cout << endl;

	    				cout << "newPoint" << endl;
	    				newPoint.show();
	    				cout << endl;

	    				cout << "runTime" << endl;
	    				runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1).show();
	    			}
	    		}else{
	    			if(idDone.at(i) < offlinePaths.at(i).path.size()-1){
	    				idDone.at(i)++;
	    			}

	    			cmd[i] = Command(0, 0, 0);
	    		}
	    	}

	    	packet_grSim = grSim_Packet();
            packet_grSim.mutable_commands()->set_isteamyellow(false);
            packet_grSim.mutable_commands()->set_timestamp(0.0);

            for(int i = 0 ; i < runtimePaths.size() ; i++){
            	//cout << "i::" << i << endl;
	            grSim_Robot_Command* command = packet_grSim.mutable_commands()->add_robot_commands();
	            command->set_id(i);

	            command->set_wheelsspeed(0);
	            command->set_wheel1(0);
	            command->set_wheel2(0);
	            command->set_wheel3(0);
	            command->set_wheel4(0);
	            command->set_veltangent(cmd[i].vel_tangent);
	            command->set_velnormal(cmd[i].vel_normal);
	            command->set_velangular(cmd[i].vel_angular);

	            command->set_kickspeedx(0);
	            command->set_kickspeedz(0);
	            command->set_spinner(true);
            }

            net2.sendPacket(packet_grSim);
        }
        //usleep(10000);
    }
}

void MPP::simulation_thread(){
    while(!finish){
    	moveObstacles();
    	for(int i = 0 ; i < runtimePaths.size() ; i++){
    		Pose result;
    		float dist = distance(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), offlinePaths.at(i).path.at(idDone.at(i)));

    		if(dist > 20){
    			if(idDone.at(i) < offlinePaths.at(i).path.size()-1){
    				result = goodrich.calcResult(i, offlinePaths.at(i).path.at(idDone.at(i)), false);
    			}else{
    				result = goodrich.calcResult(i, offlinePaths.at(i).path.at(idDone.at(i)), true);
    			}
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
    			}else
    			if(idDone.at(i) == offlinePaths.at(i).path.size()-1){
    				done.at(i) = true;
    			}
    			
    		}

    		//cout << "i: " << i << ", id: " << idDone.at(i) << ", size: " << offlinePaths.at(i).path.size() << endl;

    		for(int j = 0 ; j < workspace.objects.size() ; j++){
    			float dist = checkCollision( runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), workspace.objects.at(j) );
    			if(dist < 0.0){
    				bool repeated = false;
    				for(int k = 0; k < collisions.size() ; k++){
    					float dist_col = distance(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1), collisions.at(k));
    					if(dist_col < 100){
    						repeated = true;
    						//cout << "i: " << i << " col " << dist_col << endl;
    					}
    				}
    				if(!repeated){
    					collisions.push_back(runtimePaths.at(i).path.at(runtimePaths.at(i).path.size()-1));
	    				report_num_collisions++;
	    				//cout << "robot: " << i << " COLLISION: " << report_num_collisions << endl;
	    			}
	    		}
    		}
    	}

    	bool todos_chegaram = true;


    	for(int i = 0 ; i < done.size() ; i++){
    		if(!done.at(i)){
    			todos_chegaram = false;
    		}
    	}

    	if(todos_chegaram){
    		report_success = true;
    		finish = true;
    	}else
    	if(report_num_steps > 999){
    		finish = true;
    		report_success = false;
    	}

    	if(finish){
    		cout << endl;
    		cout << "Success: ";
		    if(report_success){
				cout << "TRUE" << endl;
			}else{
				cout << "FALSE" << endl;
			}

			cout << "Workspace: " << report_name_of_workspace << endl;
			cout << "Qtd_robots: " << report_num_robots << endl;
			cout << "Qtd_obstacles: " << report_num_obstacles << endl;
			cout << "Qtd_collisions: " << report_num_collisions << endl;
			cout << "Qtd_steps: " << report_num_steps << endl;
    	}

    	//usleep(40000);
    	report_num_steps++;
    	//cout << report_num_steps << endl;
    }
}

Workspace MPP::packetToWorkspace(SSL_WrapperPacket packet){
	Workspace ws;

    Pose start(packet.detection().robots_blue(0).x(), packet.detection().robots_blue(0).y(), packet.detection().robots_blue(0).orientation());
    Pose goal(packet.detection().balls(0).x(), packet.detection().balls(0).y(), 0);

    ws.start.push_back(handlePosition(start));
    ws.goal.push_back(handlePosition(goal));

    start = Pose(packet.detection().robots_blue(1).x(), packet.detection().robots_blue(1).y(), packet.detection().robots_blue(1).orientation());
    goal = Pose(0, 0, 0);

    ws.start.push_back(handlePosition(start));
    ws.goal.push_back(handlePosition(goal));

    for(int i = 2 ; i < 6 ; i++){ // i = 2
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

float MPP::checkCollision(Pose pose, Object obj){
	//return sqrt((pose.x-obj.x)*(pose.x-obj.x) + (pose.y-obj.y)*(pose.y-obj.y)) - obj.radius;
	return distance(pose, Pose(obj.x, obj.y, 0)) - obj.radius;
}

void MPP::moveObstacles(){
 	int helperTime = 0;
 	srand(time(NULL)+helperTime);
 
 	for(int i = 0 ; i < workspace.objects.size() ; i++){
 		int moveX, moveY;
 		int direX, direY;
 
 		if(rand() % 2 == 0)	direX = 1;
 		else				direX = -1;
 
 		helperTime++;
 
 		if(rand() % 2 == 0)	direY = 1;
 		else				direY = -1;
 
 		helperTime++;
 
 		workspace.objects.at(i).x += (rand()%5)*direX;
 		workspace.objects.at(i).y += (rand()%5)*direY;
 	}
 }