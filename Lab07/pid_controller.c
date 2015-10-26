/*
 * File:   pid_controller.c
 * Author: Bo
 *
 * Created on October 25, 2015, 3:39 PM
 */


#include "pid_controller.h"

void pid_controller_init(pid_controller_t * controller, float Target, float Delta_time, float Kp, float Ki, float Kd) {
    controller->current_delta = 0.0f;
    controller->previous_delta = 0.0f;
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
    controller->current_delta = controller->target - pos;
    
    // Integration feedback
    controller->integral += controller->current_delta;
    
    // Derivative feedback
    controller->derivative = (controller->current_delta - controller->previous_delta) / controller->delta_time;
    controller->previous_delta = controller->current_delta;
     
    float u = controller->kp * controller->current_delta + controller->ki * controller->integral - controller->kd * controller->derivative;
    
    // need to clamp u??
    
    return u + 290; // magic numbers
}

// uint16_t feed_back(pid_controller_t * controller, uint16_t pos) {
//     // Proportional feedback
//     controller->error = controller->target - pos;
    
//     // update controller history
//     controller->history[controller->history_idx] = controller->error;
    
//     ++controller->history_idx;
//     if (controller->history_idx >= MEMORY)
//         controller->history_idx = 0;

//     int i = 0;
//     for(controller->integral = 0; i < MEMORY; ++i) {
//         // Integral feedback
//         controller->integral += controller->history[i];
//     }
    
//     // Derivative feedback
//     uint16_t previous = controller->error;
//     if(controller->history_idx == 0)
//         previous = controller->history[MEMORY];
//     else
//         previous = controller->history[controller->history_idx - 1];
    
//     controller->derivative = (controller->error - controller->history[previous]) / controller->delta_time;
     
//     float u = controller->kp*controller->error + controller->ki*controller->integral - controller->kd*controller->derivative;
    

//     // need to clamp u??
    
//     return u + 290; // magic numbers
// }
