import pandas as pd
import matplotlib.pyplot as plt

try:
    # Load data from text files
    energy_data = pd.read_csv('energy_consumption.txt')
    cop_data = pd.read_csv('cop_over_time.txt')
    ph_data = pd.read_csv('ph_values.txt')

    # Plot Energy Consumption Over Time
    plt.figure(figsize=(12, 6))
    plt.plot(energy_data['Time(s)'], energy_data['CompressorEnergyConsumed(Wh)'], label='Compressor Energy Consumed (Wh)')
    plt.plot(energy_data['Time(s)'], energy_data['DefrostEnergyConsumed(Wh)'], label='Defrost Energy Consumed (Wh)')
    plt.plot(energy_data['Time(s)'], energy_data['TotalEnergyConsumed(Wh)'], label='Total Energy Consumed (Wh)')
    plt.xlabel('Time (s)')
    plt.ylabel('Energy (Wh)')
    plt.title('Energy Consumption Over Time')
    plt.legend()
    plt.grid(True)
    plt.savefig('energy_consumption_over_time.png')  # Save the plot as a PNG file
    plt.show()

    # Plot COP Over Time
    plt.figure(figsize=(12, 6))
    plt.plot(cop_data['Time(s)'], cop_data['COP'], label='COP', color='orange')
    plt.xlabel('Time (s)')
    plt.ylabel('COP')
    plt.title('COP Over Time')
    plt.legend()
    plt.grid(True)
    plt.savefig('cop_over_time.png')  # Save the plot as a PNG file
    plt.show()

    # Plot p-h Graph
    plt.figure(figsize=(12, 6))
    plt.plot(ph_data['H1'], ph_data['P1'], 'bo-', label='Evaporator Outlet')
    plt.plot(ph_data['H2'], ph_data['P2'], 'ro-', label='Compressor Outlet')
    plt.plot(ph_data['H3'], ph_data['P3'], 'go-', label='Condenser Inlet')
    plt.plot(ph_data['H4'], ph_data['P4'], 'mo-', label='Expansion Valve Inlet')
    plt.xlabel('Enthalpy (kJ/kg)')
    plt.ylabel('Pressure (kPa)')
    plt.title('p-h Diagram')
    plt.legend()
    plt.grid(True)
    plt.savefig('ph_diagram.png')  # Save the plot as a PNG file
    plt.show()

except Exception as e:
    print(f"An error occurred: {e}")

