/*
 * File:   pid_controller.c
 * Author: Bo
 *
 * Created on October 25, 2015, 3:39 PM
 */


#include "pid_controller.h"
#include <stdlib.h>

void pid_controller_init(pid_controller_t * controller, float Target, float Delta_time, float Kp, float Ki, float Kd) {
    controller->current_delta = 0.0f;
    controller->previous_delta = 0.0f;
    controller->integral = 0.0f;
    controller->derivative = 0.0f;

    controller->delta_time = Delta_time;

    controller->target = Target;
    
    controller->feedback = 0.0f;

    controller->kp = Kp;
    controller->ki = Ki;
    controller->kd = Kd;
}

uint16_t feed_back(pid_controller_t * controller, uint16_t pos) {
    // Proportional feedback
    controller->current_delta = controller->target - pos;
    
    // this happens due to ball losing contact to flextouch temporarily
    if( abs((controller->previous_delta - controller->current_delta)/controller->delta_time) > 10*controller->derivative )
        controller->current_delta = controller->previous_delta;

    // Integration feedback
    controller->integral += controller->current_delta * controller->delta_time;

    // Derivative feedback
    controller->derivative = (controller->previous_delta - controller->current_delta) / controller->delta_time;
    controller->previous_delta = controller->current_delta;

    controller->feedback = controller->kp * controller->current_delta + controller->ki * controller->integral - controller->kd * controller->derivative;

    return controller->feedback + 290; // 290 is the magic number to trim-center the midpoint of flextouch
}
