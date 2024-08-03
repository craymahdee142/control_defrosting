#include "main.h"


// Constants for base enthalpy values (example values, adjust based on pressure)
const float h1_base = 251.9; // Enthalpy at evaporator outlet (kJ/kg) for pressure p1
const float h2_base = 279.3; // Enthalpy at compressor outlet (kJ/kg) for pressure p2
const float h3_base = 117.8; // Enthalpy at condenser inlet (kJ/kg) for pressure p3
const float h4_base = 117.8; // Enthalpy at expansion valve inlet (kJ/kg) for pressure p4

/* Cal hot gas needed to defrost */
double calculate_hot_gas_power(double mass_flow_rate, float h_1, float h_2, double efficiency) {
    float enthalpy_change = h_2 - h_1;
    float hot_gas_power = mass_flow_rate * enthalpy_change * efficiency;
    return hot_gas_power; // in kW
}



// Function to calculate enthalpy at evaporator outlet (h1)
float calculate_enthalpy_evaporator(float pressure, float ambient_temperature) {
    float enthalpy;
    if (pressure == LOW_P) {
        enthalpy = h1_base + ambient_temperature * 0.005;
    } else if (pressure == HIGH_P) {
        enthalpy = h1_base + ambient_temperature * 0.005; // Modify if different for high pressure
    } else {
        enthalpy = h1_base; // Default or interpolated value if needed
    }
    printf("Evaporator outlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Ambient Temp: %.2f°C)\n", enthalpy, pressure, ambient_temperature);
    return enthalpy;
}

// Function to calculate enthalpy at compressor outlet (h2)
float calculate_enthalpy_compressor_outlet(float pressure, float ambient_temperature) {
    float enthalpy;
    if (pressure == HIGH_P) {
        enthalpy = h2_base + ambient_temperature * 0.005;
    } else {
        enthalpy = h2_base; // Default value for low pressure or other cases
    }
    printf("Compressor outlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Ambient Temp: %.2f°C)\n", enthalpy, pressure, ambient_temperature);
    return enthalpy;
}

// Function to calculate enthalpy at condenser inlet (h3)
float calculate_enthalpy_condenser(float pressure, float surrounding_temperature) {
    float enthalpy;
    if (pressure == HIGH_P) {
        enthalpy = h3_base + surrounding_temperature * 0.003;
    } else {
        enthalpy = h3_base; // Default or interpolated value if needed
    }
    printf("Condenser inlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Surrounding Temp: %.2f°C)\n", enthalpy, pressure, surrounding_temperature);
    return enthalpy;
}

// Function to calculate enthalpy at expansion valve inlet (h4)
float calculate_enthalpy_expansion_valve(float pressure, float surrounding_temperature) {
    float enthalpy;
    if (pressure == LOW_P) {
        enthalpy = h4_base + surrounding_temperature * 0.003;
    } else {
        enthalpy = h4_base; // Default or interpolated value if needed
    }
    printf("Expansion valve inlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Surrounding Temp: %.2f°C)\n", enthalpy, pressure, surrounding_temperature);
    return enthalpy;
}

// Function to calculate COP
float calculate_cop(float h_1, float h_2, float h_3, float h_4, float mass_flow_rate) {
    float Q_evap = mass_flow_rate * (h_1 - h_4);  // Cooling capacity in the evaporator
    float W_comp = mass_flow_rate * (h_2 - h_1);

    if (W_comp == 0) {
        return 0.0;
    }

    float cop = Q_evap / W_comp;
    return cop;
}

