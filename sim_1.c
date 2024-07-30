#include "main.h"

void toggle_door_state(DoorSensor* door_sensor) {
    door_sensor->isOpen = rand() % 2; // Randomly set door state to open or closed
}

/*
void simulate_heater_effect(TemperatureSensor* temp_sensor, float* defrost_energy_consumed) {
    double initial_temp = temp_sensor->temperature;

    Define the target temperature to reach during defrosting
    float target_temp = 5.0; 
    double energy_consumed = 0.0;
    double heater_power = HEATER_POWER / heater_efficiency;

    Continue defrosting until the target temperature is reached
    while (temp_sensor->temperature < target_temp) {
        
        double defrost_step_duration = 1.0; 

        Simulate the temperature increase
        float temp_increase = 1.0; 
        temp_sensor->temperature += temp_increase;

        Calculate the energy consumed for this step
        energy_consumed += heater_power * defrost_step_duration / 3600.0; // Convert to Wh

         Break if temperature reaches or exceeds the target temperature
        if (temp_sensor->temperature >= target_temp) {
            break;
        }
    }

    pdate the total defrost energy consumed
    *defrost_energy_consumed += energy_consumed;

    Print the defrost details for debugging
    printf("Defrost completed. Energy consumed: %.2f Wh, Final temperature: %.2f°C\n", energy_consumed, temp_sensor->temperature);
}

*/


void simulate_hot_gas_defrosting(float* temperature, float* defrost_energy_consumed) {
    float mass_flow_rate = 3.0; // kg/s
    float surrounding_temperature = read_surrounding_temperature();
    
    // Calculate enthalpies and energy
    float h_1 = calculate_enthalpy_evaporator(PRESSURE_LOW, ambient_temperature);
    float h_2 = calculate_enthalpy_compressor_outlet(PRESSURE_HIGH, ambient_temperature);

    float hot_gas_power = calculate_hot_gas_power(mass_flow_rate, h_2, h_1, HOT_GAS_EFFICIENCY); // kW

    float target_temp = 5.0; // Target temperature during defrosting
    float initial_temp = *temperature;
    float energy_consumed = 0.0;

    while (*temperature < target_temp) {
        float defrost_step_duration = 1.0; // Duration of defrost step in seconds
        float temp_increase = 1.0; // Temperature increase per step in °C
        *temperature += temp_increase;
        energy_consumed += hot_gas_power * defrost_step_duration / 3600.0; // Convert to Wh

        if (*temperature >= target_temp) {
            break;
        }
    }

    *defrost_energy_consumed += energy_consumed;
    printf("Defrost completed using hot gas. Energy consumed: %.2f Wh, Final temperature: %.2f°C\n", energy_consumed, *temperature);
}




void simulate_fan_effect(HumiditySensor* humidity_sensor) {
    float humidity_decrease = random_float(1, 5);
    humidity_sensor->humidity -= humidity_decrease;
    if (humidity_sensor->humidity < 0) {
        humidity_sensor->humidity = 0;
    }
}



int read_humidity(HumiditySensor* humidity_sensor) {
    humidity_sensor->humidity = random_float(30, 50);
    return humidity_sensor->humidity;
}

float read_temperature() {
    return random_float(-5.0, 25.0);
}

void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment, int* door_open_time) {
    int max_sec = 8; // Define the maximum seconds the door is allowed to be open without extra temperature adjustment

    if (door_sensor->isOpen) {
        (*door_open_time)++; // Increment the door open time counter

        // Basic temperature adjustment when the door is open
        *temperature_adjustment += 2.0;

        // Additional temperature adjustment based on how long the door has been open
        if (*door_open_time > max_sec) {
            *temperature_adjustment += (*door_open_time - max_sec) * 0.5; // Increase adjustment for every second beyond max_sec
        }

        printf("Door state: OPEN for %d seconds\n", *door_open_time);
    } else {
        *door_open_time = 0; // Reset the door open time counter if the door is closed
        printf("Door state: CLOSED\n");
    }
}


/*
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor) {
    if (hot_gas_bypass->isActive) {
        float temp_increase = 1.0;
        temp_sensor->temperature += temp_increase;
    }
}
*/

float read_ambient_temperature() {
    return random_float(15.0, 35.0);
}

float read_surrounding_temperature() {
    return random_float(10.0, 30.0);
}

