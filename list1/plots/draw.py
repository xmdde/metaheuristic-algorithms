import networkx as nx
import matplotlib.pyplot as plt
import json
import pandas as pd

filenames = ["xqf131", "xqg237", "pma343", "pka379", "pbn423", "pbm436", "xql662"] #["bcl380", "pbk411", "pbl395"]

for name in filenames:
    with open(f"../output/graph-{name}.json", 'r') as file:
        data = json.load(file)

    coords = {}
    G = nx.Graph()

    for item in data:
        coords[item["id"]] = (item["x"], item["y"])

    mst = pd.read_csv(f"../output/mst-{name}.txt", sep=";")
    id1 = mst.id1
    id2 = mst.id2

    for i in range (0, len(id1)):
        G.add_edge(id1[i], id2[i])

    nx.draw(G, coords, with_labels=False, node_size=15, node_color='blue')
    plt.savefig(f"mst-{name}.png")
    plt.clf()

    G2 = nx.Graph()
    dfs = pd.read_csv(f"../output/dfs-{name}.txt", sep=";")
    v = dfs.id

    for i in range (1, len(v)):
        G2.add_edge(v[i-1], v[i])
    G2.add_edge(v[len(v)-1], 1)
    
    nx.draw(G2, coords, with_labels=False, node_size=15, node_color='red')
    plt.savefig(f"dfs-{name}.png")
    plt.clf()