#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h> 

#define HOT_GAS_EFFICIENCY 0.85 /* efficiency of the hot gas 85% */
#define TARGET_TEMP 5.0 /*Target temperature during defrosting in °C*/
#define TEMP_INCREASE 1.0 /* Temperature increase per step in °C*/
#define DEFROST_STEP_DURATION 1.0 /* Duration of defrost step in seconds*/

#define DEFROST_EFFICIENCY 0.20 

#define SPECIFIC_HEAT_OF_ICE 2093 /*in J/kg°C */
#define LATENT_HEAT_OF_ICE 333.55  /*kJ/kg for ice*/
#define ICE_MASS 0.01   

#define FROST_THRESHOLD 0.15 

#define TARGET_TEMP_LOW -20.0
#define TARGET_TEMP_HIGH -18.0
#define TARGET_HUMIDITY_LOW 50.0
#define TARGET_HUMIDITY_HIGH 60.0

/*
#define COOLING_RATE 0.5f
#define DOOR_HEAT_GAIN 2.0f
#define DEFROST_HEAT_GAIN 5.0f
#define DOOR_HUMIDITY_GAIN 3.0f
#define HEAT_TRANSFER_COEFF 0.1f
#define HUMIDITY_TRANSFER_COEFF 0.1f
*/

/* Simulation parameters */
#define SIMULATION_DURATION 160 /* Simulate for 160 secs for 24 hours ratio (in seconds) */
#define CHECK_INTERVAL 5 /* Check sensors every 5 seconds */
#define MINIMUM_DEFROST_INTERVAL 15

#define TIME_RATIO 540 /* 540 secs for 9 mins in real time */


#define BASE_TEMP_C -18.0
#define FACTOR 0.08


/* Constants */
#define COLD_ROOM_VOLUME 0.8046 /*in cubic meters (1.45m x 0.895m x 0.62m for 400L)*/
#define AIR_DENSITY 1.2 /* in kg/m^3*/
#define AIR_SPECIFIC_HEAT 1.005 /* in J/kg·°C*/

/*Define constants for temperature rise and humidity drop during defrost*/
#define TEMP_RISE_DURING_DEFROST 5.0  
#define HUMIDITY_DROP_DURING_DEFROST 10.0  


#define CONTROL_UNIT_ENERGY_CONSUMPTION 0.05 
#define SENSOR_ENERGY_CONSUMPTION 0.001
#define NUM_SENSORS 3

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


/* Control functions */
float random_float(float min, float max);
float read_ambient_temp();

float read_hud();
float random_float(float min, float max);
float read_ambient_hud();
float read_temp();
bool read_door_sen(DoorSen* door_sen);

void check_temperature_range(float temp);
void check_humidity_range(float hud);

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
float calculate_cop(float Q_evap, float W_com);


float calculate_enthalpy_evap(float ambient_temp);
float calculate_enthalpy_com(float ambient_temp);
float calculate_enthalpy_con(float ambient_temp);
float calculate_enthalpy_exp_valve(float ambient_temp);

float calculate_entropy_evap(float ambient_temp);
float calculate_entropy_com(float ambient_temp);

float calculate_enthalpy(float ambient_temp, float pressure);

/* Simulate effects */
void simulate_com_effect(float temp, float ambient_temp, float* energy_consumed);
void simulate_con_effect(float ambient_temp, float* energy_rejected);
void simulate_evap_effect(float ambient_temp, float* energy_absorbed);
void simulate_exp_valve_effect(float ambient_temp, float* temp, float* energy_consumed);
void simulate_sol_valve_effect(SolValve* sol_valve);
void simulate_time_delay(int duration);
void simulate_hot_gas_bypass_defrosting(float* temp, float* defrost_energy_consumed, float ambient_temp);
void simulate_fan_effect(HudSen* hud_sen);

void adjust_for_door_state(TempSen* temp_sen, HudSen* hud_sen, DoorSen* door_sen);
void initialize_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen);
void initialize_components(Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass);

void update_sensors(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, float ambient_temp, float ambient_humidity);
void control_system(TempSen* temp_sen, DoorSen* door_sen, HudSen* hud_sen, Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed);
void simulate_system(float external_temp);


float get_current_time();

int control_defrosting_conditions(TempSen* temp_sen, HudSen* hud_sen, int* defrost_needed, float* last_defrost_time);

#endif
