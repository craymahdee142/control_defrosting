#include "main.h"

float get_frost_level(float current_temp, float current_hud, float target_temp_low, float target_temp_high, float target_humidity_low, float target_humidity_high) {
    float frost_level = 0.0;

    // Check if the temperature is within the range where frost could form
    if (current_temp <= target_temp_high && current_temp >= target_temp_low) {
        // Check if humidity is also within the critical range
        if (current_hud >= target_humidity_low && current_hud <= target_humidity_high) {
            // Calculate frost level based on deviation from ideal conditions
            float temp_factor = (target_temp_high - current_temp) / (target_temp_high - target_temp_low);
            float humidity_factor = (current_hud - target_humidity_low) / (target_humidity_high - target_humidity_low);
            frost_level = temp_factor * humidity_factor * 100.0;  // Scale to percentage
        }
    }

    return frost_level;
}




// Function to increase temperature
void increase_temperature(SolValve* sol_valve, Com* com, Con* con) {
    // Activate solenoid valve to release hot gas
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);  // Redirect hot gas to increase temperature

    // Keep the compressor running to maintain system operation
    com->isOn = 1;

    // Wait for the timer to expire before turning off the solenoid valve
    usleep(1000000);  // 1 second delay

    // Turn off the solenoid valve after the hot gas has been released
    sol_valve->isOn = 0;

    printf("Increasing temperature using hot gas bypass with compressor active, solenoid valve turned off after timer.\n");
}

// Function to decrease temperature
void decrease_temperature(SolValve* sol_valve, Com* com, Con* con) {
    // Ensure the solenoid valve is off to stop hot gas bypass
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;
        printf("Solenoid valve turned off to stop hot gas bypass.\n");
    }

    // Increase compressor activity to lower temperature
    com->isOn = 1;

    // Improve airflow using fans to assist cooling
    con->isOn = 1;  // Turn on fans to help with temperature reduction

    printf("Decreasing temperature by increasing compressor activity and improving airflow.\n");
}

// Function to increase humidity
void increase_humidity(SolValve* sol_valve, Com* com) {
    // Activate solenoid valve to release hot gas and increase humidity
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);

    // Ensure compressor remains on during hot gas release
    com->isOn = 1;

    // Wait for the timer to expire before turning off the solenoid valve
    usleep(3000000);  // 3 seconds delay

    // Turn off the solenoid valve after the hot gas has been released
    sol_valve->isOn = 0;

    printf("Increasing humidity using hot gas bypass with compressor active, solenoid valve turned off after timer.\n");
}

// Function to decrease humidity
void decrease_humidity(SolValve* sol_valve, Evap* evap, Com* com) {
    // Ensure the solenoid valve is off to stop hot gas bypass
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;
        printf("Solenoid valve turned off to stop hot gas bypass.\n");
    }

    evap->isOn = 1;  // Increase evaporator's dehumidifying activity

    // Ensure compressor remains active to support dehumidification
    com->isOn = 1;

    printf("Decreasing humidity by enhancing dehumidification and maintaining compressor activity.\n");
}

