
/*elev_get_stop_signal() returnerer en int, 1 hvis stopp-lampen er p� og 0 hvis stopp-lampen er av.
*/
int ev_emergency_button_pushed();

/* Returnerer antall bestillinger i listen. Hvis 0 er det ingen knapper trykket inn. 
*/
int ev_check_buttons();

/* Returnerer true hvis du har bestilling i den etasjen du er i, og retningen er riktig 
*/
int ev_order_same_floor();

/* Returnerer retningen, -1 for ned, 0 og 1
*/ 
int direction();



//Returnerer true hvis tiden er ute

int ev_time_door_out();

