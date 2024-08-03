#include "main.h"


int main() {
    srand(time(NULL));

    TemperatureSensor temp_sensor;
    DoorSensor door_sensor;
    HumiditySensor humidity_sensor;
    Compressor compressor;
    Condenser condenser;
    Evaporator evaporator;
    ExpansionValve expansion_valve;
    SolenoidValve solenoid_valve;
    HotGasBypass hot_gas_bypass;

    float last_defrost_time = 0;
    int defrost_needed = 0;
    float compressor_energy_consumed = 0.0;
    float defrost_energy_consumed = 0.0;
    float control_unit_energy_consumed = 0.0;
    float total_energy_consumed = 0.0;
    float total_heat_removed = 0.0;
    int door_open_time = 0;
    float mass_flow_rate = 3.0;
    float Q_evap = 0.0;
    int defrost_count = 0;

    float mass_of_air = AIR_DENSITY * COLD_ROOM_VOLUME;

    initialize_sensors(&temp_sensor, &door_sensor, &humidity_sensor);
    initialize_components(&compressor, &condenser, &evaporator, &expansion_valve, &solenoid_valve, &hot_gas_bypass);

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
        temp_sensor.temperature = read_temperature();
        humidity_sensor.humidity = read_humidity(&humidity_sensor);
        float ambient_temperature = read_ambient_temperature();
        float surrounding_temperature = read_surrounding_temperature();

        // Toggle door state randomly for testing
        toggle_door_state(&door_sensor);

        // Adjust temperature based on door state
        float temperature_adjustment = 0.0;
        adjust_for_door_state(&door_sensor, &temperature_adjustment, &door_open_time);
        temp_sensor.temperature += temperature_adjustment;

        // Simulate effects of various components
        if (compressor.isOn) {
            simulate_compressor_effect(&temp_sensor.temperature, ambient_temperature, &compressor_energy_consumed);
        }
        simulate_condenser_effect(&temp_sensor.temperature, surrounding_temperature);
        simulate_evaporator_effect(&temp_sensor.temperature, surrounding_temperature);
        simulate_solenoid_valve_effect(&solenoid_valve);
        simulate_expansion_valve_effect(&expansion_valve);
        simulate_hot_gas_bypass_effect(&hot_gas_bypass, &temp_sensor);

        // Control system
        control_system(&temp_sensor, &door_sensor, &humidity_sensor, &compressor, &condenser, &evaporator, &expansion_valve, &solenoid_valve, &hot_gas_bypass, &compressor_energy_consumed);

        // Calculate enthalpies and energy
        float h_1 = calculate_enthalpy_evaporator(LOW_P, ambient_temperature);
        float h_2 = calculate_enthalpy_compressor_outlet(HIGH_P, ambient_temperature);
        float h_3 = calculate_enthalpy_condenser(HIGH_P, surrounding_temperature);
        float h_4 = calculate_enthalpy_expansion_valve(LOW_P, surrounding_temperature);
        Q_evap = mass_flow_rate * (h_1 - h_4);

        printf("h_1: %.2f, h_2: %.2f, h_3: %.2f, h_4: %.2f\n", h_1, h_2, h_3, h_4);

        float temp_diff = read_ambient_temperature() - temp_sensor.temperature;
        total_heat_removed += AIR_SPECIFIC_HEAT * mass_of_air * temp_diff / 3600.0;

        float control_unit_energy = CONTROL_UNIT_ENERGY_CONSUMPTION;
        total_energy_consumed = compressor_energy_consumed + defrost_energy_consumed + control_unit_energy;

        float sensors_energy_consumed = SENSOR_ENERGY_CONSUMPTION * NUM_SENSORS;

        if ((temp_sensor.temperature <= 0 || humidity_sensor.humidity > 50) && !defrost_needed && (i * CHECK_INTERVAL - last_defrost_time >= MINIMUM_DEFROST_INTERVAL)) {
            defrost_needed = 1;
            last_defrost_time = i * CHECK_INTERVAL;
        }

        if (defrost_needed) {
            simulate_heater_effect(&temp_sensor, &defrost_energy_consumed);
            defrost_needed = 0;
            defrost_count++;
        }

        float cop = total_energy_consumed == 0.0 ? 0.0 : calculate_cop(h_1, h_2, h_3, h_4, mass_flow_rate);

        // Print current state
        printf("Time: %d seconds\n", i * CHECK_INTERVAL);
        printf("Temperature: %.2f°C\n", temp_sensor.temperature);
        printf("Humidity: %.2f%%\n", humidity_sensor.humidity);
        printf("Door state: %s\n", door_sensor.isOpen ? "Open" : "Closed");
        printf("Compressor Energy Consumed: %.2f kWh\n", compressor_energy_consumed / 1000.0);
        printf("Defrost Energy Consumed: %.2f kWh\n", defrost_energy_consumed / 1000.0);
        printf("Control Unit Energy Consumed: %.2f kWh\n", control_unit_energy / 1000.0);
        printf("Sensors Energy Consumed: %.2f kWh\n", sensors_energy_consumed / 1000.0);
        printf("Evaporator Absorbed Energy: %.2f kWh\n", Q_evap / 1000.0);
        printf("Total Energy Consumed: %.2f kWh\n", total_energy_consumed / 1000.0);
        printf("Total Heat Removed: %.2f kWh\n", total_heat_removed / 1000.0);
        printf("COP: %.2f\n", cop);
        printf("\n");

        fprintf(energy_file, "%d,%.2f,%.2f,%.2f,%.2f\n", i * CHECK_INTERVAL, compressor_energy_consumed, defrost_energy_consumed, control_unit_energy, total_energy_consumed);
        fprintf(cop_file, "%d,%.2f\n", i * CHECK_INTERVAL, cop);
        fprintf(ph_file, "%d,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f\n", i * CHECK_INTERVAL, LOW_P, HIGH_P, HIGH_P, LOW_P, h_1, h_2, h_3, h_4);

        usleep(CHECK_INTERVAL * 1000000);
    }

    fclose(energy_file);
    fclose(cop_file);
    fclose(ph_file);

    printf("Simulation completed.\n");

    printf("Final Total Energy Consumed: %.2f kWh\n", total_energy_consumed / 1000.0);
    printf("Number of Defrost Cycles: %d\n", defrost_count); // Print the defrost cycle count

    return 0;
}
