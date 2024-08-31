#include "main.h"
/**
 * initialize_components - a function of the initiate state of the components
 * @com: Compressor initially off
 * @evap: Evaporator initially off
 * @exp_valve: Exapnsion valve initially off
 * @sol_valve: Solenoid valve initially off
 * @hot_gas_bypass: Hot gas bypass initially off
 */

void initialize_components(Com* com, Con* con, Evap* evap, ExpValve* exp_valve, SolValve* sol_valve, HotGasBypass* hot_gas_bypass) {
	/*Initialize the compressor*/
	com->isOn = 0; 
        com->power = 0.0; 

        con->isOn = false; 
        /* Initialize the evaporator*/
        evap->isOn = false; 

        /*Initialize the expansion valve*/
        exp_valve->isOpen = false;

        /*Initialize the solenoid valve*/
        sol_valve->isOn = false; 

        /*Initialize the hot gas bypass valve*/
	hot_gas_bypass->isActive = false; 
}
