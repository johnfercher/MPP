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

#include "database.h"

Database::Database(){
	
}

void Database::savePath(string name, ob::PathPtr pathPtr){
	string final;
	ofstream outFile("path3y");
		boost::static_pointer_cast<og::PathGeometric>(pathPtr)->printAsMatrix(outFile);
    outFile.close();

    transform3in2();

    system("gnuplot utils/gnuplot/plotHD.plt");
    system("gnuplot utils/gnuplot/plotLOW.plt");

    final = "mv path data/paths" + name;
    //cout << final << endl;
    system(final.c_str());

    final = "mv path3y data/paths" + name + "3y";
    //cout << final << endl;
    system(final.c_str());

    final = "mv pathHD.png data/paths" + name + "HD.png";
    //cout << final << endl;
    system(final.c_str());

    final = "mv pathLOW.png data/paths" + name + "LOW.png";
    //cout << final << endl;
    system(final.c_str());
}

void Database::transform3in2(){
    stringstream ss;
    string line, linel;
    ifstream read;
    ofstream write;

    read.open("path3y");
    write.open("path");
        while(getline(read, line)){
            ss << line;
            for(int i = 0 ; getline(ss, linel, ' ') ; i++){
                //cout << linel << ";";
                if(i < 2)
                    write << linel << " ";
            }
            write << endl;
            //cout << endl;
            ss.clear();
        }
    write.close();
    read.close();
}

Workspace Database::loadWorkspace(string name){
    //cout << name << endl;
    Workspace workspace;
    stringstream ss;
    string line, linel;
    ifstream read;

    read.open(name.c_str());
        /*Carrega as Poses iniciais*/{
            while(getline(read, line)){
                if(line == "")  break;
                Pose initial;

                clearSS(ss);
                ss << line;

                getline(ss, linel, ' ');
                initial.x = toFloat(linel);

                getline(ss, linel, ' ');
                initial.y = toFloat(linel);

                getline(ss, linel, ' ');
                initial.yaw = toFloat(linel);

                workspace.start.push_back(initial);
                //initial.show();
            }
        }

        /*Carrega as Poses finais*/{
            while(getline(read, line)){
                if(line == "")  break;
                Pose final;

                clearSS(ss);
                ss << line;

                getline(ss, linel, ' ');
                final.x = toFloat(linel);

                getline(ss, linel, ' ');
                final.y = toFloat(linel);

                getline(ss, linel, ' ');
                final.yaw = toFloat(linel);

                workspace.goal.push_back(final);
                //final.show();
            }
        }

        /*Carrega os obstaculos*/{
            while(getline(read, line)){
                if(line == "")  break;
                Object objetos;

                clearSS(ss);
                ss << line;

                getline(ss, linel, ' ');
                objetos.x = toFloat(linel);

                getline(ss, linel, ' ');
                objetos.y = toFloat(linel);

                getline(ss, linel, ' ');
                objetos.radius = toFloat(linel);

                workspace.objects.push_back(objetos);
                //objetos.show();
            }
        }

    read.close();
    //workspace.show();
    return workspace;
}

void Database::makeRandomWorkspace(){
    ofstream write;
    int timeHelper;
    stringstream ss;
    int qtdRobots;
    int qtdObjects;

    vector<float> xInit, yInit, yawInit;
    vector<float> xGoal, yGoal, yawGoal;

    //float xInit, yInit, yawInit;
    //float xGoal, yGoal, yawGoal;

    srand(time(NULL)+timeHelper);
 
    ss << "src/data/workspaces/" << (time(NULL)+timeHelper);

    qtdRobots = 4+rand()%6;
    timeHelper += 10;

    qtdObjects = rand()%100;
    timeHelper += 10;

    cout << ss.str() << endl;

    write.open(ss.str().c_str());
        for(int i = 0 ; i < qtdRobots ; i++){
            xInit.push_back(rand()%1000);
            timeHelper++;
            yInit.push_back(rand()%1000);
            timeHelper++;
            yawInit.push_back(rand()%3);
            write << xInit.at(i) << " " << yInit.at(i) << " " << yawInit.at(i) << endl;
        }
        write << endl;
        for(int i = 0 ; i < qtdRobots ; i++){
            xGoal.push_back(rand()%1000);
            timeHelper++;
            yGoal.push_back(rand()%1000);
            timeHelper++;
            yawGoal.push_back(rand()%3);
            write << xGoal.at(i) << " " << yGoal.at(i) << " " << yawGoal.at(i) << endl;
        }
        write << endl;
        for(int i = 0 ; i < qtdObjects ; i++){
            float x = rand()%1000;
            timeHelper++;
            float y = rand()%1000;
            timeHelper++;
            float radius = rand()%50;

            bool ok = true;
            for(int i = 0 ; i < qtdRobots ; i++){
                float distGoal = distance(Pose(x, y, 0), Pose(xGoal.at(i), yGoal.at(i), 0));
                float distInit = distance(Pose(x, y, 0), Pose(xInit.at(i), yInit.at(i), 0));

                if(distGoal < 60 || distInit < 60){
                    ok = false;
                }
            }

            if(ok){
                write << x << " " << y << " " << radius << endl;
            }else{
                i--;
            }
        }
    write.close();

   // ofstream write;

    //write.open();
}

float Database::distance(Pose a, Pose b){
    return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}