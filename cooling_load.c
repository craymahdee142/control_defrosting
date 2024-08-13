#include "main.h"

/* Declare global variables for deltas */
extern float delta_T;
extern float delta_W;

/* Declare extern variables for sensors */
extern TempSen temp_sen;
extern HudSen hud_sen;

/* Function declarations */
extern float read_ambient_temp(void);
extern float read_ambient_hud(void);
extern float read_temp(TempSen *temp_sen);
extern float read_hud(HudSen *hud_sen);

/* Calculate deltas function */
void calculate_deltas(void) {
    /*
    delta_T = read_ambient_temp() - read_temp(&temp_sen);
    delta_W = read_ambient_hud() - read_hud(&hud_sen);*/
    float ambient_temp = read_ambient_temp();
    float temp = read_temp(&temp_sen);
    float ambient_hud = read_ambient_hud();
    float hud = read_hud(&hud_sen);

    delta_T = ambient_temp - temp;
    delta_W = ambient_hud - hud;

    // Debugging prints
    printf("Ambient Temp: %.2f°C\n", ambient_temp);
    printf("Sensor Temp: %.2f°C\n", temp);
    printf("Delta_T: %.2f°C\n", delta_T);

    printf("Ambient Humidity: %.2f%%\n", ambient_hud);
    printf("Sensor Humidity: %.2f%%\n", hud);
    printf("Delta_W: %.2f%%\n", delta_W);
}

/* Function definitions */
float calculate_heat_gain_walls_roof(float U, float A, float delta_T) {
    return U * A * delta_T;
}

float calculate_heat_gain_air(float mass_of_air, float specific_heat_air, float delta_T) {
    return mass_of_air * specific_heat_air * delta_T;
}


float calculate_heat_gain_product(float mass, float specific_heat_air, float delta_T, float usage_factor) {
    return mass * specific_heat_air * delta_T * usage_factor;
}

float calculate_heat_gain_infiltration(float CFM, float delta_T) {
    return 1.1 * CFM * delta_T;
}

float calculate_latent_heat_gain(float CFM, float delta_W) {
    return 0.68 * CFM * delta_W;
}

