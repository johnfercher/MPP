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