import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt


X = [128, 256, 512, 1024, 2048, 4096, 8192, 16384]
Y = [16.593, 34.726, 67.208, 131.502, 252.34, 484.61, 936.426, 5052.11]
X = np.log2(X)
Y = np.log2(Y)
data = pd.DataFrame(data = {'FileSize': X, 'Access_Time': Y})

sns.set_style("darkgrid")
p = sns.lineplot(data = data, x='FileSize', y='Access_Time', marker='o')
p.set_xticks(data['FileSize'])
p.set_xlabel("log2(file size) (MB)")
p.set_ylabel("log2(access time) (ms)")
plt.show()