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


#ifndef _PID_H_
#define _PID_H_

#include "math.h"
#include "../utils/commons.h"

using namespace common;

class PID{
private:
	float proportional_l, integrative_l, derivative_l;
	float proportional_a, integrative_a, derivative_a;

public:
	PID();

	Command calcCommand(Pose initial, Pose final);
	float angulation(Pose a, Pose b);
	float distance(Pose a, Pose b);
};

#endif // _PID_H_