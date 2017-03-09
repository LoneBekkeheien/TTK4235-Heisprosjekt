#include "Elevmodule.h"
#include "Timer.h"
#include "Eventmanager.h"
#include "Queue.h"
#include "Statemachine.h"
#include <stdio.h>

static void elevator_init() { //MÅ DENNNE VÆRE STATIC?
    while(elev_get_floor_sensor_signal() == -1) {
        elev_set_motor_direction(DIRN_UP);
    }
    elev_set_motor_direction(DIRN_STOP);
    printf("Heisen er initialisert \n");
}

void fsm_EMERGENCY_STOP() {
    elev_set_stop_lamp(1); 
    elev_set_motor_direction(DIRN_STOP); 

    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){ //Setter etasjeindikatoren hvis heisen er i en etasje 
        elev_set_floor_indicator(floor); 
    }
}

void fsm_IDLE() {
    update_queue();
    queue_add_order();
    print_queue(); 

    elev_set_motor_direction(DIRN_STOP); //hvis den kommer fra emergency står den allerede stille, gjør det noe?

    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){ //Hvis heisen stopper i en etasje 
        elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
        //For å unngå knapper som ikke eksisterer
        if(floor != 3){
            elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
        }
        if(floor != 0){
            elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);

        }
        elev_set_floor_indicator(floor);
    }    

}

void fsm_RUN() {
    update_queue();
    queue_add_order();
    print_queue();

    if(elev_get_floor_sensor_signal() == -1){
        if(last_floor < orders[0].floor){
            elev_set_motor_direction(1);
        }
        if(last_floor > orders[0].floor){
            elev_set_motor_direction(-1);
        }
        if(last_floor==orders[0].floor){
            printf("last_dir%d\n", last_dir);
            if(last_dir==1){
            elev_set_motor_direction(-1);
            }
            if(last_dir==-1){
            elev_set_motor_direction(1);
            }
        }
    }
    else{
        elev_set_motor_direction(direction());
    }
}
     

void fsm_DOOR_OPEN() {
    elev_set_door_open_lamp(1);
    timer_start(); 
     
}


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    //Initialisering av heis, kø  og antall bestillinger 
    elevator_init(); 
    elev_set_floor_indicator(elev_get_floor_sensor_signal()); 
    queue_init(); 
    nOrders_init(); 
    print_queue(); 
    printf("Antall bestillinger i køen: %d\n", nOrders ); 

    state=IDLE;
    nextstate=0;

    print_queue();
    
    while (1) {

        update_queue();

        //Setter etasjeindikator når heisen kjører
        int floor = elev_get_floor_sensor_signal();
        
        if(floor != -1){
            elev_set_floor_indicator(floor); 
            last_floor = floor; 
            printf("last_floor%d\n", last_floor);
            if(floor<orders[0].floor){
                last_dir=1;
            }
            if(floor>orders[0].floor){
                last_dir=-1;
            }    
            
        }
        


        switch(state){
                    case IDLE:
                       if(ev_emergency_button_pushed()==1){
                            nextstate = EMERGENCY_STOP;
                        }
                        else if(ev_order_same_floor() == 1){
                            nextstate = DOOR_OPEN;
                        }
                        else if(ev_check_buttons()==1){
                            nextstate = RUN;
                        }
                        else{
                            nextstate = IDLE; //Trenger vi å ha med denne?
                        }
                        break;
                    case EMERGENCY_STOP:
                        if(ev_emergency_button_pushed()==1){
                            nextstate = EMERGENCY_STOP;
                        }
                        if(ev_emergency_button_pushed()==0){
                            int floor = elev_get_floor_sensor_signal();
                            if(floor != -1){ //er i en etasje
                                queue_delete_all_orders();
                                elev_set_stop_lamp(0);
                                elev_set_floor_indicator(floor); //kanskje denne må inn i ev_door_open()
                                nextstate = DOOR_OPEN;
                            }
                            else{
                                queue_delete_all_orders();
                                elev_set_stop_lamp(0);
                                nextstate = IDLE; 
                            }
                        }
                        break;
                    case RUN:         
                        if(ev_order_same_floor() == 0){
                            update_queue();
                            queue_add_order();
                            nextstate = RUN;
                            break;
                        }
                        if(ev_order_same_floor() == 1){
                            update_queue();
                            queue_add_order();
                            nextstate = IDLE;
                            break;
                        }
                        if(ev_emergency_button_pushed() == 1){
                            nextstate = EMERGENCY_STOP;
                            break;
                        }
                        break;
                    case DOOR_OPEN:
                        if(ev_time_door_out() == 0){
                            nextstate = DOOR_OPEN;
                        }
                        else{
                            update_queue();
                            elev_set_door_open_lamp(0); //LUKKER DØR NÅR TIDEN ER UTE
                            queue_delete_order();
                            nextstate = IDLE;
                        }
                        break;
                    default:
                        break;
        }

        //Hvis stoppknappen trykkes inn vil neste tilstand allti være EMERGENCY_STOP
        if(ev_emergency_button_pushed() == 1){
            nextstate = EMERGENCY_STOP;
        }
        

        if(state != nextstate){
            switch(nextstate){
                case IDLE:
                    fsm_IDLE();
                    break;
                case EMERGENCY_STOP:
                    fsm_EMERGENCY_STOP();
                    break;
                case RUN:   
                    fsm_RUN();
                    break;
                case DOOR_OPEN:
                    fsm_DOOR_OPEN();
                    break;
                default:
                    break;
            }
        }
        
        state = nextstate;
    
    }
    return 0;
}
