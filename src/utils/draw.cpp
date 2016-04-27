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

#include "draw.h"

vector<Path> pathsStatic;
vector<Object> *objectsStaticDraw;
vector<Path> *runtimePathsStatic;
float widthStatic, heightStatic;

Draw::Draw(){

}

void Draw::AlteraTamanhoJanela(GLsizei w, GLsizei h){
	if(h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) 
		gluOrtho2D (-20, 20, -20*h/w, 20*h/w);
	else 
		gluOrtho2D (-20*w/h, 20*w/h, -20, 20);

	glMatrixMode(GL_MODELVIEW);
}


void Draw::init(int argc, char** argv){
	this->argc = argc;
	this->argv = argv;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowPosition(5,5); 
	glutInitWindowSize(width,height); 
	glutCreateWindow("MultiplePathPlanning");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutTimerFunc(5, timerHandler, 0);
	Inicializa();

	glutMainLoop();
}

void Draw::drawSphere(float x, float y, float radius){
	glPushMatrix();
        glTranslatef((x/25.0)-20, (y/25.0)-20, 0.0);
        glBegin(GL_TRIANGLE_FAN);
            glLineWidth(5);
            for(float i = 0 ; i < M_PI*2+01 ; i = i+0.1){
                glVertex2f((radius/50.0)*cos(i), (radius/50.0)*sin(i));
            }            
        glEnd();
    glPopMatrix(); 
}

void Draw::drawSquare(float x, float y){
	glPushMatrix();
		glTranslatef((x/25.0)-20, (y/25.0)-20, 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-0.15,-0.15);
			glVertex2f(-0.15, 0.15);       
			glVertex2f( 0.15, 0.15);       
			glVertex2f( 0.15,-0.15);
		glEnd();
	glPopMatrix();
}

void Draw::drawPaths(){
	
	/*for(int j = 0 ; j < pathsStatic.size() ; j++){
		glLineWidth(2);
		switch(j){
			case 0:{
				glColor3f(0.5, 0.0, 0.0);
			}break;
			case 1:{
				glColor3f(0.0, 0.5, 0.0);
			}break;
			case 2:{
				glColor3f(0.0, 0.0, 0.5);
			}break;
			case 3:{
				glColor3f(0.5, 0.0, 0.5);
			}break;
		}

		for(int i = 0 ; i < boost::static_pointer_cast<og::PathGeometric>(pathsStatic.at(j))->getStateCount() ; i++){
			vector<ob::State*> path = boost::static_pointer_cast<og::PathGeometric>(pathsStatic.at(j))->getStates();
			double xAnte, yAnte;

			const ob::SE2StateSpace::StateType* state2D = path.at(0)->as<ob::SE2StateSpace::StateType>();
			xAnte = state2D->getX();
			yAnte = state2D->getY();

			drawSphere(xAnte, yAnte, 15);

			for(int k = 1 ; k < path.size() ; k++){
				const ob::SE2StateSpace::StateType* state2D = path.at(k)->as<ob::SE2StateSpace::StateType>();
    			
    			glBegin(GL_LINES);
    				glVertex3f((xAnte/25.0)-20, (yAnte/25.0)-20, 0);
					glVertex3f((state2D->getX()/25.0)-20, (state2D->getY()/25.0)-20, 0);
				glEnd();

				xAnte = state2D->getX();
				yAnte = state2D->getY();

				drawSquare(xAnte, yAnte);
			}	
		}
	}

	for(int j = 0 ; j < pathsStatic.size() ; j++){
		glLineWidth(3); 

		for(int i = 0 ; i < runtimePathsStatic->size() ; i++){
			Pose anterior = runtimePathsStatic->at(i).path.at(0);
				switch(i){
				case 0:{
					glColor3f(1.0, 0.0, 0.0);
				}break;
				case 1:{
					glColor3f(0.0, 1.0, 0.0);
				}break;
				case 2:{
					glColor3f(0.0, 0.0, 1.0);
				}break;
				case 3:{
					glColor3f(1.0, 0.0, 1.0);
				}break;
			}
			drawSphere(anterior.x, anterior.y, 15);

			for(int k = 1 ; k < runtimePathsStatic->at(i).path.size() ; k++){

    			glBegin(GL_LINES);
    				glVertex3f((anterior.x/25.0)-20, (anterior.y/25.0)-20, 0);
					glVertex3f((runtimePathsStatic->at(i).path.at(k).x/25.0)-20, (runtimePathsStatic->at(i).path.at(k).y/25.0)-20, 0);
				glEnd();

				anterior = runtimePathsStatic->at(i).path.at(k);
			}	
		}
	}

	glColor3d(0.0, 0.0, 0.0);
	for(int i = 0 ; i < objectsStaticDraw->size() ; i++){
		drawSphere(objectsStaticDraw->at(i).x, objectsStaticDraw->at(i).y, objectsStaticDraw->at(i).radius);
	}*/
}

void Draw::Desenha(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	drawPaths();

	usleep(33333);
	glutSwapBuffers();
}

void Draw::Inicializa(void){   
	glClearColor(1.0f, 1.0f, 1.0f, 0.4f);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Draw::timerHandler(int v){
    glutPostRedisplay();
    glutTimerFunc(5, timerHandler, 0);
}

void Draw::setPaths(vector<Path> paths){
	this->paths = paths;
	pathsStatic = this->paths;
	//cout << paths.size() << " : " << pathsStatic.size() << endl;
}

void Draw::setObjects(vector<Object> *objects){
	this->objects = objects;
	objectsStaticDraw = this->objects;
}

Pose Draw::handlePose(Pose a){
	a.x = (a.x/25.0)-20;
	a.y = (a.y/25.0)-20;

	return a;
}

void Draw::setSize(float width, float height){
	this->width = width;
	this->height = height;
	widthStatic = width;
	heightStatic = height;
}

void Draw::allocateRuntimePaths(vector<Path> *runtimePaths){
	this->runtimePaths = runtimePaths;
	runtimePathsStatic = runtimePaths;
}
