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
    cout << name << endl;
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
    workspace.show();
    return workspace;
}

void Database::makeRandomWorkspace(){
    ofstream write;
    int timeHelper;
    stringstream ss;
    int qtdRobots;
    int qtdObjects;

    float xInit, yInit, yawInit;
    float xGoal, yGoal, yawGoal;

    srand(time(NULL)+timeHelper);
 
    ss << (time(NULL)+timeHelper);

    qtdRobots = rand()%5;
    timeHelper += 10;

    qtdObjects = rand()%100;
    timeHelper += 10;

    cout << ss.str() << endl;

    write.open(ss.str().c_str());
        for(int i = 0 ; i < qtdRobots ; i++){
            xInit = rand()%1000;
            timeHelper++;
            yInit = rand()%1000;
            timeHelper++;
            yawInit = rand()%3;
            write << xInit << " " << yInit << " " << yawInit << endl;
        }
        write << endl;
        for(int i = 0 ; i < qtdRobots ; i++){
            xGoal = rand()%1000;
            timeHelper++;
            yGoal = rand()%1000;
            timeHelper++;
            yawGoal = rand()%3;
            write << xGoal << " " << yGoal << " " << yawGoal << endl;
        }
        write << endl;
        for(int i = 0 ; i < qtdObjects ; i++){
            float x = rand()%1000;
            timeHelper++;
            float y = rand()%1000;
            timeHelper++;
            float radius = rand()%50;
            if(x != xInit && x != xGoal && y != yInit && y != yGoal){
                write << x << " " << y << " " << radius << endl;
            }else{
                i--;
            }
        }
    write.close();

   // ofstream write;

    //write.open();
}
