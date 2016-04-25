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

#include "goodrich.h"

Goodrich::Goodrich(){
	alpha = 0.2;
	beta = 0.4; //0.4
	INF = 0.4; //20
	radiusRobot = 15;
	areaRobot = 20;
	done = false;
}

int Goodrich::sign(float signal){
	if(signal < 0){
		return -1;
	}else if(signal == 0){
		return 0;
	}else{
		return 1;
	}
}

void Goodrich::moveObstacles(){
	int helperTime = 0;
	srand(time(NULL)+helperTime);

	for(int i = 0 ; i < workspace->objects.size() ; i++){
		int moveX, moveY;
		int direX, direY;

		if(rand() % 2 == 0)	direX = 1;
		else				direX = -1;

		helperTime++;

		if(rand() % 2 == 0)	direY = 1;
		else				direY = -1;

		helperTime++;

		workspace->objects.at(i).x += (rand()%5)*direX;
		workspace->objects.at(i).y += (rand()%5)*direY;
	}
}

void Goodrich::init(){
	for(int i = 0 ; i < paths->size() ; i++){
		RuntimePath pathRun;
		idDones.push_back(0);
		dists.push_back(999999);
		robots.push_back(workspace->start.at(i));
		pathRun.path.push_back(robots.at(i));
		runtimePaths->push_back(pathRun);
	}

	//runtimePaths->at(0).path.at(0).show();
	//distance(
		//Pose p1 = runtimePaths->at(0).path.at(runtimePaths->at(0).path.size()-1);
		//Pose p2 = runtimePaths->at(0).path.at(idDones.at(0));
		//);
	//distance(p1, p2);
		//p1.show();
		//p2.show();
	//runtimePaths->at(0).path.size();
	usleep(10000000);
	while(!done){
		moveObstacles();
		attractiveForce();
		repulsiveForceRobotRobot();
		repulsiveForceRobotObjects();
		usleep(50000);
	}
}

void Goodrich::attractiveForce(){
	double theta;
	double distances;

	for(int i = 0 ; i < paths->size() ; i++){
		float x, y, yaw;
		vector<ob::State*> path = boost::static_pointer_cast<og::PathGeometric>(paths->at(i))->getStates();
		
		//cout << "a" << idDones.at(i) << endl;
		ob::SE2StateSpace::StateType* state2D = path.at(idDones.at(i))->as<ob::SE2StateSpace::StateType>();
		//cout << "b" << idDones.at(i) << endl;

		x = state2D->getX();
		y = state2D->getY();
		yaw = state2D->getYaw();
	
		Pose p(x, y, yaw);

		Pose robotAct = robots.at(i);
		//robotAct.show();

		theta = angulation(robotAct, p);
		distances = distance(robotAct, p);

		if(distances < 20.0 && idDones.at(i) < path.size()-1){
			idDones.at(i) += 1;
			//cout << idDones.at(i) << endl;
			//cout << path.size()-1 << endl;
			
			if(idDones.at(i) == path.size()){
				done = true;
			}

			state2D = path.at(idDones.at(i))->as<ob::SE2StateSpace::StateType>();

			p.x = state2D->getX();
			p.y = state2D->getY();
			p.yaw = state2D->getYaw();
		
			theta = angulation(robotAct, p);
			distances = distance(robotAct, p);
		}	

		if(distances < radiusRobot){
			robotAct.x += 0;
			robotAct.y += 0;
		}
		else if(distances <= (radiusRobot + areaRobot)){
			robotAct.x += -alpha*(distances - radiusRobot)*cos(theta/180.0*M_PI); 
			robotAct.y += -alpha*(distances - radiusRobot)*sin(theta/180.0*M_PI);
		}else{
			robotAct.x += -alpha*areaRobot*cos(theta/180.0*M_PI); 
			robotAct.y += -alpha*areaRobot*sin(theta/180.0*M_PI);
		}

		robots.at(i) = robotAct; 	
		runtimePaths->at(i).path.push_back(robotAct);
	}
}


void Goodrich::setPaths(vector<ob::PathPtr> *paths){
	this->paths = paths;
}

void Goodrich::setWorkspace(Workspace *workspace){
	this->workspace = workspace;
}

void Goodrich::allocateRuntimePaths(vector<RuntimePath> *runtimePaths){
	this->runtimePaths = runtimePaths;
}

void Goodrich::repulsiveForceRobotRobot(){
	double theta;
	double distances;
	int k;

	for(int i = 0 ; i < robots.size() ; i++){
		for(int j = 0 ; j < robots.size() ; j++){
			if(i != j){
				theta = angulation(robots.at(i), robots.at(j));
				distances = distance(robots.at(i), robots.at(j));

				if(distances <= radiusRobot){
					//se esta escostado no obstaculo, recebe um vetor maximo 
					robots.at(i).x += sign(cos(theta))*INF;
					robots.at(i).y += sign(sin(theta))*INF;

				}else if(distances <= (radiusRobot + areaRobot)){
					//se esta dentro da area de influencia
					robots.at(i).x += beta*(areaRobot + radiusRobot - distances)*cos(theta/180.0*M_PI); 
					robots.at(i).y += beta*(areaRobot + radiusRobot - distances)*sin(theta/180.0*M_PI);

					robots.at(i).x += rand()%3;
					k++;
					robots.at(i).y += rand()%3;
					k++;	
				}
			}
		}
	}
}

void Goodrich::repulsiveForceRobotObjects(){
	double theta;
	double distances;
	int k;
	
	for(int i = 0 ; i < robots.size() ; i++){
		for(int j = 0 ; j < workspace->objects.size() ; j++){
			theta = angulation(robots.at(i), Pose(workspace->objects.at(j).x, workspace->objects.at(j).y, 0));
			distances = distance(robots.at(i), Pose(workspace->objects.at(j).x, workspace->objects.at(j).y, 0));

			if(distances <= workspace->objects.at(j).radius){
				//se esta escostado no obstaculo, recebe um vetor maximo 
				robots.at(i).x += sign(cos(theta))*INF;
				robots.at(i).y += sign(sin(theta))*INF;

			}else if(distances <= (workspace->objects.at(j).radius + areaRobot)){
				//se esta dentro da area de influencia
				robots.at(i).x += beta*(areaRobot + workspace->objects.at(j).radius - distances)*cos(theta/180.0*M_PI); 
				robots.at(i).y += beta*(areaRobot + workspace->objects.at(j).radius - distances)*sin(theta/180.0*M_PI);

				robots.at(i).x += rand()%3;
				k++;
				robots.at(i).y += rand()%3;
				k++;	
			}	
		}
	}
}

float Goodrich::angulation(Pose a, Pose b){
	return (atan2(a.y - b.y, a.x - b.x) * (180/M_PI));
}

float Goodrich::distance(Pose a, Pose b){
	return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}