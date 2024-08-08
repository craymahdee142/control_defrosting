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
extern int read_hud(HudSen *hud_sen);

/* Calculate deltas function */
void calculate_deltas(void) {
    delta_T = read_ambient_temp() - read_temp(&temp_sen);
    delta_W = read_ambient_hud() - read_hud(&hud_sen);
}

/* Function definitions */
float calculate_heat_gain_walls_roof(float U, float A, float delta_T) {
    return U * A * delta_T;
}

float calculate_heat_gain_product(float mass, float specific_heat, float delta_T, float usage_factor) {
    return mass * specific_heat * delta_T * usage_factor;
}

float calculate_heat_gain_infiltration(float CFM, float delta_T) {
    return 1.1 * CFM * delta_T;
}

float calculate_latent_heat_gain(float CFM, float delta_W) {
    return 0.68 * CFM * delta_W;
}

