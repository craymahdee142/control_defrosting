#include "main.h"

float calculate_volume(float length, float width, float height) {
    return length * width * height;
}

float calculate_heat_gain_walls_roof(float U, float A, float delta_T) {
    return U * A * delta_T;
}

float calculate_heat_gain_lid(float U, float A, float delta_T) {
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
