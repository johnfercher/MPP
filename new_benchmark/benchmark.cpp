#include "benchmark.h"

Benchmark::Benchmark(){
    sqlite = new SQLite("main.db", "passwd");
}

float Benchmark::angulation(Pose a, Pose b){
    return (atan2(a.y - b.y, a.x - b.x) * (180/M_PI));
}

float Benchmark::distance(Pose a, Pose b){
    return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}

void Benchmark::createRandomWorkspace(){
    int timeHelper;
    stringstream s_robots, s_obstacles;
    int qtdRobots;
    int qtdObjects;

    vector<float> xInit, yInit, yawInit;
    vector<float> xGoal, yGoal, yawGoal;
    vector<float> xObj, yObj, rObj, mObj;

    srand(time(NULL)+timeHelper);

    qtdRobots = 4+rand()%6;
    timeHelper += 10;

    qtdObjects = rand()%100;
    timeHelper += 10;


    for(int i = 0 ; i < qtdRobots ; i++){
        xInit.push_back(rand()%1000);
        timeHelper++;
        yInit.push_back(rand()%1000);
        timeHelper++;
        yawInit.push_back(rand()%3);
    }

    for(int i = 0 ; i < qtdRobots ; i++){
        xGoal.push_back(rand()%1000);
        timeHelper++;
        yGoal.push_back(rand()%1000);
        timeHelper++;
        yawGoal.push_back(rand()%3);
    }

    for(int i = 0 ; i < qtdObjects ; i++){
        float x = rand()%1000;
        timeHelper++;
        float y = rand()%1000;
        timeHelper++;
        float radius = 2+rand()%25;
        timeHelper++;
        float move = rand()%3;          // 0 - parado, 1 - linear, 2 - circular

        bool ok = true;
        for(int i = 0 ; i < qtdRobots ; i++){
            float distGoal = distance(Pose(x, y, 0), Pose(xGoal.at(i), yGoal.at(i), 0));
            float distInit = distance(Pose(x, y, 0), Pose(xInit.at(i), yInit.at(i), 0));

            if(distGoal < 60 || distInit < 60){
                ok = false;
            }
        }

        if(ok){
            xObj.push_back(x);
            yObj.push_back(y);
            rObj.push_back(radius);
            mObj.push_back(move);
        }else{
            i--;
        }
    }

    for(int i = 0 ; i < xInit.size() ; i++){
        s_robots << xInit.at(i) << ":";
        s_robots << yInit.at(i) << ":";
        s_robots << yawInit.at(i) << ":";
        s_robots << xGoal.at(i) << ":";
        s_robots << yGoal.at(i) << ":";
        s_robots << yawGoal.at(i) << ";";
    }

    for(int i = 0 ; i < xObj.size() ; i++){
        s_obstacles << xObj.at(i) << ":";
        s_obstacles << yObj.at(i) << ":";
        s_obstacles << rObj.at(i) << ":";
        s_obstacles << mObj.at(i) << ";";
    }

    cout << "qtd_robots: " << qtdRobots << endl;
    cout << "qtd_obstacles: " << qtdObjects << endl;
    cout << "robots: " << s_robots.str() << endl;
    cout << "obstacles: " << s_obstacles.str() << endl;
}