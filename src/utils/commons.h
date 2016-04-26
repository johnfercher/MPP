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

#ifndef _COMMONS_H_
#define _COMMONS_H_

#include "iostream"
#include "stdio.h"
#include "vector"
#include "sstream"
#include "math.h"

#define RADIUSROBOT		10.0
#define RAIDUSTARGET	10.0
#define PATH_WORKSPACES "data/workspaces"
#define PATH_PATHS		"data/paths"

using namespace std;

namespace common{
	enum { INVALID = -1 };
	struct Pose{
		float x, y, yaw;
		Pose(){
			x = y = yaw = 0;
		};
		Pose(float x, float y, float yaw){
			this->x = x;
			this->y = y;
			this->yaw = yaw;
		};
		Pose(Pose *p){
			x = p->x;
			y = p->y;
			yaw = p->yaw;
		};
		void show(){
			cout << "Pose(" << x << ", " << y << ", " << yaw << ")" << endl;
		};
	};
	struct RuntimePath{
		vector<Pose> path;
		RuntimePath(){};
		RuntimePath(vector<Pose> path){
			this->path = path;
		};
		RuntimePath(RuntimePath *p){
			path = p->path;
		};
		void show(){
			for(int i = 0 ; i < path.size() ; i++){
				cout << "Path" << endl;
				path.at(i).show();
			}
		};
	};
	struct Object{
		float x, y, radius;
		Object(){
			x = y = radius = 0;
		};
		Object(float x, float y, float radius){
			this->x = x;
			this->y = y;
			this->radius = radius;
		};
		Object(Object *p){
			x = p->x;
			y = p->y;
			radius = p->radius;
		};
		void show(){
			cout << "Object(" << x << ", " << y << ", " << radius << ")" << endl;
		};
	};
	struct Workspace{
		vector<Pose> start;
		vector<Pose> goal;
		vector<Object> objects;
		Workspace(){};
		Workspace(Workspace *w){
			start = w->start;
			goal = w->goal;
			objects = w->objects;
		};
		void show(){
			for(int i = 0 ; i < start.size() ; i++){
				cout << "start " << i << " ";
				start.at(i).show();
				cout << "goal " << i << " ";
				goal.at(i).show();
			}
			cout << endl;
			for(int i = 0 ; i < objects.size() ; i++){
				cout << i << " ";
				objects.at(i).show();
			}
		};
	};
	struct Command{
		float vel_tangent, vel_normal, vel_angular;
		Command(){
			vel_tangent = 0;
			vel_normal = 0;
			vel_angular = 0;
		};
		Command(float vel_tangent, float vel_normal, float vel_angular){
			this->vel_tangent = vel_tangent;
			this->vel_normal = vel_normal;
			this->vel_angular = vel_angular;
		};
		Command(Command *cmd){
			vel_tangent = cmd->vel_tangent;
			vel_normal = cmd->vel_normal;
			vel_angular = cmd->vel_angular;
		};
		void show(){
			cout << vel_tangent << " : " << vel_normal << " : " << vel_angular << endl;
		}
	};

	void clearSS(stringstream &ss);

	string toString(int a);

	string toString(float a);

	string toString(double a);

	string toString(long long int a);

	string toString(bool a);

	int toInt(char a);

	int toInt(string a);

	float toFloat(string a);

	double toDouble(string a);

	long long int toLongLongInt(string a);

	bool toBool(string a);

	string cmdTerminal(string s);
};

#endif