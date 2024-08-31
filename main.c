#include "main.h"
#include "variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*Declare global variables for sensors and components*/
TempSen temp_sen;
DoorSen door_sen;
HudSen hud_sen;
Com com;
Con con;
Evap evap;
ExpValve exp_valve;
SolValve sol_valve;
HotGasBypass hot_gas_bypass;

/**
 * main - main entry
 * Return: the output results
 */

int main() {
        srand(time(NULL));

        float last_defrost_time = 0;
        int defrost_needed = 0;
        float com_energy_consumed = 0.0;
        float defrost_energy_consumed = 0.0;
        float total_energy_consumed = 0.0;
        float total_heat_removed = 0.0;
        float mass_flow_rate = 0.000915; /* in kg/s*/

        int defrost_count = 0;

        float ambient_temp = read_ambient_temp();
        float ambient_humidity = read_ambient_hud();

        float mass_of_air = AIR_DENSITY * COLD_ROOM_VOLUME;

        initialize_sensors(&temp_sen, &door_sen, &hud_sen);
        initialize_components(&com, &con, &evap, &exp_valve, &sol_valve, &hot_gas_bypass);

        FILE *energy_file = fopen("energy_consumption.txt", "w");
        FILE *cop_file = fopen("cop_over_time.txt", "w");
        FILE *temp_humidity_file = fopen("temp_humidity_profiles.txt", "w");
        FILE *heat_absorbed_vs_removed_file = fopen("heat_absorbed_vs_removed.txt", "w");
        FILE *defrost_file = fopen("defrost_conditions.txt", "w");

        if (energy_file == NULL || cop_file == NULL || temp_humidity_file == NULL || heat_absorbed_vs_removed_file == NULL || defrost_file == NULL) {
                printf("Error opening file.\n");
                return 1;
        }

        fprintf(energy_file, "Time(s),CompressorEnergyConsumed(KWh),DefrostEnergyConsumed(KWh),TotalEnergyConsumed(KWh)\n");
        fprintf(cop_file, "Time(s),COP\n");
        fprintf(temp_humidity_file, "Time(s),Temperature(C),Humidity(%%)\n");
        fprintf(heat_absorbed_vs_removed_file, "Time(s),HeatAbsorbed(kJ),HeatRemoved(kJ)\n");
        fprintf(defrost_file, "Time(s),DefrostEnergyConsumed(KWh),TemperatureIncrease(C),HumidityDecrease(%%)\n");

        /* Main simulation loop */
        for (int current_time = 0; current_time <= SIMULATION_DURATION; current_time += CHECK_INTERVAL) {
        	/* Update sensor values */
        	update_sensors(&temp_sen, &door_sen, &hud_sen, &com, ambient_temp, ambient_humidity);
        	adjust_for_door_state(&temp_sen, &hud_sen, &door_sen);

		/* Conditions for defrosting */
        	if (temp_sen.temp < TARGET_TEMP_LOW || hud_sen.hud > TARGET_HUMIDITY_HIGH) {
        	        float  defrost_energy_consumed = 0.0;
                	simulate_hot_gas_bypass_defrosting(&temp_sen.temp, &defrost_energy_consumed, ambient_temp);
                	printf("Defrosting cycle initiated. Energy consumed: %.2f Wh\n", defrost_energy_consumed);

           	        total_energy_consumed += defrost_energy_consumed;
            	        defrost_count++;

               		float temp_increase = temp_sen.temp - (read_ambient_temp() - TEMP_RISE_DURING_DEFROST);
                	float humidity_decrease = hud_sen.hud - (read_ambient_hud() - HUMIDITY_DROP_DURING_DEFROST);
                	printf("Defrost Energy Consumed before writing: %.2f KWh\n", defrost_energy_consumed);
			fprintf(energy_file, "%d,%.2f,%.2f,%.2f\n", current_time, com_energy_consumed, total_energy_consumed, total_energy_consumed);
        	}

        	/*Simulate system effects*/
        	simulate_com_effect(temp_sen.temp, ambient_temp, &com_energy_consumed);
        	total_energy_consumed += com_energy_consumed;
        	simulate_con_effect(ambient_temp, &total_heat_removed);
        	simulate_evap_effect(ambient_temp, &total_heat_removed);
        	simulate_sol_valve_effect(&sol_valve);
        	simulate_exp_valve_effect(ambient_temp, &temp_sen.temp, &com_energy_consumed);

        	/*Control system function to control the components*/
        	control_system(&temp_sen, &door_sen, &hud_sen, &com, &con, &evap, &exp_valve, &sol_valve, &hot_gas_bypass, &total_energy_consumed);

        	/*Calculate enthalpy */
        	float h_1 = calculate_enthalpy_evap(ambient_temp);
        	float h_2 = calculate_enthalpy_com(ambient_temp);
        	float h_3 = calculate_enthalpy_con(ambient_temp);
        	float h_4 = calculate_enthalpy_exp_valve(ambient_temp);

        	float s_1 = calculate_entropy_evap(ambient_temp);
        	float s_2 = calculate_entropy_com(ambient_temp);

        	/*Calculate evaporator heat and compressor work*/
        	float Q_evap = mass_flow_rate * (h_1 - h_4);
        	printf("Evaporator [Q_vap]: %.2f KW\n", Q_evap);

        	float W_com = mass_flow_rate * (h_2 - h_1);
        	printf("Compressor [W_com]: %.2f KW\n", W_com);

        	/*Update heat removal*/
        	float temp_diff = ambient_temp - temp_sen.temp;
        	total_heat_removed += AIR_SPECIFIC_HEAT * mass_of_air * temp_diff / 3600.0;

        	/*Calculate COP and update total energy*/
        	float cop = (W_com == 0.0) ? 0.0 : Q_evap / W_com;

        	/* Log data to files*/
        	fprintf(temp_humidity_file, "%d,%.2f,%.2f\n", current_time, temp_sen.temp, hud_sen.hud);
        	fprintf(heat_absorbed_vs_removed_file, "%d,%.2f,%.2f\n", current_time, Q_evap * mass_flow_rate * 3600.0, total_heat_removed);
        	fprintf(cop_file, "%d,%.2f\n", current_time, cop);
        	fprintf(energy_file, "%d,%.2f,%.2f,%.2f\n", current_time, com_energy_consumed, defrost_energy_consumed, total_energy_consumed);

        	/*Flush data to files*/
        	fflush(energy_file);
        	fflush(cop_file);
        	fflush(temp_humidity_file);
        	fflush(heat_absorbed_vs_removed_file);
        	fflush(defrost_file);

        	/*usleep(CHECK_INTERVAL * 1000000);*/ 
        }

        /*Close file handles*/
        fclose(energy_file);
        fclose(cop_file);
        fclose(temp_humidity_file);
        fclose(heat_absorbed_vs_removed_file);
        fclose(defrost_file);

        printf("Simulation completed.\n");
        printf("Final Total Energy Consumed: %.2f KWh\n", total_energy_consumed);
        printf("Number of Defrost Cycles: %d\n", defrost_count);

        return 0;
}


