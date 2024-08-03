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

/* Simulation parameters */
#define SIMULATION_DURATION 160 /* Simulate for 160 secs for 24 hours ratio (in seconds) */
#define CHECK_INTERVAL 5 /* Check sensors every 5 seconds */
#define MINIMUM_DEFROST_INTERVAL 18

#define TIME_RATIO 540 /* 540 secs for 9 mins in real time */

/* Constants */
#define COLD_ROOM_VOLUME 0.78 // in cubic meters (1.275m x 0.825m x 0.740m)
#define AIR_DENSITY 1.2 // in kg/m^3
#define AIR_SPECIFIC_HEAT 1005.0 // in J/kg·°C
#define COMPRESSOR_POWER 0.194 // Example value in kW

//#define AMBIENT_TEMPERATURE 25.0 

#define LOW_P 106.7   /*Low pressure in Kpa */
#define HIGH_P 1016.4 /*High pressure in kpa */
/*define MAX_DATA_ROWS 1000*/

#define CONTROL_UNIT_ENERGY_CONSUMPTION 0.05 // Example: 0.05 Wh per interval
#define SENSOR_ENERGY_CONSUMPTION 0.001
#define NUM_SENSORS 3

/*
typedef struct {
    double temperature;
    double pressure;
    double enthalpy;
    double entropy;
} RefrigerantProperties;
*/


/* Sensor Structures */
typedef struct {
    float temperature;
} TemperatureSensor;

typedef struct {
    int isOpen;
} DoorSensor;

typedef struct {
    float humidity;
} HumiditySensor;

/* System Components */
typedef struct {
    int isOn;
    float power;
} Compressor;

typedef struct {
    bool isOn;
} Condenser;

typedef struct {
    bool isOn;
} Evaporator;

typedef struct {
    bool isOpen;
} ExpansionValve;

typedef struct {
    bool isOn;
} SolenoidValve;

typedef struct {
    bool isActive;
} HotGasBypass;

/* Control functions */
float random_float(float min, float max);
float read_ambient_temperature();
float read_surrounding_temperature();
int read_humidity(HumiditySensor* humidity_sensor);
float random_float(float min, float max);

double calculate_hot_gas_power(double mass_flow_rate, float h_1, float h_2, double efficiency);
void toggle_door_state(DoorSensor* door_sensor);

/* Prototypes */
float read_temperature(); /* Reads the temperature from the temperature sensor */
bool read_door_sensor(DoorSensor* door_sensor); /* Reads the state of the door sensor (open/closed) */
float calculate_cop(float h_1, float h_2, float h_3, float h_4, float mass_flow_rate);


float calculate_enthalpy_evaporator(float tempearture, float ambient_temperature);
float calculate_enthalpy_compressor_outlet(float tempearture, float ambient_temperature);
float calculate_enthalpy_condenser(float tempearture, float surrounding_temperature);
float calculate_enthalpy_expansion_valve(float tempearture, float surrounding_temperature);

float calculate_enthalpy(float ambient_temperature, float surrounding_temperature, float tempearture);



/* Simulate effects */
void simulate_compressor_effect(float* temperature, float ambient_temperature, float* energy_consumed);
void simulate_condenser_effect(float* temperature, float surrounding_temperature);
void simulate_evaporator_effect(float* temperature, float surrounding_temperature);
void simulate_expansion_valve_effect(ExpansionValve* expansion_valve);
void simulate_solenoid_valve_effect(SolenoidValve* solenoid_valve);
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor);
void simulate_heater_effect(TemperatureSensor* temp_sensor, float* defrost_energy_consumed);
void simulate_fan_effect(HumiditySensor* humidity_sensor);
void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment, int* door_open_time);

void initialize_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor);
void initialize_components(Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass);

void update_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor);
void control_system(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor, Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed);
void simulate_system(float external_temp);

#endif

