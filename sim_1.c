#include "main.h"
#include "variables.h"

void toggle_door_state(DoorSen* door_sen) {
    door_sen->isOpen = rand() % 2; // Randomly set door state to open or closed
}


void simulate_hot_gas_bypass_defrosting(float* pressure, float* temp, float* defrost_energy_consumed) {
    if (!pressure || !temp || !defrost_energy_consumed) {
        printf("Error: NULL pointer passed to simulate_hot_gas_defrosting\n");
        return;
    }

    float mass_flow_rate = 3.0; // kg/s
    
    float ambient_temp = read_ambient_temp();
    
    // Calculate enthalpies and energy
    float h_1 = calculate_enthalpy_evap(LOW_P, evap_temp, ambient_temp, superheating);
    float h_2 = calculate_enthalpy_com(HIGH_P, con_temp, ambient_temp);


    float hot_gas_power = calculate_hot_gas_power(mass_flow_rate, h_1, h_2, HOT_GAS_EFFICIENCY); // kW

    float initial_temp = *temp;
    float energy_consumed = 0.0;

    while (*temp < TARGET_TEMP) {
        *temp += TEMP_INCREASE;
        energy_consumed += hot_gas_power * DEFROST_STEP_DURATION / 3600.0; // Convert to Wh

        if (*temp >= TARGET_TEMP) {
            break;
        }
    }

    *defrost_energy_consumed += energy_consumed;
    printf("Defrost completed using hot gas. Energy consumed: %.2f Wh, Final temperature: %.2f°C\n", energy_consumed, *temp);
}



void simulate_fan_effect(HudSen* hud_sen) {
    float hud_decrease = random_float(1, 5);
    hud_sen->hud -= hud_decrease;
    if (hud_sen->hud < 0) {
        hud_sen->hud = 0;
    }
}

int read_hud(HudSen* hud_sen) {
    hud_sen->hud = random_float(30, 50);
    return hud_sen->hud;
}

float read_temp(TempSen* temp_sen) {
    return random_float(-18.0, -12.0);
}

void adjust_for_door_state(DoorSen* door_sen, float* temp_adjustment, int* door_open_time) {
    int max_sec = 8; // Define the maximum seconds the door is allowed to be open without extra temperature adjustment

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

/*
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TempSen* temp_sen) {
    if (hot_gas_bypass->isActive) {
        float temp_increase = 1.0;
        temp_sen->temp += temp_increase;
    }
}
*/

float read_ambient_temp() {
    return random_float(15.0, 35.0);
}
/*
float read_surrounding_temp() {
    return random_float(10.0, 30.0);
}*/


float read_ambient_hud() {
    return random_float(50.0, 60.0);
}
