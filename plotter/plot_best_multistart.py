import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
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


fig, ax = plt.subplots()

colors = ['#730000', '#997373', '#332a26', '#7f5940', '#f2ceb6', '#cc8533', '#4c4113', '#fff240', '#8c8846', '#224d13', '#00d957', '#7ca68d', '#36d9ce', '#4d8099', '#3d9df2', '#0d1c33', '#3f1d73', '#ad99cc', '#b63df2', '#d96cb5', '#d90057', '#40101d', '#ff0022']
for i,f in enumerate(os.listdir('data/random_multistart_small')):
    rd_data, rd_best = read_file('data/random_multistart_small/' + f)
    ax.bar(i, rd_best[0], 0.75, color=colors[i])
    print(rd_best)

ax.set_xlabel('Initial Vertex')
ax.set_ylabel('Solution Value')
plt.xticks(np.arange(0, 23, 1))

plt.savefig('plots/random-multistart-best.pdf', bbox_inches='tight')
plt.savefig('plots/random-multistart-best.png', bbox_inches='tight')

plt.show()