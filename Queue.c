#include "Queue.h"
#include "Elevmodule.h"
#include <stdio.h>

void nOrders_init(){
	nOrders=0;
	printf("Antallet bestillinger i køen er initialisert \n");
}

void queue_init(){
	for(int i=0; i<orderButtons; i++){
        orders[i].floor=(orderButtons+1);
        orders[i].direction=(orderButtons+1); 
    }    
    printf("Køen er initialisert \n");
}

void print_queue(){
	printf("-------------------------------------\n");
	for(int i=0; i<orderButtons; i++)
     {
        printf("Bestilling: etasje %d", orders[i].floor);
        printf(", retning %d \n", orders[i].direction);
     }
}

order queue_next_order(){
	return orders[0];	
}

void update_queue(){ //OPPDATERER LISTEN SLIK AT ALLE BESTILLINGENE SKYVES MOT HØYRE, UNNGÅR DA "HULL" I LISTEN
	for(int i = 0; i < orderButtons; i++){
		if(orders[i].floor == orderButtons + 1){ //trenger vi egentlig plusse på 1?
			for(int j = i+1; j < orderButtons; j++){
				if(orders[j].floor != orderButtons+1){
					orders[i].floor = orders[j].floor;
					orders[i].direction = orders[j].direction;
					orders[j].floor = orderButtons + 1;
					orders[j].direction = orderButtons + 1;
					break;
				}
			}
		}
	}
}


int order_not_same(int direction, int floor1){
	for(int i=0; i<nOrders; i++){
		if(orders[i].direction==direction && orders[i].floor==floor1){
			return 0;
		}
	}
	return 1;
}


void queue_add_order(){ 
	for(int i=0; i< N_FLOORS; i++) {
		if(elev_get_button_signal(BUTTON_COMMAND,i)==1){
			if(order_not_same(0,i)==1){
				orders[nOrders].floor=i;
				orders[nOrders].direction=0;
				elev_set_button_lamp(BUTTON_COMMAND, i, 1);
				nOrders+=1;
			}
		}
	}
	
	for(int i = 0; i < N_FLOORS-1; i++){
		if(elev_get_button_signal(BUTTON_CALL_UP,i)==1){
			if(order_not_same(1,i)==1){
				orders[nOrders].floor=i;
				orders[nOrders].direction=1;
				elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
				nOrders+=1;
			}
		}
	}

	for(int i = 1; i < N_FLOORS; i++){
		if(elev_get_button_signal(BUTTON_CALL_DOWN,i)==1){	
			if(order_not_same(-1,i)==1){
					orders[nOrders].floor=i;
					orders[nOrders].direction=-1;
					elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
					nOrders+=1;
			}
		}	
	}
	
}

void queue_delete_order(){ //skal slette alt i etasjen
	for(int i=0; i<orderButtons; i++){
		if(elev_get_floor_sensor_signal()==orders[i].floor){
			orders[i].floor=orderButtons+1;
			orders[i].direction=orderButtons+1;
			nOrders-=1; //KAN BLI -1 HVIS INGEN BESTILLINGER INNE
		}
	}
	printf("Bestillingen er fjernet fra køen \n");
	print_queue(); //for å se at bestillingen er fjernet
}

void queue_delete_all_orders(){
	for(int i=0; i<orderButtons; i++){
		orders[i].floor= orderButtons +1;
		orders[i].direction=orderButtons +1;
	}
	nOrders=0;	
	for(int i = 0; i < 4; i++){
        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
	}
	for(int i = 0; i < 3; i++){
		elev_set_button_lamp(BUTTON_CALL_UP, i, 0); 
	}
	for(int i =1 ; i < 4; i++){
        elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
	}
	printf("Hele køen er fjernet \n");
}




