#include "main.h"
#include "variables.h"

/*Generate a random float between min and max*/
float random_float(float min, float max) {
        return min + ((float)rand() / RAND_MAX) * (max - min);
}


/**
 * simulate_com_effect - function to simulates compreessor effect
 * @*ambient_temp: generates random temperature
 * @energy_consumed: the energy consumed by the compressor
 */
void simulate_com_effect(float temp, float ambient_temp, float* energy_consumed) {
        float mass_flow_rate = 0.000915; 

        /*Calculate enthalpy values*/
        float h_1 = calculate_enthalpy_evap(ambient_temp);
        float h_2 = calculate_enthalpy_com(ambient_temp);

        /*Calculate compressor work in kW*/
        float W_com = mass_flow_rate * (h_2 - h_1);

        /*Convert the energy consumed from kW to kWh based on CHECK_INTERVAL*/
        float energy_consumed_current_interval = W_com * CHECK_INTERVAL / 3600.0;
    
        // Accumulate energy consumed in kWh
        *energy_consumed += energy_consumed_current_interval;

        printf("Compressor [W_com]: %.2f kW\n", W_com);
        printf("CHECK_INTERVAL: %d seconds\n", CHECK_INTERVAL);
        printf("Temp Sensor Reading: %.2f C\n", temp);
        printf("Ambient Temperature: %.2f C\n", ambient_temp);
        printf("Energy Consumed (Current Interval): %.5f kWh\n", energy_consumed_current_interval);
        printf("Total Energy Consumed: %.5f kWh\n", *energy_consumed);
}



/**
 * simulate_con_effect - function to simulates condenser effect
 * @*ambient_temp: generates random temperature
 * @energy_rejected: the energy rejected by the condenser
 */
void simulate_con_effect(float ambient_temp, float* energy_rejected) {
        float mass_flow_rate = 0.000915;
        float h2 = calculate_enthalpy_com(ambient_temp); 
        float h3 = calculate_enthalpy_con(ambient_temp);

        float Q_cond = mass_flow_rate * (h2 - h3);
        *energy_rejected = Q_cond / 1000.0;

        printf("Condenser Simulation:\n");
        printf("Ambient Temperature: %.2f\n", ambient_temp);
        printf("Enthalpy at Compressor Discharge (h2): %.2f kJ/kg\n", h2);
        printf("Enthalpy at Condenser Outlet (h3): %.2f kJ/kg\n", h3);
        printf("Energy Rejected by Condenser: %.2f kW\n", *energy_rejected);
}

/**
 * simulate_evap_effect - function to simulates evaporator effect
 * @*ambient_temp: generates random temperature
 * @energy_absorbed: the energy absorbed by the evaporator
 */
void simulate_evap_effect(float ambient_temp, float* energy_absorbed) {
        float mass_flow_rate = 0.000915;
        float h4 = calculate_enthalpy_exp_valve(ambient_temp);
        float h1 = calculate_enthalpy_evap(ambient_temp);

        float Q_evap = mass_flow_rate * (h1 - h4);
        *energy_absorbed = Q_evap / 1000.0;

        printf("Evaporator Simulation:\n");
        printf("Ambient Temperature: %.2f\n", ambient_temp);
        printf("Enthalpy at Expansion Valve Outlet (h4): %.2f kJ/kg\n", h4);
        printf("Enthalpy at Evaporator Outlet (h1): %.2f kJ/kg\n", h1);
        printf("Energy Absorbed by Evaporator: %.2f kW\n", *energy_absorbed);
}

/**
 * simulate_exp_valve_effect - function to simulates expansion valve effect
 * @*ambient_temp: generates random temperature
 * @energy_consumed: the energy consumed by the exp valve
 */
void simulate_exp_valve_effect(float ambient_temp, float* temp, float* energy_consumed) {
        float mass_flow_rate = 0.000915;
        float h_1 = calculate_enthalpy_evap(ambient_temp);
        float h_4 = calculate_enthalpy_exp_valve(ambient_temp);

        float enthalpy_change = h_1 - h_4;
        float specific_heat_capacity = 366.5;
        float temp_drop = enthalpy_change / specific_heat_capacity; 

        *temp -= temp_drop;
        *energy_consumed += (enthalpy_change * mass_flow_rate) * CHECK_INTERVAL / 3600.0; 
}


/**
 * simulate_hot_gas_bypass_defrosting - function to simulates defrosting effect
 * @*ambient_temp: generates random temperature
 * @defrost_energy_consumed: the defrost energy consumed during defrosting
 */
void simulate_hot_gas_bypass_defrosting(float* temp, float* defrost_energy_consumed, float ambient_temp) {
        float mass_flow_rate = 0.000915;
        float h_1 = calculate_enthalpy_evap(ambient_temp);
        float h_2 = calculate_enthalpy_com(ambient_temp);
        float hot_gas_power = mass_flow_rate * (h_2 - h_1) * HOT_GAS_EFFICIENCY;
        float ice_mass = ICE_MASS;
    
        float target_temp = -5.0;
        float defrost_step_duration = 1.0;
        float heat_loss_rate = 0.01; 

        float delta_t = target_temp - *temp;
        float q1 = ice_mass * SPECIFIC_HEAT_OF_ICE * delta_t; 
        float q2 = ice_mass * LATENT_HEAT_OF_ICE;               
        float total_energy_required = q1 + q2;

        float time_step_energy = hot_gas_power * defrost_step_duration;
        float heat_loss = time_step_energy * heat_loss_rate;
        float net_energy_available = time_step_energy - heat_loss;

        if (net_energy_available >= total_energy_required) {
        	*temp = target_temp;
        	*defrost_energy_consumed += total_energy_required;
        } else {
        	*temp += (net_energy_available / (ice_mass * SPECIFIC_HEAT_OF_ICE));
        	*defrost_energy_consumed += net_energy_available;
        }
}


/*function to simulates solenoid valve effect*/
void simulate_sol_valve_effect(SolValve* sol_valve) {
        if (sol_valve->isOn) {
        }
}

