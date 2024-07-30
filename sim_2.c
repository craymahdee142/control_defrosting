#include "main.h"



void initialize_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor) {
    temp_sensor->temperature = read_temperature();
    door_sensor->isOpen = 0;
    humidity_sensor->humidity = read_humidity(humidity_sensor);
}

void initialize_components(Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass) {
    compressor->isOn = 0;
    compressor->power = COMPRESSOR_POWER;
    condenser->isOn = 0;
    evaporator->isOn = 0;
    expansion_valve->isOpen = 0;
    solenoid_valve->isOn = 0;
    hot_gas_bypass->isActive = 0;
}

void update_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor) {
    temp_sensor->temperature = read_temperature();
    humidity_sensor->humidity = read_humidity(humidity_sensor);
}

void control_system(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor, Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed) {
    if (temp_sensor->temperature > 5.0) {
        compressor->isOn = 1;
        condenser->isOn = 1;
    } else {
        compressor->isOn = 0;
        condenser->isOn = 0;
    }

    if (humidity_sensor->humidity > 50.0) {
        evaporator->isOn = 1;
    } else {
        evaporator->isOn = 0;
    }

    if (door_sensor->isOpen) {
        expansion_valve->isOpen = 1;
    } else {
        expansion_valve->isOpen = 0;
    }

    if (temp_sensor->temperature < 0.0 && humidity_sensor->humidity < 40.0) {
        solenoid_valve->isOn = 1;
    } else {
        solenoid_valve->isOn = 0;
    }

    if (compressor->isOn) {
        simulate_compressor_effect(&temp_sensor->temperature, read_ambient_temperature(), energy_consumed);
    }

    if (evaporator->isOn) {
        simulate_evaporator_effect(&temp_sensor->temperature, read_surrounding_temperature());
    }

    if (expansion_valve->isOpen) {
        simulate_expansion_valve_effect(expansion_valve);
    }

    if (solenoid_valve->isOn) {
        simulate_solenoid_valve_effect(solenoid_valve);
    }

    if (hot_gas_bypass->isActive) {
        simulate_hot_gas_bypass_effect(hot_gas_bypass, temp_sensor);
    }
}
