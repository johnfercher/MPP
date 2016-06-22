#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#include "iostream"
#include "sstream"
#include "vector"

#include "math.h"

#include "sqlite.h"

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

class Benchmark{
protected:
	SQLite *sqlite;

	float angulation(Pose a, Pose b);
	float distance(Pose a, Pose b);

public:
	Benchmark();	

	void createRandomWorkspace();
};

#endif // _BENCHMARK_H_
