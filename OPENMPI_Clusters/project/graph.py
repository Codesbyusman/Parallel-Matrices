import csv
import matplotlib.pyplot as plt

# Read data from CSV
with open('./graphs.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)

# Separate the data into processes and time
processes, times = zip(*[(int(row[0]), float(row[1])) for row in data])

# Plot the data as a bar chart
plt.bar(processes, times, color={'blue', 'red', 'green', 'orange', 'purple'})
plt.title('Comparison of Execution Time for Different Processes')
plt.xlabel('Number of Processes')
plt.ylabel('Time (seconds)')

# Set x-axis ticks to display only even values
even_processes = [p for p in processes if p % 2 == 0]
plt.xticks(even_processes)

plt.grid(axis='y', linestyle='--', alpha=0.4)
plt.show()
