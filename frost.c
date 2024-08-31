#include "main.h"
#include "variables.h"

// Calculate frost level based on current conditions
float get_frost_level(float current_temp, float current_hud, float target_temp_low, float target_temp_high, float target_humidity_low, float target_humidity_high) {
    float frost_level = 0.0;

    // Check if current temperature and humidity are within target ranges
    if (current_temp <= target_temp_high && current_temp >= target_temp_low) {
        if (current_hud >= target_humidity_low && current_hud <= target_humidity_high) {
            float temp_factor = (target_temp_high - current_temp) / (target_temp_high - target_temp_low);
            float humidity_factor = (current_hud - target_humidity_low) / (target_humidity_high - target_humidity_low);
            frost_level = temp_factor * humidity_factor * 100.0;

             printf("Temp Factor: %.2f, Humid Factor: %.2f, Frost Level: %.2f%%\n", temp_factor, humidity_factor, frost_level);
        }
    }

    // Debugging output if outside the range
    if (frost_level == 0.0) {
        printf("Conditions out of range: Current Temp: %.2f, Current Humidity: %.2f\n", current_temp, current_hud);
    }

    return frost_level;
}

// Check frost level and decide actions
void check_frost_and_defrost(float current_temp, float current_hud, float target_temp_low, float target_temp_high, float target_humidity_low, float target_humidity_high, SolValve* sol_valve, Com* com, Con* con, Evap* evap) {
      // Declare and initialize variables
    float defrost_energy_consumed = 0.0;
    float ambient_temp = read_ambient_temp(); 
    float frost_level = get_frost_level(current_temp, current_hud, target_temp_low, target_temp_high, target_humidity_low, target_humidity_high);

    if (frost_level >= FROST_THRESHOLD) {
        printf("Frost level is high (%.2f%%). Triggering defrost.\n", frost_level);
        // Call defrosting function
        simulate_hot_gas_bypass_defrosting(&current_temp, &defrost_energy_consumed, ambient_temp);
    } else {
        printf("Frost level is within acceptable range (%.2f%%).\n", frost_level);
    }
}

// Increase temperature by activating the solenoid valve and compressor
void increase_temperature(SolValve* sol_valve, Com* com, Con* con) {
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);  // Simulate the effect of activating the solenoid valve
    com->isOn = 1;  // Activate the compressor
    usleep(1000000);  // Wait for 1 second
    sol_valve->isOn = 0;  // Turn off the solenoid valve
    printf("Increasing temperature.\n");
}

// Decrease temperature by activating the compressor and condenser
void decrease_temperature(SolValve* sol_valve, Com* com, Con* con) {
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;  // Turn off the solenoid valve if it was on
        printf("Stopping hot gas bypass.\n");
    }
    com->isOn = 1;  // Activate the compressor
    con->isOn = 1;  // Activate the condenser
    printf("Decreasing temperature.\n");
}

// Increase humidity by activating the solenoid valve and compressor
void increase_humidity(SolValve* sol_valve, Com* com) {
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);  // Simulate the effect of activating the solenoid valve
    com->isOn = 1;  // Activate the compressor
    usleep(3000000);  // Wait for 3 seconds
    sol_valve->isOn = 0;  // Turn off the solenoid valve
    printf("Increasing humidity.\n");
}

// Decrease humidity by activating the evaporator and compressor
void decrease_humidity(SolValve* sol_valve, Evap* evap, Com* com) {
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;  // Turn off the solenoid valve if it was on
        printf("Stopping hot gas bypass.\n");
    }
    evap->isOn = 1;  // Activate the evaporator
    com->isOn = 1;  // Activate the compressor
    printf("Decreasing humidity.\n");
}

