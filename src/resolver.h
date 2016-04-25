#ifndef _OMPL_H_
#define _OMPL_H_

#include "utils/commons.h"
#include "utils/includes/boost.h"
#include "utils/draw.h"
#include "utils/database.h"

using namespace common;

class Resolver{
private:
    int argc;
    char **argv;

    float timeToResolve;                                    // Tempo para resolver o problema     
    float lowBound, highBound;                              // Limites do espaço
    typePlanner planner;                                    // Algoritmo de planejamento de trajetória
    planningObjective objectiveType;

    vector<ob::PathPtr> paths;
    Workspace *workspace;
    
    //ob::PlannerPtr allocatePlanner(ob::SpaceInformationPtr si, optimalPlanner plannerType);

    static bool isStateValid(const ob::State *state);
public:
    Resolver();  
    Resolver(int argc, char** argv);
    
    vector<ob::PathPtr> solvePaths();
    
    void setBounds(float, float);
    void setPlanner(typePlanner);
    void setTimeToResolve(float);
    void setWorkspace(Workspace*);
};

#endif
