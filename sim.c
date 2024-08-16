#include "main.h"
#include "variables.h"

float random_float(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}


// Simulate the compressor effect
void simulate_com_effect(float temp_sensor_reading, float* con_temp, float ambient_temp, float* energy_consumed) {
    // Adjust energy consumption based on the simulation interval
    *energy_consumed += COMPRESSOR_POWER * CHECK_INTERVAL / 3600.0;
}


// Simulate the condenser effect with subcooling
void simulate_con_effect(float temp_sensor_reading, float* con_temp, float ambient_temp, float subcooling) {
    // Define a local baseline temperature for the condenser
    float baseline_temp = 40.0f;  // Assuming 40.0°C as the baseline temperature for the condenser

    // Reset the condenser temperature with subcooling
    *con_temp = baseline_temp - subcooling;

    // Adjust based on ambient temperature
    if (ambient_temp > 35.0) {
        *con_temp -= (ambient_temp - 35.0) * 0.05;
    } else if (ambient_temp < 15.0) {
        *con_temp += (15.0 - ambient_temp) * 0.05;
    }
}



// Simulate the evaporator effect based on surrounding temp and superheating
void simulate_evap_effect(float temp_sensor_reading, float* evap_temp, float ambient_temp, float superheating) {
    // Define a local baseline temperature
    float baseline_temp = -25.0f;

    // Apply superheating to the baseline temperature
    *evap_temp = baseline_temp + superheating;

    // Adjust based on surrounding temperature
    if (ambient_temp > 35.0) {
        *evap_temp += (ambient_temp - 35.0) * 0.05;
    } else if (ambient_temp < 15.0) {
        *evap_temp -= (15.0 - ambient_temp) * 0.05;
    }
}



void simulate_exp_valve_effect(ExpValve* exp_valve) {
    if (exp_valve->isOpen) {
        // Example logic to simulate exp valve effect
    }
}

void simulate_sol_valve_effect(SolValve* sol_valve) {
    if (sol_valve->isOn) {
        // Example logic to simulate sol valve effect
    }
}
