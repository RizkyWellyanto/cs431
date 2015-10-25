/*
 * File:   pid_controller.c
 * Author: Bo
 *
 * Created on October 25, 2015, 3:39 PM
 */


#include "pid_controller.h"

void pid_controller_init(pid_controller_t * controller, float Target, float Delta_time, float Kp, float Ki, float Kd) {
    controller->error = 0.0f;
    controller->integral = 0.0f;
    controller->derivative = 0.0f;
    
    controller->delta_time = Delta_time;
    
    controller->target = Target;
    
    controller->kp = Kp;
    controller->ki = Ki;
    controller->kd = Kd;
    
    controller->history_idx = 0;
    
    int i = 0;
    for(; i < MEMORY; ++i) {
        controller->history[i] = 0;
    }
}

uint16_t feed_back(pid_controller_t * controller, uint16_t pos) {
    // Proportional feedback
    controller->error = controller->target - pos;
    
    // update controller history
    controller->history[controller->history_idx] = controller->error;
    
    ++controller->history_idx;
    if (controller->history_idx >= MEMORY)
        controller->history_idx = 0;
    
    int i = 0;
    for(; i < MEMORY; ++i) {
        // Integral feedback
        controller->integral += controller->history[i];
    }
    
    // Derivative feedback
    float previous = controller->error;
    if(controller->history_idx == 0)
        previous = controller->history[MEMORY];
    else
        previous = controller->history[controller->history_idx - 1];
    
    float controller->derivative = (controller->error - controller->history[previous]) / controller->delta_time;
     
    float u = controller->kp*controller->error + controller->ki*controller->integral - controller->kd*controller->derivative;
    
    // clamp u and convert to duty
    if(u > 200)
        u = 200;
    else if(u < -400)
        u = -400;
    
    return (((u+400)*160L)/ (600)) + 220; // magic numbers
}
