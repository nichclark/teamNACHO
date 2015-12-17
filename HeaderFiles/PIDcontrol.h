/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("PID Control Header File");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This header file declares TeamNACHO's PID Control Fxn
*/


#ifndef PIDCONTROL_H
#define PIDCONTROL_H

float PIDcontrol(float Desired, float Actual, float kP, float kI, float kD);

#endif