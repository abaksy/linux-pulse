import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

BASE_PATH = "../results"
data_files = list(filter(lambda x: x.startswith("memaccess"), os.listdir(BASE_PATH)))
stride_lengths = list(map(lambda x: x.split("_")[-1], data_files))
hue_order = list(map(str, sorted(map(int, stride_lengths))))
all_data = list()

for file, stride in zip(data_files, stride_lengths):
    with open(f"{BASE_PATH}/{file}") as f:
        lines = f.readlines()
        lines = lines[1:]
        data_lines = list(map(lambda x: x.strip().split(","), lines))
        data_lines = [[stride] + list(map(float, x)) for x in data_lines]
        all_data.extend(data_lines)

sns.set_style("darkgrid")
sns.set_palette("Paired", n_colors=7)


df = pd.DataFrame(data = all_data, columns=['stride', 'arr_size', 'access_time'])
df['arr_size'] = np.log2(df['arr_size'])
df_sorted = df.sort_values(by='stride')
p = sns.lineplot(data = df_sorted, x='arr_size', y = 'access_time', hue='stride', marker="o", hue_order=hue_order)
p.set_xticks(df['arr_size'])
p.set_xlabel("log2(arr_size) (KB)")
plt.show()
