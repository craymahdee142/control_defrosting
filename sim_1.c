#include "main.h"
#include "variables.h"

void toggle_door_state(DoorSen* door_sen) {
    door_sen->isOpen = rand() % 2; // Randomly set door state to open or closed
}


void simulate_hot_gas_bypass_defrosting(float pressure, float* con_temp, float* defrost_energy_consumed) {

    /*  if (!con_temp || !defrost_energy_consumed) {
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


void adjust_for_door_state(TempSen* temp_sen, HudSen* hud_sen, DoorSen* door_sen) {
    static float door_open_start_time = 0.0f; // Static to persist across function calls
    static float accumulated_open_time = 0.0f; // Static to persist across function calls
    static int door_open_count = 0;

    float current_time = get_current_time(); // Get the current time in seconds

    if (door_sen->isOpen) {
        // Track the start time when the door is opened
        if (door_open_start_time == 0.0f) {
            door_open_start_time = current_time;
            door_open_count++;
            printf("Door opened %d times.\n", door_open_count);
        }

        // Calculate the duration the door has been open
        accumulated_open_time = current_time - door_open_start_time;

        // Additional adjustments based on the door open duration
        if (accumulated_open_time > 3.0f) { // More than 3 seconds
            float extra_temp_adjustment = (accumulated_open_time - 3.0f) * 0.5; // Example linear increment
            float extra_hud_adjustment = (accumulated_open_time - 3.0f) * 0.2; // Example linear increment

            temp_sen->temp += extra_temp_adjustment;
            hud_sen->hud += extra_hud_adjustment;
        }

        printf("Door state: OPEN for %.2f seconds\n", accumulated_open_time);
    } else {
        // Reset the door open time and accumulated time if the door is closed
        if (door_open_start_time != 0.0f) {
            printf("Door state: CLOSED after %.2f seconds\n", accumulated_open_time);
        }

        door_open_start_time = 0.0f;
        accumulated_open_time = 0.0f;
    }
}



float read_ambient_temp() {
    float base_ambient_temp = 25.0;
    float variation = random_float(-10.0, 10.0);
    float ambient_temp = base_ambient_temp + variation;

    printf("Ambient Temp: %.2f°C\n", ambient_temp);
    return ambient_temp;
}

float read_ambient_hud() {
    float base_ambient_hud = 55.0;
    float variation = random_float(-30.0, 30.0);
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
