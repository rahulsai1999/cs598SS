import pandas as pd
import matplotlib.pyplot as plt

def plot_util_cpu(csv_file_name, db_type, hardware_type):
    # Load the data from the CSV file and strip any trailing spaces
    data = pd.read_csv(csv_file_name, sep=',', dtype=str)
    # Strip whitespace from column names and values
    data.columns = data.columns.str.strip()
    data = data.apply(lambda x: x.str.strip() if x.dtype == "object" else x)
    print("data is:\n", data)
    # Convert the 'Time' column to datetime objects
    data['Time'] = pd.to_datetime(data['Time'])
    data['Time'] = pd.to_datetime(data['Time'])
    data['util%'] = data['util%'].str.strip()
    data['cpu%'] = data['cpu%'].str.strip()
    data['util%'] = pd.to_numeric(data['util%'])
    data['cpu%'] = pd.to_numeric(data['cpu%'])

    plt.figure(figsize=(12, 6))
    plt.plot(data['Time'], data['util%'])
    plt.title(db_type + " " + hardware_type + " Disk")
    plt.xlabel("Time")
    plt.ylabel("util%")
    plt.ylim(0, 100)
    plt.yticks(range(0, 101, 20))
    plt.grid(True)
    plt.savefig(db_type + "_" + hardware_type + "_disk.png")

    plt.figure(figsize=(12, 6))
    plt.plot(data['Time'], data['cpu%'])
    plt.title(db_type + " " + hardware_type + " CPU")
    plt.xlabel("Time")
    plt.ylabel("cpu%")
    plt.ylim(0, 100)
    plt.yticks(range(0, 101, 20))
    plt.grid(True)
    plt.savefig(db_type + "_" + hardware_type + "_cpu.png")

    plt.show()
if __name__ == '__main__':
    csv_file_name = "data/splinterdb/outWorkloadA.txt/utilization.csv"
    db_type = "SplinterDB"
    hardware_type = "SATA"
    plot_util_cpu(csv_file_name, db_type, hardware_type)

