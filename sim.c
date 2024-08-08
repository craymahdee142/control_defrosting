#include "main.h"

float random_float(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}


// Simulate the compressor effect
void simulate_com_effect(float* evap_temp, float ambient_temp, float* energy_consumed) {
    // Adjust energy consumption based on the simulation interval
    *energy_consumed += COMPRESSOR_POWER * CHECK_INTERVAL / 3600.0;
}


// Simulate the condenser effect with subcooling
void simulate_con_effect(float* cond_temp, float ambient_temp, float subcooling) {
    // Apply subcooling to the cond temp
    *cond_temp -= subcooling;

    // Adjust based on ambient temp
    float temp_adjustment = 2.0;
    if (ambient_temp > 25.0) {
        temp_adjustment -= (ambient_temp - 25.0) * 0.05;
    } else if (ambient_temp < 15.0) {
        temp_adjustment += (15.0 - ambient_temp) * 0.05;
    }
    *cond_temp -= temp_adjustment;
}

// Simulate the evaporator effect based on surrounding temp and superheating
void simulate_evap_effect(float* evap_temp, float ambient_temp, float superheating) {
    // Apply superheating to the evap temp
    *evap_temp += superheating;

    // Adjust based on surrounding temp
    float temp_increase = 2.0;
    if (ambient_temp > 25.0) {
        temp_increase += (ambient_temp - 25.0) * 0.05;
    } else if (ambient_temp < 15.0) {
        temp_increase -= (15.0 - ambient_temp) * 0.05;
    }
    *evap_temp += temp_increase;
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
