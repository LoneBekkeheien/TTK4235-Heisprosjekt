/* Global variabel for � lagre tidspunktet i millisekunder n�r d�ren �pnes. Skal brukes til sammenligning.
*/
#include <time.h>
#include <stdio.h>
time_t timer_door_opened_ms;

/* Leser av tiden 1 Januar 1970, gj�r dette om fra sekunder til millisekunder (1s = 1000ms) og lagrer i timer_ms_door_opened.
*/
void timer_start();

/* Etter at d�ren er �pnet og tiden skrevet ned, sammenlignes denne med en variabel timer_now_md som hele tiden oppdateres. Etter 3s = 3000ms
er l�kken ferdig og true returneres. D�ren skal n� lukkes.
*/
int timer_3sec();