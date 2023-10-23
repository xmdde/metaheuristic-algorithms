import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv(f"../output/data.txt", sep=";")
y = ["MST weight", "MST dfs weight", "min_1000", "avg_min_50", "avg_min_10"]

plt.plot(y, data.bcl380, color='royalblue', label='bcl380')
plt.plot(y, data.pbk411, color='hotpink', label='pbk411')
plt.plot(y, data.pbl395, color='green', label='pbl395')
plt.plot(y, data.pbm436, color='gold', label='pbm436')
plt.plot(y, data.pbn423, color='purple', label='pbn423')
plt.plot(y, data.pka379, color='darkblue', label='pka379')
plt.plot(y, data.pma343, color='cyan', label='pma343')
plt.plot(y, data.xqf131, color='lawngreen', label='xqf131')
plt.plot(y, data.xqg237, color='orange', label='xqg237')
plt.plot(y, data.xql662, color='grey', label='xql662')

plt.legend(loc='upper left')
plt.show()