#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "variables.h"



/* Declare global variables for sensors and components */
TempSen temp_sen;
DoorSen door_sen;
HudSen hud_sen;
Com com;
Con con;
Evap evap;
ExpValve exp_valve;
SolValve sol_valve;
HotGasBypass hot_gas_bypass;


int main() {
    srand(time(NULL));

    float last_defrost_time = 0;
    int defrost_needed = 0;
    float com_energy_consumed = 0.0;
    float defrost_energy_consumed = 0.0;
    float control_unit_energy_consumed = 0.0;
    float total_energy_consumed = 0.0;
    float total_heat_removed = 0.0;
    int door_open_time = 0;
    float mass_flow_rate = 0.0096;
    float Q_evap = 0.0;
    float W_com = 0.0;
    int defrost_count = 0;

    float high_pressure = HIGH_P;
    float low_pressure = LOW_P;
    float con_temp = 0.0; /* Initialize temp in the freezer */
    float evap_temp = 0.0; // Initialize with sensor reading

     // Get ambient conditions
    float ambient_temp = read_ambient_temp();
    float ambient_humidity = read_ambient_hud();


    float mass_of_air = AIR_DENSITY * COLD_ROOM_VOLUME;

    initialize_sensors(&temp_sen, &door_sen, &hud_sen);
    initialize_components(&com, &con, &evap, &exp_valve, &sol_valve, &hot_gas_bypass);

    FILE *energy_file = fopen("energy_consumption.txt", "w");
    FILE *cop_file = fopen("cop_over_time.txt", "w");
    FILE *ph_file = fopen("ph_values.txt", "w");

    if (energy_file == NULL || cop_file == NULL || ph_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(energy_file, "Time(s),CompressorEnergyConsumed(Wh),DefrostEnergyConsumed(Wh),ControlUnitEnergyConsumed(Wh),TotalEnergyConsumed(Wh)\n");
    fprintf(cop_file, "Time(s),COP\n");
    fprintf(ph_file, "Time(s),H1,H2,H3,H4,Q_evap,W_com,TotalHeatRemoved\n");

    // Main simulation loop, runs until the simulation duration is complete
    for (int current_time = 0; current_time <= SIMULATION_DURATION; current_time += CHECK_INTERVAL) {
        // Read sensors and update states
        update_sensors(&temp_sen, &door_sen, &hud_sen, &com, ambient_temp, ambient_humidity);

        // Calculate deltas (commented out since it's not defined in the snippet)
        // calculate_deltas();

        // Adjust temp and humidity based on door state
        adjust_for_door_state(&temp_sen, &hud_sen, &door_sen);

        // Simulate effects of various components
        if (com.isOn) {
            simulate_com_effect(temp_sen.temp, &evap_temp, read_ambient_temp(), &com_energy_consumed);
        }

        simulate_con_effect(temp_sen.temp, &con_temp, read_ambient_temp(), subcooling);
        simulate_evap_effect(temp_sen.temp, &evap_temp, read_ambient_temp(), superheating);

        simulate_sol_valve_effect(&sol_valve);
        simulate_exp_valve_effect(&exp_valve);

        float current_defrost_energy_consumed = 0.0;

        // Simulate defrosting
        if (defrost_needed) {
            simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &current_defrost_energy_consumed);
            defrost_count++;
            defrost_energy_consumed += current_defrost_energy_consumed; // Track energy for each defrost
        }

        // Control system
        control_system(&temp_sen, &door_sen, &hud_sen, &com, &con, &evap, &exp_valve, &sol_valve, &hot_gas_bypass, &total_energy_consumed);

        /* Calculate enthalpies and energy */
        float h_1 = calculate_enthalpy_evap(low_pressure, evap_temp, read_ambient_temp(), superheating);
        float h_2 = calculate_enthalpy_com(high_pressure, con_temp, read_ambient_temp());
        float h_3 = calculate_enthalpy_con(high_pressure, con_temp, subcooling, read_ambient_temp());
        float h_4 = calculate_enthalpy_exp_valve(low_pressure, evap_temp);

        Q_evap = mass_flow_rate * (h_1 - h_4);
        W_com = mass_flow_rate * (h_2 - h_1);

        printf("h_1: %.2f, h_2: %.2f, h_3: %.2f, h_4: %.2f\n", h_1, h_2, h_3, h_4);

        float temp_diff = read_ambient_temp() - temp_sen.temp;
        total_heat_removed += AIR_SPECIFIC_HEAT * mass_of_air * temp_diff / 3600.0;

        float control_unit_energy = CONTROL_UNIT_ENERGY_CONSUMPTION;
        
        total_energy_consumed = com_energy_consumed + defrost_energy_consumed + control_unit_energy;

        float cop = total_energy_consumed == 0.0 ? 0.0 : calculate_cop(Q_evap, W_com);

        printf("COP: %.2f\n", cop);
        fprintf(cop_file, "%d,%.2f\n", current_time, cop);
        fprintf(energy_file, "%d,%.2f,%.2f,%.2f,%.2f\n", current_time, com_energy_consumed, defrost_energy_consumed, control_unit_energy, total_energy_consumed);
        fprintf(ph_file, "%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", current_time, h_1, h_2, h_3, h_4, Q_evap, W_com, total_heat_removed);

        usleep(CHECK_INTERVAL * 1000000); // Simulate delay of 5 seconds (CHECK_INTERVAL in microseconds)
    }

    fclose(energy_file);
    fclose(cop_file);
    fclose(ph_file);

    printf("Simulation completed.\n");
    printf("Final Total Energy Consumed: %.2f kWh\n", total_energy_consumed / 1000.0);
    printf("Number of Defrost Cycles: %d\n", defrost_count);

    return 0;
}

