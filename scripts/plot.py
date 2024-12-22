import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import scipy.stats

data = [36.8439,
37.442,
37.4747,
38.6009,
38.7319,
39.2178,
39.2311,
40.3399]

dd = list(map(lambda x: x*0.3858, data))
X = list(range(8))
df = pd.DataFrame({'X': X, 'data': data})
p = sns.regplot(data=df, x='X', y='data')
p.set_ylabel("Procedure Call Overhead (cycles)")
slope, intercept, r, p, sterr = scipy.stats.linregress(x=p.get_lines()[0].get_xdata(),
                                                       y=p.get_lines()[0].get_ydata())

#add regression equation to plot
plt.text(1, 40, 'y = ' + str(round(intercept,3)) + ' + ' + str(round(slope,3)) + 'x')

plt.show()