#include "main.h"
#include "variables.h"
/*
void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
    // Declare and initialize variables
    static float last_defrost_time = 0;
    int defrost_needed = 0;
    float minimum_defrost_interval = 10; // seconds
    float high_pressure = HIGH_P; 
    float defrost_energy_consumed = 0.0; // Initialize defrost_energy_consumed
    static int defrost_count = 0; // Declare defrost_count here

    // Read current sensor values
    float current_temp = read_hud(hud_sen, &door_sen, 0, ambient_humidity);
    float current_hud = read_hud(hud_sen, &door_sen, ambient_humidity);
    float ambient_temp = read_ambient_temp();
    float ambient_hud = read_ambient_hud();
*/

void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
    // Declare and initialize variables
    static float last_defrost_time = 0;
    int defrost_needed = 0;
    float minimum_defrost_interval = 10; // seconds
    float high_pressure = HIGH_P; 
    float defrost_energy_consumed = 0.0; // Initialize defrost_energy_consumed
    static int defrost_count = 0; // Declare defrost_count here

    // Initialize ambient variables
    float ambient_temp = read_ambient_temp();
    float ambient_hud = read_ambient_hud();

    // Read current sensor values
    float current_temp = read_temp(temp_sen, com, door_sen, 0, ambient_temp); // Updated to use read_temp
    float current_hud = read_hud(hud_sen, door_sen, ambient_hud); // Updated to use read_hud correctly



    // Ensure sensor readings are within expected ranges
    if (current_temp < -30.0 || current_temp > -10.0) {
        fprintf(stderr, "Error: Temperature reading out of range: %.2f°C\n", current_temp);
        return;
    }
    if (current_hud < 0.0 || current_hud > 100.0) {
        fprintf(stderr, "Error: Humidity reading out of range: %.2f%%\n", current_hud);
        return;
    }

    // Control expansion valve
    if (superheating < MIN_SUPERHEATING) {
        exp_valve->isOpen = 1;
    } else if (superheating > MAX_SUPERHEATING) {
        exp_valve->isOpen = 0;
    }

    // Control condenser
    if (subcooling < MIN_SUBCOOLING) {
        con->isOn = 1;
    } else if (subcooling > MAX_SUBCOOLING) {
        con->isOn = 0;
    }

    // Check frost level
    float frost_level = get_frost_level(current_temp, current_hud, target_temp_low, target_temp_high, target_humidity_low, target_humidity_high);

    if (frost_level < 20.0) {
        // Control temperature and humidity
        control_temperature(temp_sen, sol_valve, com, con);
        control_humidity(hud_sen, sol_valve, evap, com);
    } else {
        // General action if frost level is too high
        printf("Adjusting temperature and humidity to manage frost level.\n");
        com->isOn = 0;
        con->isOn = 0;
        evap->isOn = 0;
    }

    // Check if defrosting is needed
    if ((current_temp <= -18.0 || current_hud > target_humidity_high) &&
        (get_current_time() - last_defrost_time >= minimum_defrost_interval)) {
        printf("Defrost conditions met. Temperature: %.2f°C, Humidity: %.2f%%\n", current_temp, current_hud);
        defrost_needed = 1;
        last_defrost_time = get_current_time(); // Update last defrost time
    }

    // Perform defrost if needed
    if (defrost_needed) {
        defrost_count++;
        simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed;
    }

    // Simulate effects of various components
    if (com->isOn) {
        simulate_com_effect(temp_sen->temp, &con_temp, ambient_temp, energy_consumed);
    }
    if (evap->isOn) {
        simulate_evap_effect(temp_sen->temp, &evap_temp, ambient_temp, superheating);
    }
    if (con->isOn) {
        simulate_con_effect(temp_sen->temp, &con_temp, ambient_temp, subcooling);
    }
    if (exp_valve->isOpen) {
        simulate_exp_valve_effect(exp_valve);
    }
    if (sol_valve->isOn) {
        simulate_sol_valve_effect(sol_valve);
    }
    if (hot_gas_bypass->isActive) {
        simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed; // Accumulate total energy consumed
    }
}

