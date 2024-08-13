#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "variables.h"

/* Declare global variables for deltas */
float delta_T = 0.0;
float delta_W = 0.0;

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

/* Declare constants and variables */
float U = 0.28; // Example value
float A = 2.65235; // Example value
float mass = 20.0; // Example value
float specific_heat_air = 0.5; // Example value
float usage_factor = 0.8; // Example value
float CFM = 0.085; // Example value

/* Function prototypes */
void calculate_deltas(void);

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
    float mass_flow_rate = 0.79;
    float Q_evap = 0.0;
    float W_com = 0.0;
    int defrost_count = 0;

    float high_pressure = HIGH_P;
    float low_pressure = LOW_P;
    float con_temp; /* Initial temp in the freezer */
    float evap_temp; // Initialize with sensor reading

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
    fprintf(ph_file, "Time(s),P1,P2,P3,P4,H1,H2,H3,H4\n");

    for (int i = 0; i < SIMULATION_DURATION / CHECK_INTERVAL; i++) {
        // Read sensors and update states
        temp_sen.temp = read_temp(&temp_sen);
        hud_sen.hud = read_hud(&hud_sen);
        float ambient_temp = read_ambient_temp();
        float ambient_hud = read_ambient_hud();

        // Toggle door state randomly for testing
        toggle_door_state(&door_sen);

        // Adjust temp based on door state
        float temp_adjustment = 0.0;
        adjust_for_door_state(&door_sen, &temp_adjustment, &door_open_time);
        temp_sen.temp += temp_adjustment;

        // Calculate deltas
        calculate_deltas();

        // Calculate heat gains
        float heat_gain_walls_roof = calculate_heat_gain_walls_roof(U, A, delta_T);
        float heat_gain_product = calculate_heat_gain_product(mass, specific_heat_air, delta_T, usage_factor);
        float heat_gain_infiltration = calculate_heat_gain_infiltration(CFM, delta_T);
        float latent_heat_gain = calculate_latent_heat_gain(CFM, delta_W);
        float heat_gain_air = calculate_heat_gain_air(mass_of_air, specific_heat_air, delta_T);

        /* Q_total heat */
        float total_heat_gain = heat_gain_walls_roof + heat_gain_product + heat_gain_infiltration + latent_heat_gain + heat_gain_air;
        printf("Total Heat Gain: %.2f W\n", total_heat_gain);

        float current_com_energy_consumed = 0.0;
        // Simulate effects of various components
        if (com.isOn) {
            simulate_com_effect(temp_sen.temp, &evap_temp, ambient_temp, &com_energy_consumed);
        }
        com_energy_consumed += current_com_energy_consumed; /* Accumulates compressor energy */

        simulate_con_effect(temp_sen.temp, &ambient_temp, con_temp, subcooling);
        simulate_evap_effect(temp_sen.temp, &ambient_temp, evap_temp, superheating);

        simulate_sol_valve_effect(&sol_valve);
        simulate_exp_valve_effect(&exp_valve);

        float current_defrost_energy_consumed = 0.0;

        // Simulate defrosting
        if (defrost_needed) {
            simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &current_defrost_energy_consumed);
            defrost_count++;
        }
        defrost_energy_consumed += current_defrost_energy_consumed;

        // Control system
        control_system(&temp_sen, &door_sen, &hud_sen, &com, &con, &evap, &exp_valve, &sol_valve, &hot_gas_bypass, &com_energy_consumed);

        /* Calculate enthalpies and energy */
        float h_1 = calculate_enthalpy_evap(low_pressure, evap_temp, ambient_temp, superheating);
        float h_2 = calculate_enthalpy_com(high_pressure, con_temp, ambient_temp);
        float h_3 = calculate_enthalpy_con(high_pressure, con_temp, subcooling, ambient_temp);
        float h_4 = calculate_enthalpy_exp_valve(low_pressure, evap_temp);

        Q_evap = mass_flow_rate * (h_1 - h_4);
        W_com = mass_flow_rate * (h_2 - h_1);

        printf("h_1: %.2f, h_2: %.2f, h_3: %.2f, h_4: %.2f\n", h_1, h_2, h_3, h_4);

        float temp_diff = ambient_temp - temp_sen.temp;
        total_heat_removed += AIR_SPECIFIC_HEAT * mass_of_air * temp_diff / 3600.0;

        float control_unit_energy = CONTROL_UNIT_ENERGY_CONSUMPTION;
        total_energy_consumed = com_energy_consumed + defrost_energy_consumed + control_unit_energy;

        float sensors_energy_consumed = SENSOR_ENERGY_CONSUMPTION * NUM_SENSORS;

        // Before calculating COP
        printf("Before COP Calculation - Total Heat Gain: %.2f, W_com: %.2f\n", total_heat_gain, W_com);

        float cop = total_energy_consumed == 0.0 ? 0.0 : calculate_cop(total_heat_gain, W_com);

        // Print current state
        printf("Time: %d seconds\n", i * CHECK_INTERVAL);
        printf("Temperature: %.2f°C\n", temp_sen.temp);
        printf("Humidity: %.2f%%\n", hud_sen.hud);
        printf("Door state: %s\n", door_sen.isOpen ? "Open" : "Closed");
        printf("Compressor Energy Consumed: %.2f kWh\n", com_energy_consumed / 1000.0);
        printf("Defrost Energy Consumed: %.2f kWh\n", defrost_energy_consumed / 1000.0);
        printf("Control Unit Energy Consumed: %.2f kWh\n", control_unit_energy / 1000.0);
        printf("Sensors Energy Consumed: %.2f kWh\n", sensors_energy_consumed / 1000.0);
        printf("Evaporator Absorbed Energy: %.2f kWh\n", Q_evap / 1000.0);
        printf("Total Energy Consumed: %.2f kWh\n", total_energy_consumed / 1000.0);
        printf("Total Heat Removed: %.2f kWh\n", total_heat_removed / 1000.0);
        printf("COP: %.2f\n", cop);
        printf("\n");

        fprintf(energy_file, "%d,%.2f,%.2f,%.2f,%.2f\n", i * CHECK_INTERVAL, com_energy_consumed, defrost_energy_consumed, control_unit_energy, total_energy_consumed);
        fprintf(cop_file, "%d,%.2f\n", i * CHECK_INTERVAL, cop);
        fprintf(ph_file, "%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", i * CHECK_INTERVAL, LOW_P, HIGH_P, HIGH_P, LOW_P, h_1, h_2, h_3, h_4);

        usleep(CHECK_INTERVAL * 1000000);
    }

    fclose(energy_file);
    fclose(cop_file);
    fclose(ph_file);

    printf("Simulation completed.\n");
    printf("Final Total Energy Consumed: %.2f kWh\n", total_energy_consumed / 1000.0);
    printf("Number of Defrost Cycles: %d\n", defrost_count);

    return 0;
}
