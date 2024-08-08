#include "main.h"

// Constants for base enthalpy values (example values, adjust based on pressure)
float h1_base = 533.989; // Enthalpy at evap outlet (kJ/kg) for pressure p1
float h2_base = 619.025; // Enthalpy at com outlet (kJ/kg) for pressure p2
float h3_base = 619.025; // Enthalpy at con inlet (kJ/kg) for pressure p3
float h4_base = 276.505; // Enthalpy at exp valve inlet (kJ/kg) for pressure p4

// Calculate hot gas power needed to defrost
double calculate_hot_gas_power(double mass_flow_rate, float h_1, float h_2, double efficiency) {
    float enthalpy_change = h_2 - h_1;
    float hot_gas_power = mass_flow_rate * enthalpy_change * efficiency;
    return hot_gas_power; // in kW
}


// Function to calculate enthalpy at evaporator outlet (h1)
float calculate_enthalpy_evap(float pressure, float evap_temp, float ambient_temp, float superheating) {
    float enthalpy;
    if (pressure == LOW_P) {
        enthalpy = h1_base + (evap_temp - ambient_temp) * 0.005 + superheating;
    } else {
        enthalpy = h1_base;
    }
	
    printf("Evaporator outlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Ambient Temp: %.2f°C)\n", enthalpy, pressure, ambient_temp);
    return enthalpy;
}

// Function to calculate enthalpy at compressor outlet (h2)
float calculate_enthalpy_com(float pressure, float con_temp, float ambient_temp) {
    float enthalpy;
    if (pressure == HIGH_P) {
        enthalpy = h2_base + (con_temp - ambient_temp) * 0.005;
    } else {
        enthalpy = h2_base;
    }
    printf("Compressor outlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Ambient Temp: %.2f°C)\n", enthalpy, pressure, ambient_temp);
    return enthalpy;
}

// Function to calculate enthalpy at condenser inlet (h3)
float calculate_enthalpy_con(float pressure, float con_temp, float subcooling, float ambient_temp) {
    float enthalpy;
    if (pressure == HIGH_P) {
        enthalpy = h3_base + (con_temp - ambient_temp) * 0.003 - subcooling;
    } else {
        enthalpy = h3_base;
    }
    printf("Condenser inlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Ambient_Temp: %.2f°C)\n", enthalpy, pressure, ambient_temp);
    return enthalpy;
}

// Function to calculate enthalpy at expansion valve inlet (h4)
float calculate_enthalpy_exp_valve(float pressure, float evap_temp) {
    float enthalpy;
    if (pressure == LOW_P) {
        enthalpy = h4_base + evap_temp * 0.003;
    } else {
        enthalpy = h4_base;
    }
    printf("Expansion valve inlet enthalpy: %.2f kJ/kg (Pressure: %.2f kPa, Evaporator Temp: %.2f°C)\n", enthalpy, pressure, evap_temp);

    return enthalpy;
}



/* Function to calculate COP*/
float calculate_cop(float h_1, float h_2, float total_heat_gain, float mass_flow_rate) {
    
     if (mass_flow_rate == 0) {
        return 0.0;
    }
    

    float W_com = mass_flow_rate * (h_2 - h_1);

    if (W_com == 0) {
        return 0.0;
    }

    float cop = total_heat_gain / W_com;
    return cop;
}

