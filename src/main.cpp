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
#include "mpp.h"
#include "pthread.h"

using namespace std;
using namespace common;

pthread_t thread[4];
vector<Path> runtimePaths;
Draw draw;
Net net;
Workspace workspace;
grSim_Packet packet_grSim;
int idDone;

void *receive_thread(void *arg);

bool argParse(int argc, char** argv, std::string *outputFilePtr);

int main(int argc, char** argv){
    MPP mpp(argc, argv);
    mpp.init();

    return 0;   
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
            //workspace = packetToWorkspace(net2.receivePacket());
            //cout << paths.size() << endl;
            /*vector<ob::State*> path = boost::static_pointer_cast<og::PathGeometric>(paths.at(0))->getStates();
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

            net2.sendPacket(packet_grSim);*/
        }
    }
}
