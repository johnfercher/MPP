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

#include "pid.h"

PID::PID(){
	proportional_l = 0.0004; //p1 = 0.0004
	proportional_a = 0.7000; //p2 = 0.8000
}

Command PID::calcCommand(Pose initial, Pose final){
	Command cmd(0, 0, 0);

	// Controle Demo-ai
	//if(distance(initial, final) > 20){
		cmd.vel_tangent = proportional_l * ((final.x - initial.x) * cos(initial.yaw) + (final.y - initial.y) * sin(initial.yaw));
		cmd.vel_normal = proportional_l * ((final.y - initial.y) * cos(initial.yaw) - (final.x - initial.x) * sin(initial.yaw));
		cmd.vel_angular = proportional_a * (final.yaw - initial.yaw);
	//}

	/*x, y, w = robot_x, robot_y, robot_w
	tx, ty, tw = ball_x, ball_y, 0

	v_tan  = p1 * ((tx - x) * cos(w) + (ty - y) * sin(w))
    v_norm = p1 * ((ty - y) * cos(w) - (tx - x) * sin(w))
    v_ang = p2 * (tw - w)*/

	return cmd;
}

float PID::angulation(Pose a, Pose b){
	return (atan2(a.y - b.y, a.x - b.x) * (180/M_PI));
}

float PID::distance(Pose a, Pose b){
	return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)));
}
