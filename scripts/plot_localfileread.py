import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt


X = [128, 256, 512, 1024, 2048, 4096]*2
Y = [42.0032, 70.1372, 126.142, 236.022, 469.534, 874.808, 81.3321, 141.193, 283.283, 556.136, 1101.99, 1574.94]
T = ['Seq']*6 + ['Random']*6
X = np.log2(X)
Y = np.log2(Y)
data = pd.DataFrame(data = {'FileSize': X, 'Access_Time': Y, 'Access_Type': T})

seq_estimate = np.log2([40.119*(2**i) for i in range(6)])
random_estimate = np.log2([70.096*(2**i) for i in range(6)])

sns.set_style("darkgrid")
p = sns.lineplot(data = data, x='FileSize', y='Access_Time', hue='Access_Type', marker='o')
p.plot(X[:6], seq_estimate, linestyle='--', color='blue')
p.plot(X[:6], random_estimate, linestyle='--', color='orange')
p.set_xticks(data['FileSize'])
p.set_xlabel("log2(file size) (MB)")
p.set_ylabel("log2(access time) (ms)")
plt.show()