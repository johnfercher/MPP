#ifndef _DRAW_H_
#define _DRAW_H_

#include "includes/ompl.h"
#include "commons.h"
#include <GL/glut.h>
#include "iostream"

using namespace std;
using namespace common;

class Draw{
private:
	int argc;
	char** argv;

	vector<ob::PathPtr> paths;
	vector<RuntimePath> *runtimePaths;
	vector<Object> *objects;
	float width, height;

	static void drawSquare(float, float);
	static void drawSphere(float, float, float);
	static void Desenha();
	static void Inicializa();
	static void timerHandler(int v);
	static void AlteraTamanhoJanela(GLsizei w, GLsizei h);
	static void drawPaths();
	Pose handlePose(Pose);
	static void setColor(int);
	
public:
	Draw();

	void init(int argc, char** argv);
	void setPaths(vector<ob::PathPtr>);
	void allocateRuntimePaths(vector<RuntimePath>*);
	void setObjects(vector<Object>*);
	void setSize(float, float);
};

#endif