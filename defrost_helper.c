#include "main.h"
#include "variables.h"
/**
 * control_defrosting_conditions- a function that check and control defrosting
 * @temp_sen: temperature sensor to read tempearture
 * @hud_sen: temperature sensor to read humidity
 * @las_defrost_time: check for the time for the last defrost
 * Return: if defrost is needed
 */
int control_defrosting_conditions(TempSen* temp_sen, HudSen* hud_sen, int* defrost_needed, float* last_defrost_time) {
	float current_time = get_current_time();

	if (temp_sen->temp < -20.0 || hud_sen->hud > 60.0) {
        	if (current_time - *last_defrost_time >= MINIMUM_DEFROST_INTERVAL) {
            		*defrost_needed = 1;
        		*last_defrost_time = current_time;
        	}
        } else {
		*defrost_needed = 0;
    	}
    	return *defrost_needed;
}
