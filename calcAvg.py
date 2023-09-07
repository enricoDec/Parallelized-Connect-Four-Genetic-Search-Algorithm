import csv

# Replace 'your_csv_file.csv' with the actual CSV file path
csv_file_path = 'benchmark.csv'

# Initialize variables to calculate the average search time
total_search_time = 0.0
num_entries = 0

# Open the CSV file for reading
with open(csv_file_path, 'r') as csv_file:
    csv_reader = csv.DictReader(csv_file)
    # collect all the search times in a list separating by number of threads
    search_times = [[],[],[],[],[],[],[],[],[],[]]
    # Iterate through each row in the CSV file
    for row in csv_reader:
        # Extract the 'average search time' value from the row and convert it to a float
        search_time = float(row['average search time (turn)'])
        
        # Extract the 'threads' value from the row and convert it to an integer
        threads = int(row['threads'])

        # Add the search time to the total search time
        total_search_time += search_time

        # Add the search time to the list of search times
        search_times[threads].append(search_time)

        # Increment the number of entries
        num_entries += 1

# Calculate the average search time for each number of threads
for i in range(10):
    if len(search_times[i]) == 0:
        continue
    avg_search_time = sum(search_times[i]) / len(search_times[i])
    print(f'Average search time for {i} threads: {avg_search_time}')
