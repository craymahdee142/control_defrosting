#include "main.h"
#include "variables.h"

void toggle_door_state(DoorSen* door_sen) {
    door_sen->isOpen = rand() % 2; // Randomly set door state to open or closed
}


void simulate_hot_gas_bypass_defrosting(float pressure, float* con_temp, float* defrost_energy_consumed) {

    /*	if (!con_temp || !defrost_energy_consumed) {
        printf("Error: NULL pointer passed to simulate_hot_gas_bypass_defrosting\n");
        return;
    }*/

    float mass_flow_rate = 0.79; // kg/s
    float ambient_temp = read_ambient_temp(); // Assuming this function provides ambient temperature
    float evap_temp;

    // Calculate enthalpy values at evaporator and condenser
    float h_1 = calculate_enthalpy_evap(LOW_P, evap_temp, ambient_temp, superheating);
    float h_2 = calculate_enthalpy_com(HIGH_P, *con_temp, ambient_temp); 

    // Calculate hot gas power
    float hot_gas_power = calculate_hot_gas_power(mass_flow_rate, h_1, h_2, HOT_GAS_EFFICIENCY); // kW

    float initial_temp = *con_temp;
    float energy_consumed = 0.0;

    // Simulate temperature increase due to hot gas
    while (*con_temp < TARGET_TEMP) {
        *con_temp += TEMP_INCREASE;
        energy_consumed += hot_gas_power * DEFROST_STEP_DURATION / 3600.0; // Convert to Wh

        if (*con_temp >= TARGET_TEMP) {
            break;
        }
    }

    *defrost_energy_consumed += energy_consumed;
    printf("Defrost completed using hot gas. Energy consumed: %.2f Wh, Final temperature: %.2f°C\n", energy_consumed, *con_temp);
}



void simulate_fan_effect(HudSen* hud_sen) {
    float hud_decrease = random_float(1, 5);
    hud_sen->hud -= hud_decrease;
    if (hud_sen->hud < 0) {
        hud_sen->hud = 0;
    }
}

/*
float read_hud(HudSen* hud_sen) {
    if (hud_sen == NULL) {
	printf("Error: hud_sen is NULL\n");
        return -1.0f;
    }	
    return hud_sen->hud;
}


float read_temp(TempSen* temp_sen) {
    if (temp_sen == NULL) {
        // Handle null pointer error
        printf("Error: temp_sen is NULL\n");
        return -1.0f; // Return an error code
    }
    return temp_sen->temp;
}
*/

void adjust_for_door_state(DoorSen* door_sen, float* temp_adjustment, int* door_open_time) {
    int max_sec = 5; // Define the maximum seconds the door is allowed to be open without extra temperature adjustment

    if (door_sen->isOpen) {
        (*door_open_time)++; // Increment the door open time counter

        // Basic temperature adjustment when the door is open
        *temp_adjustment += 2.0;

        // Additional temperature adjustment based on how long the door has been open
        if (*door_open_time > max_sec) {
            *temp_adjustment += (*door_open_time - max_sec) * 0.5; // Increase adjustment for every second beyond max_sec
        }

        printf("Door state: OPEN for %d seconds\n", *door_open_time);
    } else {
        *door_open_time = 0; // Reset the door open time counter if the door is closed
        printf("Door state: CLOSED\n");
    }
}



float read_ambient_temp() {
    float base_ambient_temp = 25.0;
    float variation = random_float(-10.0, 10.0);
    return base_ambient_temp + variation;
}



float read_ambient_hud() {
    float base_ambient_hud = 40.0;
    float variation = random_float(-20.0, 20.0);
    return base_ambient_hud + variation;
}
