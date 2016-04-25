#include "iostream"
#include "sstream"
#include "utils/includes/boost.h"
#include "utils/includes/ompl.h"
#include "utils/commons.h"

#include "control/goodrich.h"
#include "resolver.h"
#include "pthread.h"

using namespace std;
using namespace common;

pthread_t thread[2];
vector<ob::PathPtr> paths;
vector<RuntimePath> runtimePaths;
Draw draw;
Workspace workspace;
int argcG;
char** argvG;

void* draw_thread(void *arg){
    draw.setPaths(paths);
    draw.allocateRuntimePaths(&runtimePaths);
    draw.setObjects(&workspace.objects);
    draw.setSize(1000.0, 1000.0);
    draw.init(argcG, argvG);
}

void *exec_thread(void *arg){
    Goodrich pf;
    pf.setPaths(&paths);
    pf.setWorkspace(&workspace);
    pf.allocateRuntimePaths(&runtimePaths);
    pf.init();
}

int main(int argc, char** argv){
    Resolver resolver(argc, argv);
    Database db;

    argcG = argc;
    argvG = argv;

    string s;
    if(argc > 1){
        stringstream ss;
        ss << argv[1];
        s = ss.str();
    }

    workspace = db.loadWorkspace("src/data/workspaces/random7");

    resolver.setBounds(0.0, 1000.0);        // (0, 0) -> (640, 640)
    resolver.setTimeToResolve(1);           // 1 segundo no máximo para resolver o problema
    resolver.setPlanner(PLANNER_RRTSTAR);
    resolver.setWorkspace(&workspace);

    paths = resolver.solvePaths();

    pthread_create(&(thread[0]), NULL, draw_thread, NULL);
    pthread_create(&(thread[1]), NULL, exec_thread, NULL);
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    return 0;   
}