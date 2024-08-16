#include "main.h"


/* Declare global variables for deltas 
extern float delta_T;
extern float delta_W;

 Declare extern variables for sensors 
extern TempSen temp_sen;
extern HudSen hud_sen;

 Function declarations 
extern float read_ambient_temp(void);
extern float read_ambient_hud(void);
extern float read_temp(TempSen *temp_sen);
extern float read_hud(HudSen *hud_sen);

/Calculate deltas function 
void calculate_deltas(void) {
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
*/
/*
void check_temperature_range(float temp) {
    if (temp < -50.0 || temp > 50.0) {
        printf("Error: Temperature reading out of range: %.2f°C\n", temp);
    }
}

void check_humidity_range(float hud) {
    if (hud < 0.0 || hud > 100.0) {
        printf("Error: Humidity reading out of range: %.2f%%\n", hud);
    }
}*/


/* Function definitions
float calculate_heat_gain_walls_roof(float U, float A, float delta_T) {
    float heat_gain_w = U * A * delta_T;
    float heat_gain_kw = W_TO_KW(heat_gain_w);
    printf("Heat gain through walls/roof: %.2f W (%.2f kW)\n", heat_gain_w, heat_gain_kw);
    return heat_gain_kw;
}

float calculate_heat_gain_air(float mass_of_air, float specific_heat_air, float delta_T) {
    float heat_gain_w = mass_of_air * specific_heat_air * delta_T;
    float heat_gain_kw = W_TO_KW(heat_gain_w);
    printf("Heat gain through air: %.2f W (%.2f kW)\n", heat_gain_w, heat_gain_kw);
    return heat_gain_kw;
}

float calculate_heat_gain_product(float mass, float specific_heat_air, float delta_T, float usage_factor) {
    float heat_gain_w = mass * specific_heat_air * delta_T * usage_factor;
    float heat_gain_kw = W_TO_KW(heat_gain_w);
    printf("Heat gain through product: %.2f W (%.2f kW)\n", heat_gain_w, heat_gain_kw);
    return heat_gain_kw;
}

float calculate_heat_gain_infiltration(float CFM, float delta_T) {
    float heat_gain_w = 1.1 * CFM * delta_T;
    float heat_gain_kw = W_TO_KW(heat_gain_w);
    printf("Heat gain through infiltration: %.2f W (%.2f kW)\n", heat_gain_w, heat_gain_kw);
    return heat_gain_kw;
}

float calculate_latent_heat_gain(float CFM, float delta_W) {
    float latent_heat_gain_w = 0.68 * CFM * delta_W;
    float latent_heat_gain_kw = W_TO_KW(latent_heat_gain_w);
    printf("Latent heat gain: %.2f W (%.2f kW)\n", latent_heat_gain_w, latent_heat_gain_kw);
    return latent_heat_gain_kw;
} */
