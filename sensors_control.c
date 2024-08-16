#include "main.h"
#include "variables.h"

void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen) {
    door_sen->isOpen = 0;
    temp_sen->temp = -17.0; // Initial temperature
    hud_sen->hud = 55.0;  // Initial humidity
    printf("Initial Sensor Temp: %.2f°C\n", temp_sen->temp);
    printf("Initial Sensor Humidity: %.2f%%\n", hud_sen->hud);
}

/*
void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, float ambient_temp, float ambient_humidity) {
    temp_sen->temp = read_temp(temp_sen, com, door_sen, 0, ambient_temp); // Assuming defrost_state is 0 for now
    door_sen->isOpen = read_door_sen(door_sen);
    hud_sen->hud = read_hud(hud_sen, door_sen, ambient_humidity);
    printf("Updated Sensor Temp: %.2f°C\n", temp_sen->temp);
    printf("Updated Sensor Humidity: %.2f%%\n", hud_sen->hud);
}*/


void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, float ambient_temp, float ambient_humidity) {
    temp_sen->temp = read_temp(temp_sen, com, door_sen, 0, ambient_temp); // Ensure correct parameters
    door_sen->isOpen = read_door_sen(door_sen);
    hud_sen->hud = read_hud(hud_sen, door_sen, ambient_humidity);
    printf("Updated Sensor Temp: %.2f°C\n", temp_sen->temp);
    printf("Updated Sensor Humidity: %.2f%%\n", hud_sen->hud);
}


// Get the current time in seconds
float get_current_time() {
    return (float)time(NULL);
}

float read_hud(HudSen* hud_sen, DoorSen* door_sen, float ambient_humidity) {
    if (hud_sen == NULL) {
        printf("Error: hud_sen is NULL\n");
        return -999.0f;
    }
    if (door_sen->isOpen) {
        hud_sen->hud += DOOR_HUMIDITY_GAIN;
    }
    hud_sen->hud += (ambient_humidity - hud_sen->hud) * HUMIDITY_TRANSFER_COEFF;
    printf("Reading Humidity: %.2f%%\n", hud_sen->hud);
    return hud_sen->hud;
}

// Update temperature based on compressor, door, and defrost cycles
float read_temp(TempSen* temp_sen, Com* com, DoorSen* door_sen, int defrost_state, float ambient_temp) {
    if (com->isOn) {
        temp_sen->temp -= COOLING_RATE;
    }
    if (door_sen->isOpen) {
        temp_sen->temp += DOOR_HEAT_GAIN;
    }
    if (defrost_state) {
        temp_sen->temp += DEFROST_HEAT_GAIN;
    }
    temp_sen->temp += (ambient_temp - temp_sen->temp) * HEAT_TRANSFER_COEFF;
    printf("Reading Temperature: %.2f°C\n", temp_sen->temp);
    return temp_sen->temp;
}

bool read_door_sen(DoorSen* door_sen) {
    if (door_sen == NULL) {
        printf("Error: door_sen is NULL\n");
        return false;
    }

    // Randomly simulate the door being open or closed.
    int randomValue = rand() % 2; // Generates 0 or 1 randomly

    // Update the door sensor state
    door_sen->isOpen = randomValue;

    // Return true if the door is open, false if closed
    return door_sen->isOpen == 1;
}

void check_temperature_range(float temp) {
    if (temp < -50.0 || temp > 50.0) {
        printf("Error: Temperature reading out of range: %.2f°C\n", temp);
    }
}

void check_humidity_range(float hud) {
    if (hud < 0.0 || hud > 100.0) {
        printf("Error: Humidity reading out of range: %.2f%%\n", hud);
    }
}

// Function to control temperature within target range
void control_temperature(TempSen* temp_sen, SolValve* sol_valve, Com* com, Con* con) {
    DoorSen door_sen;   // Initialize door sensor locally or pass as parameter from calling function
    int defrost_state = 0;  // Initialize defrost state locally or pass as parameter from calling function
    float ambient_temp = read_ambient_temp(); 

    float temp = read_temp(temp_sen, com, &door_sen,  defrost_state, ambient_temp); // Adjust parameters as needed

    if (temp == -999.0f) {
        printf("Error reading temperature. Cannot control temperature.\n");
        return; // Exit or handle the error appropriately
    }

    check_temperature_range(temp);

    if (temp < -18.0) {
        printf("Temperature is too low. Increasing temperature.\n");
        increase_temperature(sol_valve, com, con);
    } else if (temp > -16.0) {
        printf("Temperature is too high. Decreasing temperature.\n");
        decrease_temperature(sol_valve, com, con);
    } else {
        printf("Temperature is within the target range. Maintaining current state.\n");
        sol_valve->isOn = 0;
        com->isOn = 0;
        con->isOn = 0;
    }
}

// Function to control humidity within target range
void control_humidity(HudSen* hud_sen, SolValve* sol_valve, Evap* evap, Com* com) {
    check_humidity_range(hud_sen->hud);

    if (hud_sen->hud < 50.0) {
        printf("Humidity is too low. Increasing humidity.\n");
        increase_humidity(sol_valve, com);
    } else if (hud_sen->hud > 60.0) {  // Adjusted to the correct humidity range
        printf("Humidity is too high. Decreasing humidity.\n");
        decrease_humidity(sol_valve, evap, com);
    } else {
        printf("Humidity is within the target range. Maintaining current state.\n");
        sol_valve->isOn = 0;
        evap->isOn = 0;
        com->isOn = 0;
    }
}

