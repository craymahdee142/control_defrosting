#include "main.h"
#include "variables.h"

// Initialize sensors with starting values
void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen) {
    temp_sen->temp = -18.0f; // Start with -18.0°C
    door_sen->isOpen = 0;     // Assume door is initially closed
    hud_sen->hud = 55.0f;     // Start with 55% humidity
}

// Update sensor values at runtime
void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen) {
    // Simulate reading new temperature from sensor
    temp_sen->temp = read_temp(temp_sen);

    // Simulate reading new door state from sensor
    door_sen->isOpen = read_door_sen(door_sen);  // Assuming you have a function to read door sensor

    // Simulate reading new humidity from sensor
    hud_sen->hud = read_hud(hud_sen);
}


// Get the current time in seconds
float get_current_time() {
    return (float)time(NULL);
}


float read_hud(HudSen* hud_sen) {
    if (hud_sen == NULL) {
        printf("Error: hud_sen is NULL\n");
        return -1.0f;
    }
    return hud_sen->hud;
}


float read_temp(TempSen* temp_sen) {
    if (temp_sen == NULL) {
        // Handle null pointer error
        printf("Error: temp_sen is NULL\n");
        return -1.0f; // Return an error code
    }
    return temp_sen->temp;
}


// Function to control temperature within target range
void control_temperature(TempSen* temp_sen, SolValve* sol_valve, Com* com, Con* con) {
    if (temp_sen->temp < -18.0) {
        // Temperature is below the target range, take action to increase temperature
        printf("Temperature is too low. Increasing temperature.\n");
        increase_temperature(sol_valve, com, con);
    } else if (temp_sen->temp > -16.0) {
        // Temperature is above the target range, take action to decrease temperature
        printf("Temperature is too high. Decreasing temperature.\n");
        decrease_temperature(sol_valve, com, con);
    } else {
        // Temperature is within the target range, maintain current state
        printf("Temperature is within the target range. Maintaining current state.\n");
        // Optionally, turn off unnecessary components to save energy
        sol_valve->isOn = 0;
        com->isOn = 0;
        con->isOn = 0;
    }
}

void control_humidity(HudSen* hud_sen, SolValve* sol_valve, Evap* evap, Com* com) {
    if (hud_sen->hud < 50.0) {
        // Humidity is below the target range, take action to increase humidity
        printf("Humidity is too low. Increasing humidity.\n");
        increase_humidity(sol_valve, com);
    } else if (hud_sen->hud > 70.0) {
        // Humidity is above the target range, take action to decrease humidity
        printf("Humidity is too high. Decreasing humidity.\n");
        decrease_humidity(sol_valve, evap, com);
    } else {
        // Humidity is within the target range, maintain current state
        printf("Humidity is within the target range. Maintaining current state.\n");
        // Optionally, you can turn off unnecessary components to save energy
        sol_valve->isOn = 0;
        evap->isOn = 0;
        com->isOn = 0;
    }
}

// Improved main control system function with updated range checks
void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
    // Declare and initialize variables
    float high_pressure = HIGH_P;
    float evap_temp = -25.0; 
    float con_temp = 40.0;
    float defrost_energy_consumed = 0.0;
    static float last_defrost_time = 0;
    int defrost_needed = 0;
    int defrost_count = 0;
    float minimum_defrost_interval = 10; 
    float ambient_temp = read_ambient_temp();

    // Initialize sensors
    initialize_sensors(temp_sen, door_sen, hud_sen);

    // Update sensor readings at the start of each control cycle
    update_sensors(temp_sen, door_sen, hud_sen);


    // Read current sensor values with realistic range checks
    float current_temp = read_temp(temp_sen);
    float current_hud = read_hud(hud_sen);

    // Ensure the sensor readings are within expected ranges
    if (current_temp < -30.0 || current_temp > -10.0) { // Example range check for chest freezer
        fprintf(stderr, "Error: Temperature reading out of range: %.2f°C\n", current_temp);
        return;
    }
    if (current_hud < 0.0 || current_hud > 100.0) { // Humidity should be between 0% and 100%
        fprintf(stderr, "Error: Humidity reading out of range: %.2f%%\n", current_hud);
        return;
    }

    // Define target ranges for temperature and humidity
    float target_temp_low = -18.0;
    float target_temp_high = -16.0;
    float target_humidity_low = 50.0;
    float target_humidity_high = 70.0;

    // Control superheating and subcooling
    if (superheating < MIN_SUPERHEATING) {
        exp_valve->isOpen = 1;
    } else if (superheating > MAX_SUPERHEATING) {
        exp_valve->isOpen = 0;
    }

    if (subcooling < MIN_SUBCOOLING) {
        con->isOn = 1;
    } else if (subcooling > MAX_SUBCOOLING) {
        con->isOn = 0;
    }

    // Check frost level
    float frost_level = get_frost_level(current_temp, current_hud, target_temp_low, target_temp_high, target_humidity_low, target_humidity_high);

    if (frost_level < 20.0) {
        // Control temperature
        control_temperature(temp_sen, sol_valve, com, con);

        // Control humidity
        control_humidity(hud_sen, sol_valve, evap, com);
    } else {
        // General action if both temperature and humidity are acceptable
        printf("Adjusting temperature and humidity to manage frost level.\n");
        com->isOn = 0;
        con->isOn = 0;
        evap->isOn = 0;
    }

    // Check if defrosting is needed
    if ((current_temp <= -18.0 || current_hud > target_humidity_high) &&
        (get_current_time() - last_defrost_time >= minimum_defrost_interval)) {
        printf("Defrost conditions met. Temperature: %.2f°C, Humidity: %.2f%%\n", current_temp, current_hud);
        defrost_needed = 1;
        last_defrost_time = get_current_time(); // Update last defrost time
    }

    // Perform defrosting if needed
    if (defrost_needed) {
        simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed; // Accumulate total energy consumed
        defrost_needed = 0;
        defrost_count++;
    }

    // Simulate effects of various components
    if (com->isOn) {
        simulate_com_effect(temp_sen->temp, &con_temp, ambient_temp, energy_consumed);
    }
    if (evap->isOn) {
        simulate_evap_effect(temp_sen->temp, &evap_temp, ambient_temp, superheating);
    }
    if (con->isOn) {  
        simulate_con_effect(temp_sen->temp, &con_temp, ambient_temp, subcooling);
    }
    if (exp_valve->isOpen) {
        simulate_exp_valve_effect(exp_valve);
    }
    if (sol_valve->isOn) {
        simulate_sol_valve_effect(sol_valve);
    }
    if (hot_gas_bypass->isActive) {
        simulate_hot_gas_bypass_defrosting(high_pressure, &con_temp, &defrost_energy_consumed);
        *energy_consumed += defrost_energy_consumed; // Accumulate total energy consumed
    }
}

