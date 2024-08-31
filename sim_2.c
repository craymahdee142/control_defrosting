#include "main.h"
#include "variables.h"

/**
 * control_system - function to control the components
 */
void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
        static float last_defrost_time = 0;
        static int defrost_in_progress = 0;
        float minimum_defrost_interval = 35.0;
        float defrost_energy_consumed = 0.0;
        static int defrost_count = 0;

        float com_energy_consumed = 0.0;
        float energy_absorbed = 0.0;
        float energy_rejected = 0.0;

        float ambient_temp = read_ambient_temp();
        float ambient_humidity = read_ambient_hud();

        update_sensors(temp_sen, door_sen, hud_sen, com, ambient_temp, ambient_humidity);

        /* Ensure sensor readings are within expected ranges*/
        if (temp_sen->temp < -30.0 || temp_sen->temp > -10.0) {
        fprintf(stderr, "Error: Temperature reading out of range: %.2fC\n", temp_sen->temp);
        return;
}

if (hud_sen->hud < 0.0 || hud_sen->hud > 100.0) {
        fprintf(stderr, "Error: Humidity reading out of range: %.2f%%\n", hud_sen->hud);
        return;
}

/* Determine if defrost conditions are met*/
float current_time = get_current_time();
if ((temp_sen->temp < TARGET_TEMP_LOW || temp_sen->temp > TARGET_TEMP_HIGH ||
	hud_sen->hud < TARGET_HUMIDITY_LOW || hud_sen->hud > TARGET_HUMIDITY_HIGH) &&
        (current_time - last_defrost_time >= minimum_defrost_interval) && !defrost_in_progress) {
        printf("Defrost conditions met. Temperature: %.2fC, Humidity: %.2f%%\n", temp_sen->temp, hud_sen->hud);
        defrost_in_progress = 1;
        last_defrost_time = current_time;
} 

if (defrost_in_progress) {
	simulate_hot_gas_bypass_defrosting(&temp_sen->temp, &defrost_energy_consumed, ambient_temp);
        *energy_consumed += defrost_energy_consumed;
        defrost_count++;

        // Check if defrost is complete and reset the flag
        if (temp_sen->temp >= -18.0) { 
                defrost_in_progress = 0;
                printf("Defrost complete. Total cycles: %d\n", defrost_count);
        }
}

com->isOn = 1;
/* Control temperature based on thresholds*/
if (temp_sen->temp > -18.0) {
	printf("Temperature is above -18.0C. Decreasing temperature.\n");
        decrease_temperature(sol_valve, com, con);
} else if (temp_sen->temp < -20.0) {
        printf("Temperature is below -20.0C. Increasing temperature.\n");
        increase_temperature(sol_valve, com, con);
} else {
        printf("Temperature is within the target range. Maintaining current state.\n");
        sol_valve->isOn = 0;
        com->isOn = 0;
        
}

/*Control humidity based on thresholds*/
if (hud_sen->hud < 50.0) {
        printf("Humidity is below 50.0%%. Increasing humidity.\n");
        increase_humidity(sol_valve, com);
} else if (hud_sen->hud > 60.0) {
        printf("Humidity is above 60.0%%. Decreasing humidity.\n");
        decrease_humidity(sol_valve, evap, com);
} else {
        printf("Humidity is within the target range. Maintaining current state.\n");
        sol_valve->isOn = 0;
        evap->isOn = 0;
        
}

/*Simulate effects of various components*/
if (com->isOn) {
        simulate_com_effect(temp_sen->temp, ambient_temp, &com_energy_consumed);
        *energy_consumed += com_energy_consumed;
}
if (evap->isOn) {
        simulate_evap_effect(ambient_temp, &energy_absorbed);
}
if (con->isOn) {
        simulate_con_effect(ambient_temp, &energy_rejected);
}
if (exp_valve->isOpen) {
        simulate_exp_valve_effect(ambient_temp, &temp_sen->temp, energy_consumed);
}
if (sol_valve->isOn) {
        simulate_sol_valve_effect(sol_valve);
}
} 



