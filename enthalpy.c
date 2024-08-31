#include "main.h"
#include "variables.h"

/*base enthalpy values*/
float h1_base = 541.28; /*Enthalpy at evap outlet (kJ/kg) for pressure p1*/
float h2_base = 621.63; /*Enthalpy at com outlet (kJ/kg) for pressure p2*/
float h3_base = 270.62; /*Enthalpy at con inlet (kJ/kg) for pressure p3*/
float h4_base = 270.62; /*Enthalpy at exp valve inlet (kJ/kg) for pressure p4*/

float s1_base = 2.296;
float s2_base = 2.327;


/*Calculate hot gas power needed to defrost*/
double calculate_hot_gas_power(double mass_flow_rate, float h_1, float h_2, double efficiency) {
        float enthalpy_change = h_2 - h_1;

        printf("Enthalpy Change: %.2f kJ/kg\n", enthalpy_change);
        float hot_gas_power = mass_flow_rate * enthalpy_change * efficiency;
        return hot_gas_power;
}


/*Function to calculate enthalpy at evaporator outlet (h1)*/
float calculate_enthalpy_evap(float ambient_temp) {
        float adjustment_factor = 0.05; /*adjust for ambient_temp factor*/
        float h1 = h1_base + ambient_temp * adjustment_factor;
    
        printf("Evap Enthalpy: %.2f kJ/kg\n", h1);
        return h1;
}


/*Function to calculate enthalpy at compressor discharge (h2)*/
float calculate_enthalpy_com(float ambient_temp) {
        float adjustment_factor = 0.09; // Example factor; adjust based on system data
        float h2 = h2_base + ambient_temp * adjustment_factor;
        
	printf("Com Enthalpy: %.2f kJ/kg\n", h2);
        return h2;
}


/*Function to calculate enthalpy at condenser outlet (h3)*/
float calculate_enthalpy_con(float ambient_temp) {
        float adjustment_factor = 0.03; /*adjust for ambient_temp factor*/
        float h3 = h3_base + ambient_temp * adjustment_factor;
        
	printf("Con Enthalpy: %.2f kJ/kg\n", h3);
        return h3;
}


/*Function to calculate enthalpy at expansion valve outlet (h4)*/
float calculate_enthalpy_exp_valve(float ambient_temp) {
        float adjustment_factor = 0.02; /*adjust for ambient_temp factor*/
        
	float h4 = h4_base + ambient_temp * adjustment_factor;
        printf("Exp Valve Enthalpy: %.2f kJ/kg\n", h4);
        return h4;
}


/* Function to calculate entropy at evaporator outlet (s1)*/
float calculate_entropy_evap(float ambient_temp) {
        float adjustment_factor = 0.01; /*adjust for ambient_temp factor*/
        float s1 = s1_base + ambient_temp * adjustment_factor;
    
	printf("Evap Entropy: %.2f kJ/kg\n", s1);
        return s1;
}


/*Function to calculate entropy at compressor discharge (s2)*/
float calculate_entropy_com(float ambient_temp) {
        float adjustment_factor = 0.02; /*adjust for ambient_temp factor*/
        float s2 = s2_base + ambient_temp * adjustment_factor;
    
	printf("Com Entropy: %.2f kJ/kg\n", s2);
        return s2;
}


/*Function to calculate Coefficient of Performance (COP)*/
float calculate_cop(float Q_evap, float W_com) {
        if (W_com == 0) {
        return 0.0;
        }

        float cop = Q_evap / W_com;
        return cop;
}

