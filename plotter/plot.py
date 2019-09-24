import matplotlib.pyplot as plt
import numpy as np

best_result = (-1, -1, [])
ponctuations = []
with open('data/random-multistart.txt', 'r') as f:
    for line in f.readlines():
        line = line.split(',')

        ponct = float(line[0])
        ponctuations.append(ponct)
        
        if(ponct > best_result[0]):
            best_result = (ponct, float(line[1]), [int(i) for i in line[2:]])

bins = sorted([int(i) for i in ponctuations])

n = len(ponctuations)

for i in range(n):
    ponctuations.append(21.6129)

fig, ax = plt.subplots()

# plt.xlim([0, int(max(ponctuations))+1])
plt.xticks([i for i in range(0, 22)])

counts, bins = np.histogram(ponctuations)
plt.hist(bins[:-1], bins, weights=counts, normed=1, rwidth=0.25)

# N, bins, patches = ax.hist(ponctuations, bins=50, normed=1, rwidth=0.25)#, bins=bins)
plt.title('Random Gaussian data (fixed bin size)')
plt.xlabel('variable X (bin size = 5)')
plt.ylabel('count')

plt.show()

print(best_result)