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

#include "offline_planning.h"

vector<Object> objectsStatic;
Offline_planning::Offline_planning(){
    
}

Offline_planning::Offline_planning(int argc, char** argv){
    this->argc = argc;
    this->argv = argv;
}

vector<ob::PathPtr> Offline_planning::solvePaths(){
    objectsStatic = workspace->objects;

    ob::StateSpacePtr space(new ob::SE2StateSpace());

    ob::RealVectorBounds bounds(2);
    bounds.setLow(lowBound);
    bounds.setHigh(highBound);

    space->as<ob::SE2StateSpace>()->setBounds(bounds);

    ob::SpaceInformationPtr si(new ob::SpaceInformation(space));

    si->setStateValidityChecker(boost::bind(&isStateValid, _1));

    ob::ScopedState<ob::SE2StateSpace> start(space);
    ob::ScopedState<ob::SE2StateSpace> goal(space);

    //workspace.show();

    for(int i = 0 ; i < workspace->start.size() ; i++){
        start->setX(workspace->start.at(i).x);
        start->setY(workspace->start.at(i).y);
        start->setYaw(workspace->start.at(i).yaw);

        goal->setX(workspace->goal.at(i).x);
        goal->setY(workspace->goal.at(i).y);
        goal->setYaw(workspace->goal.at(i).yaw);


        ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));
        pdef->setStartAndGoalStates(start, goal);
        ob::PlannerPtr planner(new og::RRTConnect(si));
        planner->setProblemDefinition(pdef);
        //planner->setup();
        //si->printSettings(std::cout);
        //pdef->print(std::cout);
        ob::PlannerStatus solved = planner->solve(timeToResolve);

        if(solved){
            ob::PathPtr path;
            path = pdef->getSolutionPath();
            paths.push_back(path);
        }else{
            std::cout << "No solution found" << std::endl;
        }
    }

    return paths;
}

bool Offline_planning::isStateValid(const ob::State *state){
    bool ok = true;
    const ob::SE2StateSpace::StateType* state2D = state->as<ob::SE2StateSpace::StateType>();
    double x = state2D->getX();
    double y = state2D->getY();
    for(int i = 0 ; i < objectsStatic.size() ; i++){
        double dis = sqrt((x-objectsStatic.at(i).x)*(x-objectsStatic.at(i).x) + (y-objectsStatic.at(i).y)*(y-objectsStatic.at(i).y)) - objectsStatic.at(i).radius;
        if(dis < 0.0){
            ok = false;
        }
    }
    
    //objectsStatic.at(0).show();
    return ok;
}

void Offline_planning::setBounds(float lowBound, float highBound){
    this->lowBound = lowBound;
    this->highBound = highBound;
}

void Offline_planning::setPlanner(typePlanner planner){
    this->planner = planner;
}

void Offline_planning::setTimeToResolve(float timeToResolve){
    this->timeToResolve = timeToResolve;
}

void Offline_planning::setWorkspace(Workspace *workspace){
    this->workspace = workspace;
}

/*ob::PlannerPtr Offline_planning::allocatePlanner(ob::SpaceInformationPtr si, optimalPlanner plannerType)
{
    switch (plannerType)
    {
        case PLANNER_BITSTAR:
        {
            return boost::make_shared<og::BITstar>(si);
            break;
        }
        case PLANNER_CFOREST:
        {
            return boost::make_shared<og::CForest>(si);
            break;
        }
        case PLANNER_FMTSTAR:
        {
            return boost::make_shared<og::FMT>(si);
            break;
        }
        case PLANNER_INF_RRTSTAR:
        {
            return boost::make_shared<og::InformedRRTstar>(si);
            break;
        }
        case PLANNER_PRMSTAR:
        {
            return boost::make_shared<og::PRMstar>(si);
            break;
        }
        case PLANNER_RRTSTAR:
        {
            return boost::make_shared<og::RRTstar>(si);
            break;
        }
        default:
        {
            OMPL_ERROR("Planner-type enum is not implemented in allocation function.");
            return ob::PlannerPtr(); // Address compiler warning re: no return value.
            break;
        }
    }
}*/

