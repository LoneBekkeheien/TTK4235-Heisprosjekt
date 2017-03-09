#include <time.h>
#include <stdio.h>
#include "Eventmanager.h"
#include "Elevmodule.h"
#include "Timer.h"
#include "Queue.h"

int ev_emergency_button_pushed(){
    return elev_get_stop_signal();
}

int ev_time_door_out(){
    return timer_3sec();
}

int ev_check_buttons(){ 

    int x=0;
    for (int i = 0; i < N_FLOORS; i++){
        if (elev_get_button_signal(BUTTON_COMMAND,i)){
            elev_set_button_lamp(BUTTON_COMMAND, i,1);
            x=1;
       }
   }
    for (int i = 0; i < N_FLOORS-1; i++){
        if(elev_get_button_signal(BUTTON_CALL_UP,i)){
            elev_set_button_lamp(BUTTON_CALL_UP, i,1);
            x=1;
        }
        else if(elev_get_button_signal(BUTTON_CALL_DOWN,i+1)){
            elev_set_button_lamp(BUTTON_CALL_DOWN, i+1,1);
            x=1;
       }
    }
    if(nOrders!=0){
        x=1;
    }
    return x;
}

 
int ev_order_same_floor(){ 
   int floor = elev_get_floor_sensor_signal();
   if(orders[0].floor==floor){
        return 1;
    }
    for(int i=0; i<nOrders; i++){ 
        if(orders[i].floor==floor && orders[i].direction==0){ //bare heispanel
            return 1;
        }
    }
    for(int i=0; i<nOrders; i++){ 
        if(orders[i].floor==floor && orders[i].direction==direction()){ //bare heispanel
            return 1;
        }
    } 
    return 0;
}

int direction(){
    int floor = elev_get_floor_sensor_signal();
    if(orders[0].floor>floor){
        return 1;
    }

    if(orders[0].floor<floor){
        return -1;
    }   
    return 0;
}

