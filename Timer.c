#include "Timer.h"
#include "Queue.h"

void timer_start() {
	timer_door_opened_ms = time(NULL) * 1000;
}

int timer_3sec() {
	time_t timer_now_ms = time(NULL) * 1000;

	while (timer_now_ms < timer_door_opened_ms + 3000) {
		timer_now_ms = time(NULL) * 1000; //oppdaterer tiden
        queue_add_order();
	}
	return 1;
}

