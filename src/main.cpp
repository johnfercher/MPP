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

#include "iostream"
#include "sstream"
#include "utils/includes/boost.h"
#include "utils/includes/ompl.h"
#include "utils/commons.h"
#include "utils/net.h"

#include "control/goodrich.h"
#include "resolver.h"
#include "pthread.h"

using namespace std;
using namespace common;

pthread_t thread[3];
vector<ob::PathPtr> paths;
vector<RuntimePath> runtimePaths;
Draw draw;
Net net;
Workspace workspace;
int argcG;
char** argvG;

void* draw_thread(void *arg);
void *exec_thread(void *arg);
void *net_thread(void *arg);
Workspace getWorkspaceGrSim();
Pose handlePosition(Pose poseGR);
Object handlePosition(Object objectGR);

bool argParse(int argc, char** argv, std::string *outputFilePtr);

int main(int argc, char** argv){
    Resolver resolver(argc, argv);
    Database db;
    
    argcG = argc;
    argvG = argv;

    //net.setIpPortSend();        // with default values
    net.setIpPortReceive();     // with default values

    workspace = db.loadWorkspace("src/data/workspaces/random7"); // carregando um espaço de trabalho salvo em memória
    workspace = getWorkspaceGrSim();

    resolver.setBounds(0.0, 1000.0);        // (0, 0) -> (640, 640)
    resolver.setTimeToResolve(1);           // 1 segundo no máximo para resolver o problema
    resolver.setPlanner(PLANNER_RRTSTAR);
    resolver.setWorkspace(&workspace);

    paths = resolver.solvePaths();

    pthread_create(&(thread[0]), NULL, draw_thread, NULL);
    pthread_create(&(thread[1]), NULL, exec_thread, NULL);
    pthread_create(&(thread[2]), NULL, net_thread, NULL);
    
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    return 0;   
}

void* draw_thread(void *arg){
    draw.setPaths(paths);
    draw.allocateRuntimePaths(&runtimePaths);
    draw.setObjects(&workspace.objects);
    draw.setSize(1000.0, 1000.0); // pode dar problema
    draw.init(argcG, argvG);
}

void *exec_thread(void *arg){
    Goodrich pf;
    pf.setPaths(&paths);
    pf.setWorkspace(&workspace);
    pf.allocateRuntimePaths(&runtimePaths);
    pf.init();
}

void *net_thread(void *arg){
    while(1){
        if(net.hasNewPacket()){
            net.receivePacket();
        }
    }
}

Workspace getWorkspaceGrSim(){
    Workspace ws;
    SSL_WrapperPacket packet;

    usleep(1000000);

    while(net.hasNewPacket()){
        packet = net.receivePacket();
    }

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

    ws.show();

    return ws;
}

Pose handlePosition(Pose poseGR){
    Pose poseMPP;
    
    poseMPP.x = (poseGR.x + 3000)/5.0;
    poseMPP.y = (poseGR.y + 2000)/5.0;
    poseMPP.yaw = poseGR.yaw;

    return poseMPP;
}

Object handlePosition(Object objectGR){
    Object objectMPP;

    objectMPP.x = (objectGR.x + 3000)/5.0;
    objectMPP.y = (objectGR.y + 2000)/5.0;
    objectMPP.radius = objectGR.radius;

    return objectMPP;
}

