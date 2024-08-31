#include "main.h"
#include "variables.h"

/*Randomly set door state to open or closed*/
void toggle_door_state(DoorSen* door_sen) {
        door_sen->isOpen = rand() % 2; 
}


/**
 * adjust_for_door_state - function that gives the door state
 * @temp_sen: Temperature sensor
 * @hud_sen: humidity sensor
 * @door_sen: door sensor
 */
void adjust_for_door_state(TempSen* temp_sen, HudSen* hud_sen, DoorSen* door_sen) {
        static float door_open_start_time = 0.0f;
        static float accumulated_open_time = 0.0f;
        static int door_open_count = 0;

        float current_time = get_current_time(); 

        if (door_sen->isOpen) {
        	/*Track the start time when the door is opened*/
        	if (door_open_start_time == 0.0f) {
            		door_open_start_time = current_time;
            		door_open_count++;
            		printf("Door opened %d times.\n", door_open_count);
        	}

                /*Calculate the duration the door has been open*/
        	accumulated_open_time = current_time - door_open_start_time;

        	if (accumulated_open_time > 3.0f) { 
            		float extra_temp_adjustment = (accumulated_open_time - 3.0f) * 0.5; 
            		float extra_hud_adjustment = (accumulated_open_time - 3.0f) * 0.2; 

            	  	temp_sen->temp += extra_temp_adjustment;
            		hud_sen->hud += extra_hud_adjustment;
        	}

        	printf("Door state: OPEN for %.2f seconds\n", accumulated_open_time);
    	} else {
        	/*Reset the door open time and accumulated time if the door is closed*/
        	if (door_open_start_time != 0.0f) {
                printf("Door state: CLOSED after %.2f seconds\n", accumulated_open_time);
        }

        door_open_start_time = 0.0f;
        accumulated_open_time = 0.0f;
       }
}


/**
 * read_ambient_temp - function that generates random ambient temp
 * Return: returns the ambient temp readings 
 */
float read_ambient_temp() {
        float base_ambient_temp = 22.5;
        float variation = random_float(-7.5, 7.5);
        float ambient_temp = base_ambient_temp + variation;

        if (ambient_temp < -5.0) ambient_temp = -5.0;
        if (ambient_temp > 35.0) ambient_temp = 35.0;

        printf("Ambient Temp: %.2fC\n", ambient_temp);
        return ambient_temp;
}


/**
 * read_ambient_hud - function that generates random ambient hud
 * Return: returns the ambient hud readings 
 */
float read_ambient_hud() {
        float base_ambient_hud = 55.0;
        float variation = random_float(-40.0, 40.0);
        float ambient_hud = base_ambient_hud + variation;

        // Ensure humidity is within 0% to 100%
        if (ambient_hud < 0.0) {
        ambient_hud = 0.0;
        } else if (ambient_hud > 100.0) {
        	ambient_hud = 100.0;
        }


        printf("Ambient Humidity: %.2f%%\n", ambient_hud);
        return ambient_hud;
}


void simulate_fan_effect(HudSen* hud_sen) {
        float hud_decrease = random_float(1, 5);
        hud_sen->hud -= hud_decrease;
    
	if (hud_sen->hud < 0) {
        	hud_sen->hud = 0;
        }
}

