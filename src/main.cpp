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
#include "control/pid.h"
#include "offline_planning.h"
#include "pthread.h"

using namespace std;
using namespace common;

pthread_t thread[4];
vector<ob::PathPtr> paths;
vector<RuntimePath> runtimePaths;
Draw draw;
Net net;
Workspace workspace;
grSim_Packet packet_grSim;
int idDone;
int argcG;
char** argvG;

void* draw_thread(void *arg);
void *exec_thread(void *arg);
void *receive_thread(void *arg);
void *send_thread(void *arg);
Workspace packetToWorkspace(SSL_WrapperPacket);
Pose handlePosition(Pose poseGR);
Object handlePosition(Object objectGR);

bool argParse(int argc, char** argv, std::string *outputFilePtr);

int main(int argc, char** argv){
    Offline_planning offline(argc, argv);
    Database db;
    
    argcG = argc;
    argvG = argv;

    net.setIpPortReceive();     // with default values

    //workspace = db.loadWorkspace("src/data/workspaces/random7"); // carregando um espaço de trabalho salvo em memória
    usleep(1000000);

    while(net.hasNewPacket()){
        workspace = packetToWorkspace(net.receivePacket());
    }
    
    offline.setBounds(0.0, 1000.0);        // (0, 0) -> (640, 640)
    offline.setTimeToResolve(1);           // 1 segundo no máximo para offline o problema
    offline.setPlanner(PLANNER_RRTSTAR);
    offline.setWorkspace(&workspace);

    paths = offline.solvePaths();

    pthread_create(&(thread[0]), NULL, draw_thread, NULL);
    //pthread_create(&(thread[1]), NULL, exec_thread, NULL);
    pthread_create(&(thread[2]), NULL, receive_thread, NULL);
    //pthread_create(&(thread[3]), NULL, send_thread, NULL);
    
    pthread_join(thread[0], NULL);
    //pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
    //pthread_join(thread[3], NULL);

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

void *receive_thread(void *arg){
    idDone = 0;
    Net net2;
    PID pid;
    Goodrich goodrich;

    net2.setIpPortSend();        // with default values
    net2.setIpPortReceive();     // with default values

    while(1){
        if(net2.hasNewPacket()){
            float x, y, yaw;
            Command cmd;
            workspace = packetToWorkspace(net2.receivePacket());
            //cout << paths.size() << endl;
            vector<ob::State*> path = boost::static_pointer_cast<og::PathGeometric>(paths.at(0))->getStates();
            cout << path.size() << endl;
            ob::SE2StateSpace::StateType* state2D = path.at(idDone)->as<ob::SE2StateSpace::StateType>();
            
            x = state2D->getX();
        y = state2D->getY();
        yaw = state2D->getYaw();
    

            Pose p(x, y, yaw);
            if(goodrich.distance(workspace.start.at(0), p) < 20 && idDone < path.size()-1){
                idDone += 1;

                state2D = path.at(idDone)->as<ob::SE2StateSpace::StateType>();
            
                p.x = state2D->getX();
                p.y = state2D->getY();
                p.yaw = state2D->getYaw();
            }

            cout << "idDone " << idDone << endl; 

            cmd = pid.calcCommand(workspace.start.at(0), p);
            cmd.show();

            usleep(10000);
            // planning
            bool yellow = false;

            packet_grSim.mutable_commands()->set_isteamyellow(yellow);
            packet_grSim.mutable_commands()->set_timestamp(0.0);

            grSim_Robot_Command* command = packet_grSim.mutable_commands()->add_robot_commands();
            command->set_id(0);

            command->set_wheelsspeed(0);
            command->set_wheel1(0);
            command->set_wheel2(0);
            command->set_wheel3(0);
            command->set_wheel4(0);
            command->set_veltangent(cmd.vel_tangent);
            command->set_velnormal(cmd.vel_normal);
            command->set_velangular(cmd.vel_angular);

            command->set_kickspeedx(0);
            command->set_kickspeedz(0);
            command->set_spinner(false);

            net2.sendPacket(packet_grSim);
        }
    }
}

void *send_thread(void *arg){
    while(1){
        bool yellow = false;

        packet_grSim.mutable_commands()->set_isteamyellow(yellow);
        packet_grSim.mutable_commands()->set_timestamp(0.0);

        grSim_Robot_Command* command = packet_grSim.mutable_commands()->add_robot_commands();
        command->set_id(0);

        command->set_wheelsspeed(0);
        command->set_wheel1(0);
        command->set_wheel2(0);
        command->set_wheel3(0);
        command->set_wheel4(0);
        command->set_veltangent(0);
        command->set_velnormal(0);
        command->set_velangular(0);

        command->set_kickspeedx(0);
        command->set_kickspeedz(0);
        command->set_spinner(false);

        net.sendPacket(packet_grSim);
    }
}

Workspace packetToWorkspace(SSL_WrapperPacket packet){
    Workspace ws;

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

    //ws.show();

    return ws;
}

Pose handlePosition(Pose poseGR){
    Pose poseMPP;
    
    poseMPP.x = (poseGR.x + 3000)/6.0;
    poseMPP.y = (poseGR.y + 2000)/6.0;
    poseMPP.yaw = poseGR.yaw;

    return poseMPP;
}

Object handlePosition(Object objectGR){
    Object objectMPP;

    objectMPP.x = (objectGR.x + 3000)/6.0;
    objectMPP.y = (objectGR.y + 2000)/6.0;
    objectMPP.radius = objectGR.radius;

    return objectMPP;
}

