#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h> // For boolean type

#define HOT_GAS_EFFICIENCY 0.85 /* efficiency of the hot gas 80% */
#define TARGET_TEMP 5.0 // Target temperature during defrosting in °C
#define TEMP_INCREASE 1.0 // Temperature increase per step in °C
#define DEFROST_STEP_DURATION 1.0 // Duration of defrost step in seconds



/* Simulation parameters */
#define SIMULATION_DURATION 160 /* Simulate for 160 secs for 24 hours ratio (in seconds) */
#define CHECK_INTERVAL 5 /* Check sensors every 5 seconds */
#define MINIMUM_DEFROST_INTERVAL 10

#define TIME_RATIO 540 /* 540 secs for 9 mins in real time */

/* Constants */
#define COLD_ROOM_VOLUME 0.2954 // in cubic meters (0.632m x 0.85m x 0.55m for 142L)
#define AIR_DENSITY 1.2 // in kg/m^3
#define AIR_SPECIFIC_HEAT 1005.0 // in J/kg·°C
#define COMPRESSOR_POWER 0.165 /* Compressor power in KW */


/*Cooling load constants */
/*
#define U 0.25;example value
#define A 10.0; example value
#define PRODUCT_MASS 100.0; Mass of product
#define PRODUCT_SPECIFIC_HEAT 3.5; Product specific heat
#define USAGE_FACTOR 0.8;product usgae factor
#define INFILTRATION_CFM 500.0; example value*/


#define LOW_P 58.99   /*Low pressure in Kpa */
#define HIGH_P 535.9 /*High pressure in kpa */

#define TOLERANCE 0.01 /* For precise cal*/

#define CONTROL_UNIT_ENERGY_CONSUMPTION 0.05 // Example: 0.05 Wh per interval
#define SENSOR_ENERGY_CONSUMPTION 0.001
#define NUM_SENSORS 3

/*Define global variables*/
extern float delta_T;
extern float delta_W;

#define MIN_SUPERHEATING 5.0  // Minimum superheating temperature in °C
#define MAX_SUPERHEATING 10.0 // Maximum superheating temperature in °C
#define MIN_SUBCOOLING 5.0    // Minimum subcooling temperature in °C
#define MAX_SUBCOOLING 10.0   // Maximum subcooling temperature in °C


/* Sensor Structures */
typedef struct {
    float temp;
} TempSen;

typedef struct {
    bool isOpen;
} DoorSen;

typedef struct {
    float hud;
} HudSen;

/* System Components*/
typedef struct {
    bool isOn;
    float power;
} Com;

typedef struct {
    bool isOn;
} Con;

typedef struct {
    bool isOn;
} Evap;

typedef struct {
    bool isOpen;
} ExpValve;

typedef struct {
    bool isOn;
} SolValve;

typedef struct {
    bool isActive;
} HotGasBypass;

/*
typedef struct {
    int isOn;  // Correct member name should match the code in initialize_components.c
    float pressure;  // Check if this member exists in the Com struct
    float energy_consumed;  // Check if this member exists in the Com struct
} Com;

typedef struct {
    float temp;  // Check if this member exists in the Con struct
    float pressure;  // Check if this member exists in the Con struct
} Con;

typedef struct {
    float temp;  // Check if this member exists in the Evap struct
    float pressure;  // Check if this member exists in the Evap struct
} Evap;

typedef struct {
    int state;  // Check if this member exists in the ExpValve struct
    float pressure;  // Check if this member exists in the ExpValve struct
} ExpValve;

typedef struct {
    int isOn;  // Check if this member exists in the SolValve struct
} SolValve;

typedef struct {
    int isOn;  // Check if this member exists in the HotGasBypass struct
    float pressure;  // Check if this member exists in the HotGasBypass struct
} HotGasBypass;
*/

/* Control functions */
float random_float(float min, float max);
float read_ambient_temp();
float read_surrounding_temp();
float read_hud(HudSen* hud_sen);
float random_float(float min, float max);
float read_ambient_hud();
float read_temp(TempSen* temp_sen); /* Reads the temp from the temperature sensor */

/* Frost level check*/
void increase_temperature(SolValve* sol_valve, Com* com, Con* ccn);
void decrease_temperature(SolValve* sol_valve, Com* com, Con* con);
void increase_humidity(SolValve* sol_valve, Com* com);
void decrease_humidity(SolValve* sol_valve, Evap* evap, Com* com);
float get_frost_level(float current_temp, float current_hud, float target_temp_low, float target_temp_high, float target_humidity_low, float target_humidity_high);


void control_humidity(HudSen* hud_sen, SolValve* sol_valve, Evap* evap, Com* com);
void control_temperature(TempSen* temp_sen, SolValve* sol_valve, Com* com, Con* con);

double calculate_hot_gas_power(double mass_flow_rate, float h_1, float h_2, double efficiency);
void toggle_door_state(DoorSen* door_sen);

/* Prototypes */
bool read_door_sen(DoorSen* door_sen); /* Reads the state of the door sen (open/closed) */
//float calculate_cop(float h_1, float h_2, float total_heat_gain, float mass_flow_rate);

float calculate_cop(float total_heat_gain, float W_com);
//float calculate_mass_flow_rate(float total_heat_gain, float h_1, float h_4);

float calculate_enthalpy_evap(float low_pressure, float evap_temp, float ambient_temp, float superheating);
float calculate_enthalpy_com(float high_pressure, float con_temp, float ambient_temp);
float calculate_enthalpy_con(float low_pressure, float con_temp, float suncooling, float ambient_temp);
float calculate_enthalpy_exp_valve(float low_pressure, float evap_temp);

float calculate_enthalpy(float ambient_temp, float pressure);

/* Simulate effects */
void simulate_com_effect(float temp_sensor_reading, float* evap_temp, float ambient_temp, float* energy_consumed);
void simulate_con_effect(float temp_sensor_reading, float* con_temp, float ambient_temp, float subcooling);
void simulate_evap_effect(float temp_sensor_reading, float* evap_temp, float ambient_temp, float superheating);
void simulate_exp_valve_effect(ExpValve* exp_valve);
void simulate_sol_valve_effect(SolValve* sol_valve);
//void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TempSen* temp_sen);
void simulate_hot_gas_bypass_defrosting(float pressure, float* con_temp, float* defrost_energy_consumed);
//void simulate_heater_effect(TempSen* temp_sen, float* defrost_energy_consumed);
void simulate_fan_effect(HudSen* hud_sen);
void adjust_for_door_state(DoorSen* door_sen, float* temp_adjustment, int* door_open_time);

void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen);
void initialize_components(Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass);

void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen);
void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed);
void simulate_system(float external_temp);

/* Calculate cooling load*/
float calculate_heat_gain_walls_roof(float U, float A, float delta_T);
float calculate_heat_gain_product(float mass, float specific_heat_air, float delta_T, float usage_factor);
float calculate_heat_gain_infiltration(float CFM, float delta_T);
float calculate_latent_heat_gain(float CFM, float delta_W);
float calculate_heat_gain_air(float mass_of_air, float specific_heat_air, float delta_T);

void calculate_deltes();

float get_current_time();


#endif

