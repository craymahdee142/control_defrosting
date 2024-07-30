#include "main.h"

float random_float(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}


void simulate_compressor_effect(float* temperature, float ambient_temperature, float* energy_consumed) {
    float temp_decrease = 2.0;
    if (ambient_temperature > 25.0) {
        temp_decrease -= (ambient_temperature - 25.0) * 0.05;
    } else if (ambient_temperature < 15.0) {
        temp_decrease += (15.0 - ambient_temperature) * 0.05;
    }
    *temperature -= temp_decrease;
    *energy_consumed += COMPRESSOR_POWER * CHECK_INTERVAL / 3600.0;
}

void simulate_condenser_effect(float* temperature, float surrounding_temperature) {
    float temp_decrease = 2.0;
    if (surrounding_temperature > 25.0) {
        temp_decrease -= (surrounding_temperature - 25.0) * 0.05;
    } else if (surrounding_temperature < 15.0) {
        temp_decrease += (15.0 - surrounding_temperature) * 0.05;
    }
    *temperature -= temp_decrease;
}

void simulate_evaporator_effect(float* temperature, float surrounding_temperature) {
    float temp_increase = 2.0;
    if (surrounding_temperature > 25.0) {
        temp_increase += (surrounding_temperature - 25.0) * 0.05;
    } else if (surrounding_temperature < 15.0) {
        temp_increase -= (15.0 - surrounding_temperature) * 0.05;
    }
    *temperature += temp_increase;
}




void simulate_expansion_valve_effect(ExpansionValve* expansion_valve) {
    if (expansion_valve->isOpen) {
        // Example logic to simulate expansion valve effect
    }
}

void simulate_solenoid_valve_effect(SolenoidValve* solenoid_valve) {
    if (solenoid_valve->isOn) {
        // Example logic to simulate solenoid valve effect
    }
}
