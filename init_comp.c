#include "main.h"

void initialize_components(Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass) {
    // Initialize the compressor
    com->isOn = 0; // Example: compressor is initially off
    com->power = 0.0; // Initialize power (use power instead of pressure)

    // Initialize the condenser
    con->isOn = false; // Set the initial state (use isOn instead of temperature and pressure)

    // Initialize the evaporator
    evap->isOn = false; // Set the initial state (use isOn instead of temperature and pressure)

    // Initialize the expansion valve
    exp_valve->isOpen = false; // Example: expansion valve is initially closed

    // Initialize the solenoid valve
    sol_valve->isOn = false; // Example: solenoid valve is initially off

    // Initialize the hot gas bypass valve
    hot_gas_bypass->isActive = false; // Example: hot gas bypass is initially inactive
}

