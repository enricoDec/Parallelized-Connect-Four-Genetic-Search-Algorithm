import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv('../benchmark.csv')
df_filtered = df[df['threads'].isin([1, 2])]
grouped = df_filtered.groupby('threads')['average search time (turn)'].mean()
custom_labels = ['No optimization', '- O3']

plt.figure(figsize=(8, 6))
grouped.plot(kind='bar', color=['orange', 'grey'], alpha=0.7)
plt.title('Average Search Time (Turn)')
plt.xlabel('Compiler Optimization')
plt.ylabel('Average Search Time (Turn)')
plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)
plt.legend(['No optimization', '- O3'])
plt.show()

df_filtered = df[df['threads'].isin([1, 2])]

plt.figure(figsize=(8, 6))
sns.boxplot(x='threads', y='average search time (turn)', data=df_filtered)
plt.title('Box Plot of Average Search Time (Turn)')
plt.xlabel('Compiler Optimization')
plt.ylabel('Average Search Time (Turn)')
plt.xticks(range(len(custom_labels)), custom_labels, rotation=0)
plt.legend(['No optimization', '- O3'])
plt.show()
