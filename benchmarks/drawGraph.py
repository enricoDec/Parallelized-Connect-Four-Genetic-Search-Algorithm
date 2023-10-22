import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def bar_plot_comparison_log(csv1, csv2):
    df1 = pd.read_csv(csv1)
    df2 = pd.read_csv(csv2)
    grouped1 = df1.groupby('threads')['average search time (turn)'].mean()
    grouped2 = df2.groupby('threads')['average search time (turn)'].mean()
    custom_labels = ['1', '2', '3', '4']
    plt.figure(figsize=(12, 6))
    width = 0.35  # Width of each bar
    x = range(len(grouped1))
    x1 = [i - width/2 for i in x]
    x2 = [i + width/2 for i in x]
    plt.bar(x1, grouped1.values, width=width, alpha=0.7, color='orange', label='Non-distributed')
    plt.bar(x2, grouped2.values, width=width, alpha=0.7, color='grey', label='Distributed')
    plt.yscale('log')
    plt.title('Average Search Time (per turn) Distributed workload vs. Non-distributed workload')
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Search Time (Turn) in ms (Log Scale)')
    plt.xticks(x, custom_labels, rotation=0)
    plt.legend()
    for i, value1, value2 in zip(x, grouped1, grouped2):
        plt.text(i - width/2, value1 + 0.4, str(round(value1, 2)) + "ms", ha='center')
        plt.text(i + width/2, value2 + 0.4, str(round(value2, 2)) + "ms", ha='center')
    plt.show()


def compiler_comparison(csv):
    df = pd.read_csv(csv)
    df_filtered = df[df['threads'].isin([1, 2])]
    grouped = df_filtered.groupby('threads')['average search time (turn)'].mean()
    custom_labels = ['No optimization', '- O3']

    plt.figure(figsize=(8, 6))
    grouped.plot(kind='bar', color=['orange', 'grey'], alpha=0.7)
    plt.title('Average Search Time (Turn)')
    plt.xlabel('Compiler Optimization')
    plt.ylabel('Average Search Time (Turn) in ms')
    plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)
    plt.legend(['No optimization', '- O3'])
    for i, value1 in zip(range(len(grouped)), grouped):
        plt.text(i, value1 + 0.4, str(round(value1, 2)) + "ms", ha='center')
    plt.show()
    df_filtered = df[df['threads'].isin([1, 2])]
    plt.figure(figsize=(8, 6))
    sns.boxplot(x='threads', y='average search time (turn)', data=df_filtered)
    plt.title('Box Plot of Average Search Time (Turn)')
    plt.xlabel('Compiler Optimization')
    plt.ylabel('Average Search Time (Turn) in ms')
    plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)
    plt.legend(['No optimization', '- O3'])
    plt.show()

def bar_plot_log(csv):
    df = pd.read_csv(csv)
    grouped = df.groupby('threads')['average search time (turn)'].mean()
    custom_labels = ['1', '2', '3', '4']

    plt.figure(figsize=(8, 6))
    ax = sns.barplot(x=grouped.index, y=grouped.values, alpha=0.7)

    plt.yscale('log')  # Set the y-axis to logarithmic scale

    plt.title('Average Search Time (per turn)')
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Search Time (Turn) in ms (Log Scale)')
    plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)

    for index, value in enumerate(grouped):
        plt.text(index, value + 0.4, str(round(value, 2)) + "ms", ha='center')
    plt.show()

def bar_plot_openacc_log(csv):
    df = pd.read_csv(csv)
    grouped = df.groupby('threads')['average search time (turn)'].mean()
    custom_labels = ['No OpenACC', 'With OpenACC']

    plt.figure(figsize=(8, 6))
    ax = sns.barplot(x=grouped.index, y=grouped.values, alpha=0.7)

    plt.yscale('log')  # Set the y-axis to logarithmic scale

    plt.title('Average Search Time (per turn)')
    plt.ylabel('Average Search Time (Turn) in ms (Log Scale)')
    plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)

    for index, value in enumerate(grouped):
        plt.text(index, value + 0.08, str(round(value, 2)) + "ms", ha='center')
    plt.show()

bar_plot_openacc_log('../benchmark.csv')