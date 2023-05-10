import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_disk_throughput(hardware_type, db_type):
    num_threads = [2, 4, 8, 12, 16]
    disk_throughput_lst = []
    actual_throughput_lst = []
    for num_thread in num_threads:
        csv_file_name = f"data/final/{hardware_type}/{db_type}/outWorkloadA.txt{num_thread}-node1-20230509.tab"
        df = pd.read_csv(csv_file_name, sep=' ', dtype=str)
        df['DateTime'] = pd.to_datetime(df['Date'] + ' ' + df['Time'])
        start = df['DateTime'][df.index[0]]
        end = df['DateTime'][df.index[-1]]
        elapsed = end - start
        actual_throughput = ((5e7 * 124) / 1e6) / (elapsed.total_seconds())
        print(elapsed.total_seconds())
        actual_throughput_lst.append(actual_throughput)
        max_throughput_per_ts = max(pd.to_numeric(df['[DSK]WriteKBTot'])) * 1e-3
        disk_throughput_lst.append(max_throughput_per_ts)
        if num_thread == 12:
            plot_cpu(csv_file_name, db_type, hardware_type)
            plot_disk(csv_file_name, db_type, hardware_type)
    # print(disk_throughput)
    plt.plot(num_threads, disk_throughput_lst, linestyle='-', marker='')
    plt.xlabel('Num Threads')
    plt.ylabel('Disk Throughput MB/s')
    title = db_type + "_" + hardware_type + '_disk_throughput.png'
    plt.title(title)
    filename = 'data/final/' + hardware_type + '/' + db_type + "/" + title
    plt.savefig(filename)
    plt.show()
    plt.plot(num_threads, actual_throughput_lst, linestyle='-', marker='')
    plt.xlabel('Num Threads')
    plt.ylabel('Actual Throughput MB/s')
    title = db_type + "_" + hardware_type + '_actual_throughput.png'
    plt.title(title)
    filename = 'data/final/' + hardware_type + '/' + db_type + "/" + title
    plt.savefig(filename)
    plt.show()

def plot_cpu(csv_file_name, db_type, hardware_type):
    # Load the data from the CSV file and strip any trailing spaces
    df = pd.read_csv(csv_file_name, sep=' ', dtype=str)
   # Combine the "Date" and "Time" columns into a single "DateTime" column
    df['DateTime'] = pd.to_datetime(df['Date'] + ' ' + df['Time'])
    # Plot the data as a line graph
    df['[CPU]Totl%'] = pd.to_numeric(df['[CPU]Totl%'])
    plt.plot(df['DateTime'], df['[CPU]Totl%'], linestyle='-', marker='')

    # Set the x-axis label
    plt.xlabel('Timestamp')

    # Set the y-axis label
    plt.ylabel('CPU Utilization%')

    # Set the y-axis limits
    plt.ylim(1, 100)

    # Set the title of the graph
    title = db_type + "_" + hardware_type + '_cpu.png'
    plt.title(title)
    # Save the graph to a file
    filename = 'data/final/' + hardware_type + '/' + db_type + "/" + title
    print(filename)
    plt.savefig(filename)
    plt.show()

def plot_disk(csv_file_name, db_type, hardware_type):
    # Load the data from the CSV file and strip any trailing spaces
    df = pd.read_csv(csv_file_name, sep=' ', dtype=str)
   # Combine the "Date" and "Time" columns into a single "DateTime" column
    df['DateTime'] = pd.to_datetime(df['Date'] + ' ' + df['Time'])
    # Plot the data as a line graph
    df['[DSK]WriteKB%'] = (pd.to_numeric(df['[DSK]WriteKBTot'])/1572000.0) *100
    plt.plot(df['DateTime'], df['[DSK]WriteKB%'], linestyle='-', marker='')

    # Set the x-axis label
    plt.xlabel('Timestamp')

    # Set the y-axis label
    plt.ylabel('Disk Utilization%')

    # Set the y-axis limits
    plt.ylim(1, 100)

    # Set the title of the graph
    title = db_type + hardware_type + '_disk.png'
    plt.title(title)

    # Save the graph to a file
    filename = 'data/final/' + hardware_type + '/' + db_type + "/" + title
    print(filename)
    plt.savefig(filename)
    plt.show()
if __name__ == '__main__':
    db_type = "rocksdb"
    hardware_type = "nvme"
    plot_disk_throughput(hardware_type, db_type)
    # plot_disk(csv_file_name_disk, db_type, hardware_type)
    # plot_cpu(csv_file_name_cpu, db_type, hardware_type)

