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

	vector<Path> paths;
	vector<Path> *runtimePaths;
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
	void setPaths(vector<Path>);
	void allocateRuntimePaths(vector<Path>*);
	void setObjects(vector<Object>*);
	void setSize(float, float);
};

#endif