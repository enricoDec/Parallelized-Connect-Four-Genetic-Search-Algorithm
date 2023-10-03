import csv

csv_file_path = '../benchmark.csv'
search_times = {}
num_entries = 0

with open(csv_file_path, 'r') as csv_file:
    csv_reader = csv.DictReader(csv_file)
    for row in csv_reader:
        search_time = float(row['average search time (turn)'])
        threads = int(row['threads'])
        if threads not in search_times:
            search_times[threads] = []
        search_times[threads].append(search_time)
        num_entries += 1

for threads, times in search_times.items():
    if len(times) == 0:
        continue
    avg_search_time = sum(times) / len(times)
    print(f'Average search time for {threads} threads: {avg_search_time}')
