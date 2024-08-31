import pandas as pd
import matplotlib.pyplot as plt

try:
    # Load data from text files
    energy_data = pd.read_csv('energy_consumption.txt')
    cop_data = pd.read_csv('cop_over_time.txt')
    temp_humidity_data = pd.read_csv('temp_humidity_profiles.txt')
    heat_absorbed_data = pd.read_csv('heat_absorbed_vs_removed.txt')
    defrost_conditions_data = pd.read_csv('defrost_conditions.txt')

    # Print data to verify it was loaded correctly
    print("Energy Data Columns:", energy_data.columns)
    print("COP Data Columns:", cop_data.columns)
    print("Temperature and Humidity Data Columns:", temp_humidity_data.columns)
    print("Heat Absorbed Data Columns:", heat_absorbed_data.columns)
    print("Defrost Conditions Data Columns:", defrost_conditions_data.columns)

    # Plot Energy Consumption Over Time
    try:
        plt.figure(figsize=(12, 6))
        plt.plot(energy_data['Time(s)'], energy_data['CompressorEnergyConsumed(KWh)'], label='Compressor Energy Consumed (KWh)')
        plt.plot(energy_data['Time(s)'], energy_data['DefrostEnergyConsumed(KWh)'], label='Defrost Energy Consumed (KWh)')
        plt.plot(energy_data['Time(s)'], energy_data['TotalEnergyConsumed(KWh)'], label='Total Energy Consumed (KWh)')
        plt.xlabel('Time (s)')
        plt.ylabel('Energy (KWh)')
        plt.title('Energy Consumption Over Time')
        plt.legend()
        plt.grid(True)
        plt.savefig('energy_consumption_over_time.png')  # Save the plot as a PNG file
        plt.show()
    except KeyError as e:
        print(f"Error plotting Energy Consumption Over Time: Column {e} not found.")
    except Exception as e:
        print(f"Error plotting Energy Consumption Over Time: {e}")

    # Plot COP Over Time
    try:
        plt.figure(figsize=(12, 6))
        plt.plot(cop_data['Time(s)'], cop_data['COP'], label='COP', color='orange')
        plt.xlabel('Time (s)')
        plt.ylabel('COP')
        plt.title('COP Over Time')
        plt.legend()
        plt.grid(True)
        plt.savefig('cop_over_time.png')  # Save the plot as a PNG file
        plt.show()
    except KeyError as e:
        print(f"Error plotting COP Over Time: Column {e} not found.")
    except Exception as e:
        print(f"Error plotting COP Over Time: {e}")

    # Plot Temperature and Humidity Profiles
    try:
        plt.figure(figsize=(12, 6))
        plt.plot(temp_humidity_data['Time(s)'], temp_humidity_data['Temperature(C)'], label='Temperature (C)')
        plt.plot(temp_humidity_data['Time(s)'], temp_humidity_data['Humidity(%)'], label='Humidity (%)')
        plt.xlabel('Time (s)')
        plt.ylabel('Value')
        plt.title('Temperature and Humidity Profiles')
        plt.legend()
        plt.grid(True)
        plt.savefig('temp_humidity_profiles.png')
        plt.show()
    except KeyError as e:
        print(f"Error plotting Temperature and Humidity Profiles: Column {e} not found.")
    except Exception as e:
        print(f"Error plotting Temperature and Humidity Profiles: {e}")

    # Plot Heat Absorbed vs Heat Removed
    try:
        plt.figure(figsize=(12, 6))
        plt.plot(heat_absorbed_data['Time(s)'], heat_absorbed_data['HeatAbsorbed(kJ)'], label='Heat Absorbed (kJ)')
        plt.plot(heat_absorbed_data['Time(s)'], heat_absorbed_data['HeatRemoved(kJ)'], label='Heat Removed (kJ)')
        plt.xlabel('Time (s)')
        plt.ylabel('Heat (kJ)')
        plt.title('Heat Absorbed vs Heat Removed')
        plt.legend()
        plt.grid(True)
        plt.savefig('heat_absorbed_vs_removed.png')
        plt.show()
    except KeyError as e:
        print(f"Error plotting Heat Absorbed vs Heat Removed: Column {e} not found.")
    except Exception as e:
        print(f"Error plotting Heat Absorbed vs Heat Removed: {e}")

    # Plot Defrost Conditions
    try:
        plt.figure(figsize=(12, 6))
        plt.plot(defrost_conditions_data['Time(s)'], defrost_conditions_data['DefrostEnergyConsumed(KWh)'], label='Defrost Energy Consumed (KWh)')
        plt.plot(defrost_conditions_data['Time(s)'], defrost_conditions_data['TemperatureIncrease(C)'], label='Temperature Increase (C)')
        plt.plot(defrost_conditions_data['Time(s)'], defrost_conditions_data['HumidityDecrease(%)'], label='Humidity Decrease (%)')
        plt.xlabel('Time (s)')
        plt.ylabel('Value')
        plt.title('Defrost Conditions')
        plt.legend()
        plt.grid(True)
        plt.savefig('defrost_conditions.png')
        plt.show()
    except KeyError as e:
        print(f"Error plotting Defrost Conditions: Column {e} not found.")
    except Exception as e:
        print(f"Error plotting Defrost Conditions: {e}")

except Exception as e:
    print(f"An error occurred: {e}")
