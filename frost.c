#include "main.h"

float get_frost_level(float current_temp, float current_hud, float target_temp_low, float target_temp_high, float target_humidity_low, float target_humidity_high) {
    float frost_level = 0.0;

    if (current_temp <= target_temp_high && current_temp >= target_temp_low) {
        if (current_hud >= target_humidity_low && current_hud <= target_humidity_high) {
            float temp_factor = (target_temp_high - current_temp) / (target_temp_high - target_temp_low);
            float humidity_factor = (current_hud - target_humidity_low) / (target_humidity_high - target_humidity_low);
            frost_level = temp_factor * humidity_factor * 100.0;
        }
    }

    return frost_level;
}

// Functions for managing temperature and humidity
void increase_temperature(SolValve* sol_valve, Com* com, Con* con) {
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);
    com->isOn = 1;
    usleep(1000000);
    sol_valve->isOn = 0;
    printf("Increasing temperature.\n");
}

void decrease_temperature(SolValve* sol_valve, Com* com, Con* con) {
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;
        printf("Stopping hot gas bypass.\n");
    }
    com->isOn = 1;
    con->isOn = 1;
    printf("Decreasing temperature.\n");
}

void increase_humidity(SolValve* sol_valve, Com* com) {
    sol_valve->isOn = 1;
    simulate_sol_valve_effect(sol_valve);
    com->isOn = 1;
    usleep(3000000);
    sol_valve->isOn = 0;
    printf("Increasing humidity.\n");
}

void decrease_humidity(SolValve* sol_valve, Evap* evap, Com* com) {
    if (sol_valve->isOn) {
        sol_valve->isOn = 0;
        printf("Stopping hot gas bypass.\n");
    }
    evap->isOn = 1;
    com->isOn = 1;
    printf("Decreasing humidity.\n");
}

