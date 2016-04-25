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
