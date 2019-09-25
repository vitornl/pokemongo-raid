import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import numpy as np

def read_file(f):
    best_result = (-1, -1, [])
    ponctuations = []
    with open(f, 'r') as f:
        for line in f.readlines():
            line = line.split(',')

            ponct = float(line[0])
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

rd_data, rd_best = read_file('data/random-multistart_niteroi.txt')
gr_data, gr_best = read_file('data/greedy_niteroi.txt')
ag_data, ag_best = read_file('data/adaptive-greedy_niteroi.txt')

rd_hist = build_histogram(rd_data)
gr_hist = build_histogram(gr_data)
ag_hist = build_histogram(ag_data)

fig, ax = plt.subplots()
# ax.axvline(21.6129, color='k', alpha=0.5)
# plt.bar(21.6129, 1699, 0.1, color='k')

for rd_b, gr_b, ag_b, in zip(rd_hist, gr_hist, ag_hist):
    if(rd_b[0] == gr_b[0] and rd_b[0] == ag_b[0]):
        if(rd_b[1] > gr_b[1] and rd_b > ag_b[1]):
            ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
            if(gr_b[1] > ag_b[1]):
                ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
                ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
            else:
                ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
                ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
        elif(rd_b[1] > gr_b[1] and rd_b[1] < ag_b[1]):
            ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
            ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
            ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
        elif(gr_b[1] < ag_b[1]):
            ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
            ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
            ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
        else:
            ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
            ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
            ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
    
    elif(gr_b[0] == ag_b[0]):
        ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
        if(gr_b[1] > ag_b[1]):
            ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
            ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
        else:
            ax.bar(ag_b[0], ag_b[1], 0.1, color='g')
            ax.bar(gr_b[0], gr_b[1], 0.1, color='r')

    else:
        ax.bar(rd_b[0], rd_b[1], 0.1, color='b')
        ax.bar(gr_b[0], gr_b[1], 0.1, color='r')
        ax.bar(ag_b[0], ag_b[1], 0.1, color='g')

ax.set_xlabel('Solution Value')
ax.set_ylabel('Occurrence over 10000 trials')
# plt.xticks(np.arange(0, 23, 1))        
# plt.yticks(np.arange(0, 1800, 100))

blue_patch = mpatches.Patch(color='blue', label='Random heuristic')
red_patch = mpatches.Patch(color='red', label='Greedy heuristic')
green_patch = mpatches.Patch(color='green', label='Adaptive greedy heuristic')
gray_patch = mpatches.Patch(color='gray', label='Best known solution value')
plt.legend(handles=[blue_patch, red_patch, green_patch, gray_patch])

print(rd_best)
print(gr_best)
print(ag_best)

plt.savefig("executions_niteroi.pdf", bbox_inches='tight')

plt.show()