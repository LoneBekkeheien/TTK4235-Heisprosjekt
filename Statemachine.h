enum state{
    IDLE=0,
    RUN = 1,
    EMERGENCY_STOP = 2,
    DOOR_OPEN = 3,
} state, nextstate;

int last_floor = N_FLOORS + 1;
int last_dir=N_FLOORS+1;

//Heisen initialiseres ved å kjøre OPP til nærmeste etasje, hvis ikke allerede i en etasje 
static void elevator_init();

//Funksjonene gjør endringer når heisen bytter state
void fsm_EMERGENCY_STOP();
void fsm_IDLE();
void fsm_RUN();
void fsm_DOOR_OPEN();