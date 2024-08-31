#include "main.h"
#include "variables.h"

/**
 * initialize_sensors - initiaze sensors
 * @temp_sen: temperature sensor to read tempearture
 * @hud_sen: humidity sensor to read humidity
 * @door_sen: dooe sensor to read open or close of the freezer door
 */
void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen) {
        door_sen->isOpen = 0;
        temp_sen->temp = TARGET_TEMP_HIGH; /*Initial temperature*/
        hud_sen->hud = TARGET_HUMIDITY_LOW + 5.0; 
        printf("Initial Sensor Temp: %.2fC\n", temp_sen->temp);
        printf("Initial Sensor Humidity: %.2f%%\n", hud_sen->hud);
}

/**
 * update_sensors - function to update the sensors
 */
void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, float ambient_temp, float ambient_humidity) {
        temp_sen->temp = read_temp(); 
        door_sen->isOpen = read_door_sen(door_sen);
        hud_sen->hud = read_hud();
        printf("Updated Sensor Temp: %.2fC\n", temp_sen->temp);
        printf("Updated Sensor Humidity: %.2f%%\n", hud_sen->hud);
}

/* Get the current time in seconds*/
float get_current_time() {
	return (float)time(NULL);
}

/**
 * read_hud - a function that read humidity inside the compartment
 * Return: returns humidity readings
 */
float read_hud() {
	float base_hud = 55.0;  // Base humidity value
        float variation = random_float(-8.0, 10.0);  /*Random variation between -8.0 and 8.0*/
        float humidity = base_hud + variation;  

        printf("Humidity Sensor Reading: %.2f%%\n", humidity);
        return humidity;
}

/**
 * read_temp - a function that read tempearture inside the compartment
 * Return: returns tempearture readings
 */
float read_temp() {
        float base_temp = -18.0;  
        float variation = random_float(-4.0, 5.0);  
        float temp_reading = base_temp + variation;  

        printf("Temperature Reading: %.2fC\n", temp_reading);
        return temp_reading;
}

/**
 * read_door_sen - a function that read whether door is open or close
 * Return: returns tempearture readings
 */
bool read_door_sen(DoorSen* door_sen) {
        float variation = random_float(-0.1, 0.1);  
        int door_state = (door_sen->isOpen + (variation > 0.05 ? 1 : 0)) % 2;
 
        door_sen->isOpen = door_state;
        printf("Door Sensor Reading: %s\n", door_state ? "Open" : "Closed");

        return door_state;
}

/**
 * control_temperature - a function that control the temperature
 * @temp_sen: temperature sensor to read tempearture
 * @sol_valve: opens or closes to allow redirect of refrigerant
 */
void control_temperature(TempSen* temp_sen, SolValve* sol_valve, Com* com, Con* con) {
        if (temp_sen->temp > TARGET_TEMP_HIGH) {
        	printf("Temperature is too high. Decreasing temperature.\n");
        	decrease_temperature(sol_valve, com, con);
        } else {
        	printf("Temperature is within the target range or too low. Maintaining current state.\n");
        	sol_valve->isOn = 0;
        	com->isOn = 0;
        	con->isOn = 0;
        }
}

/**
 * control_humidity - a function that control the humidity
 * @hud_sen: temperature sensor to read tempearture
 * @sol_valve: opens or closes to allow redirect of refrigerant
 */
// Function to control humidity within target range
void control_humidity(HudSen* hud_sen, SolValve* sol_valve, Evap* evap, Com* com) {
        if (hud_sen->hud < TARGET_HUMIDITY_LOW) {
        	printf("Humidity is too low. Increasing humidity.\n");
        	increase_humidity(sol_valve, com);
    	} else if (hud_sen->hud > TARGET_HUMIDITY_HIGH) {  
        	printf("Humidity is too high. Decreasing humidity.\n");
        	decrease_humidity(sol_valve, evap, com);
    	} else {
        	 printf("Humidity is within the target range. Maintaining current state.\n");
         	 sol_valve->isOn = 0;
        	evap->isOn = 0;
        	com->isOn = 0;
        }
}


