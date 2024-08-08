#include "main.h"
#include "variables.h"


void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen) {
    temp_sen->temp = read_temp(temp_sen);
    door_sen->isOpen = 0;
    hud_sen->hud = read_hud(hud_sen);
}

void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
    // Declare and initialize variables
    float pressure = LOW_P;
    float temp = 20; // Initialize with appropriate value, about 5 degrees
    float defrost_energy_consumed = 0.0;
    float last_defrost_time = 0;
    int defrost_needed = 0;
    int defrost_count = 0;
    int check_interval = 5; // Example interval in seconds
    float minimum_defrost_interval = 3600; // Example minimum interval in seconds
    float ambient_temp = read_ambient_temp();

    // Read current sensor values
    float current_temp = read_temp(temp_sen);
    float current_hud = read_hud(hud_sen);

    // Define target ranges for temperature and humidity
    float target_temp_low = -18.0;
    float target_temp_high = -16.0;
    float target_humidity_low = 50.0;
    float target_humidity_high = 60.0;

    
    if (superheating < MIN_SUPERHEATING) {
        exp_valve->isOpen = 1;
    } else if (superheating > MAX_SUPERHEATING) {
        exp_valve->isOpen = 0;
    }

    if (subcooling < MIN_SUBCOOLING) {
        con->isOn = 1;
    } else if (subcooling > MAX_SUBCOOLING) {
        con->isOn = 0;
    }

    // Check frost level
    float frost_level = get_frost_level(current_temp, current_hud, target_temp_low, target_temp_high, target_humidity_low, target_humidity_high);

    if (frost_level < 20.0) { // If frost level is less than 20%
        if (current_temp > target_temp_high || current_hud < target_humidity_low) {
            // Take corrective action
            if (current_temp > target_temp_high) {
                printf("Temperature is too high. Initiating cooling.\n");
                decrease_temperature(sol_valve, com, con);
                com->isOn = 1;
                con->isOn = 1;
            }
            if (current_hud < target_humidity_low) {
                printf("Humidity is too low. Increasing humidity.\n");
                increase_humidity(sol_valve, com);
                evap->isOn = 1;
            }
        } else if (current_temp < target_temp_low || current_hud > target_humidity_high) {
            // Take corrective action
            if (current_temp < target_temp_low) {
                printf("Temperature is too low. Raising temperature.\n");
                increase_temperature(sol_valve, com, con);
            }
            if (current_hud > target_humidity_high) {
                printf("Humidity is too high. Reducing humidity.\n");
                decrease_humidity(sol_valve, evap, com);
            }
        } else {
            // General action if both temperature and humidity are acceptable
            printf("Adjusting temperature and humidity to manage frost level.\n");
            com->isOn = 0;
            con->isOn = 0;
            evap->isOn = 0;
        }
    }

    // Initiate defrosting if needed
    int i = 0; // Example iteration variable, replace with actual loop index if used in a loop
    if ((current_temp <= -18.0 || current_hud > target_humidity_high) &&
        (i * check_interval - last_defrost_time >= minimum_defrost_interval)) {
        printf("Defrost conditions met. Temperature: %.2f°C, Humidity: %.2f%%\n", current_temp, current_hud);
        defrost_needed = 1;
        last_defrost_time = i * check_interval;
    }

    if (defrost_needed) {
        simulate_hot_gas_bypass_defrosting(&pressure, &temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed; // Accumulate total energy consumed
        defrost_needed = 0;
        defrost_count++;
    }

    // Simulate effects of various components
    if (com->isOn) {
        simulate_com_effect(&temp, ambient_temp, energy_consumed);
    }
    if (evap->isOn) {
        simulate_evap_effect(&temp, ambient_temp, superheating);
    }
    if (exp_valve->isOpen) {
        simulate_exp_valve_effect(exp_valve);
    }
    if (sol_valve->isOn) {
        simulate_sol_valve_effect(sol_valve);
    }
    if (hot_gas_bypass->isActive) {
        simulate_hot_gas_bypass_defrosting(&pressure, &temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed; // Accumulate total energy consumed
    }
}

