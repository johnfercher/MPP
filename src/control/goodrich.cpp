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

Pose Goodrich::calcResult(int id, Pose goal){
	this->id = id;
	this->goal = goal;
	result = Pose(0, 0, 0);
	
	attractiveForce();
	repulsiveForceRobotObjects();
	repulsiveForceRobotRobot();

	return result;
}

void Goodrich::attractiveForce(){
	double theta, distances;

	theta = angulation(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size()-1), goal);
	distances = distance(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size()-1), goal);
	//cout << distances << endl;
	
	if(distances < radiusRobot){
		result.x += 0;
		result.y += 0;
	}
	else if(distances <= (radiusRobot + areaRobot)){
		result.x += -alpha*(distances - radiusRobot)*cos(theta/180.0*M_PI); 
		result.y += -alpha*(distances - radiusRobot)*sin(theta/180.0*M_PI);
	}else{
		result.x += -alpha*areaRobot*cos(theta/180.0*M_PI); 
		result.y += -alpha*areaRobot*sin(theta/180.0*M_PI);
	}

	//cout << "atrativo" << endl;
	//result.show();
}

void Goodrich::repulsiveForceRobotRobot(){
	double theta;
	double distances;
	int k;

	for(int j = 0 ; j < runtimePaths->size() ; j++){
		if(id != j){
			// Distancia entre o ponto atual de cada robô em runtimePaths
			theta = angulation(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size() - 1), runtimePaths->at(j).path.at(runtimePaths->at(j).path.size() - 1));
			distances = distance(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size() - 1), runtimePaths->at(j).path.at(runtimePaths->at(j).path.size() - 1));

			if(distances <= radiusRobot){
				//se esta escostado no obstaculo, recebe um vetor maximo 
				result.x += sign(cos(theta))*INF;
				result.y += sign(sin(theta))*INF;

			}else if(distances <= (radiusRobot + areaRobot)){
				//se esta dentro da area de influencia
				result.x += beta*(areaRobot + radiusRobot - distances)*cos(theta/180.0*M_PI); 
				result.y += beta*(areaRobot + radiusRobot - distances)*sin(theta/180.0*M_PI);

				result.x += rand() % 3;
				k++;
				result.y += rand() % 3;
				k++;	
			}
		}
	}

	//cout << "atrativo" << endl;
	//result.show();
}

void Goodrich::repulsiveForceRobotObjects(){
	double theta;
	double distances;
	int k;
	
	for(int j = 0 ; j < workspace->objects.size() ; j++){
		theta = angulation(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size() - 1), Pose(workspace->objects.at(j).x, workspace->objects.at(j).y, 0));
		distances = distance(runtimePaths->at(id).path.at(runtimePaths->at(id).path.size() - 1), Pose(workspace->objects.at(j).x, workspace->objects.at(j).y, 0));

		if(distances <= workspace->objects.at(j).radius){
			//se esta escostado no obstaculo, recebe um vetor maximo 
			result.x += sign(cos(theta))*INF;
			result.y += sign(sin(theta))*INF;

		}else if(distances <= (workspace->objects.at(j).radius + areaRobot)){
			//se esta dentro da area de influencia
			result.x += beta*(areaRobot + workspace->objects.at(j).radius - distances)*cos(theta/180.0*M_PI); 
			result.y += beta*(areaRobot + workspace->objects.at(j).radius - distances)*sin(theta/180.0*M_PI);

			result.x += rand() % 3;
			k++;
			result.y += rand() % 3;
			k++;	
		}
	}

	//cout << "repulsivo" << endl;
	//result.show();
}

void Goodrich::setWorkspace(Workspace *workspace){
	this->workspace = workspace;
}

void Goodrich::setRuntimePaths(vector<Path>* runtimePaths){
	this->runtimePaths = runtimePaths;
}


float Goodrich::angulation(Pose a, Pose b){
	return (atan2(a.y - b.y, a.x - b.x) * (180/M_PI));
}

float Goodrich::distance(Pose a, Pose b){
	return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}