import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os

def read_file(f):
    best_result = (-1, -1, [])
    ponctuations = []
    with open(f, 'r') as f:
        for line in f.readlines():
            line = line.split(',')

            #ponct = float(line[0])
            ponct = int(len(line[2:]))
            ponctuations.append(ponct)
            
            if(ponct > best_result[0]):
                best_result = (ponct, float(line[1]), [int(i)+1 for i in line[2:]])
    
    return ponctuations, best_result

def build_histogram(raw_data):
    histogram = []
    data = sorted(raw_data)
    for i in set(data):
        histogram.append((i, data.count(i)))

    return histogram


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

colors = ['#730000', '#997373', '#332a26', '#7f5940', '#f2ceb6', '#cc8533', '#4c4113', '#fff240', '#8c8846', '#224d13', '#00d957', '#7ca68d', '#36d9ce', '#4d8099', '#3d9df2', '#0d1c33', '#3f1d73', '#ad99cc', '#b63df2', '#d96cb5', '#d90057', '#40101d', '#ff0022']
for i,f in enumerate(os.listdir('data/random_multistart_small')):
    rd_data, rd_best = read_file('data/random_multistart_small/' + f)
    
    rd_hist = build_histogram(rd_data)
    
    for e in rd_hist:
        ax.scatter([e[0] for k in range(e[1])], [(i+1) for k in range(e[1])], [k for k in range(e[1])], marker='_', color=colors[i])
        ax.scatter(e[0], (i+1), e[1], marker='.', color=colors[i])

ax.set_xlabel('Solution Value')
ax.set_ylabel('Initial Vertex')
ax.set_zlabel('Occurrence over 10000 trials')

plt.savefig('test.pdf', bbox_inches='tight')
plt.savefig('test.png', bbox_inches='tight')

plt.show()