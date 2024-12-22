import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt


X = [16, 32, 64, 128, 256, 512, 1024]*2
Y = [5119.54, 9671.06, 19278.3, 39444.5, 79349.7, 158455, 357772, 5623.59, 11272, 22639, 45894.6, 76952.5, 162348, 333355.55]
T = ['Seq']*7 + ['Random']*7
X = np.log2(X)
Y = np.log2(np.array(Y)/1000)
data = pd.DataFrame(data = {'FileSize': X, 'Access_Time': Y, 'Access_Type': T})

seq_estimate = np.log2([5.866*(2**i) for i in range(7)])
random_estimate = np.log2([6.67125*(2**i) for i in range(7)])

sns.set_style("darkgrid")
p = sns.lineplot(data = data, x='FileSize', y='Access_Time', hue='Access_Type', marker='o')
p.plot(X[:7], seq_estimate, linestyle='--', color='blue')
p.plot(X[:7], random_estimate, linestyle='--', color='orange')
p.set_xticks(data['FileSize'])
p.set_xlabel("log2(file size) (MB)")
p.set_ylabel("log2(access time) (s)")
plt.show()