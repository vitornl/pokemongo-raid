import json
import ast
import numpy as np

D = np.load('data/distance_mat.npy')
T = np.load('data/duration_mat.npy')

with open('data/icarai-gym_loc.json') as f:
    L = json.load(f)
    L = [ast.literal_eval(i) for i in L]

with open('data/vertices.lhp', 'w') as f:
    f.write('VERTICES.LHP\n')
    f.write(str(len(L)) + '\n')
    s = 'ID '
    for k in L[0].keys():
        s += k.upper() + ' '
    f.write(s + '\n')
    for i,e in enumerate(L):
        s = str(i) + ' '
        for v in e.values():
            s += str(v) + ' '
        f.write(s + '\n')

with open('data/d_time.lhp', 'w') as f:
    f.write('D_TIME.LHP\n')
    for v in T:
        for i in v:
            f.write(str(int(i)) + ' ')
        f.write('\n')

with open('data/d_space.lhp', 'w') as f:
    f.write('D_SPACE.LHP\n')
    for v in D:
        for i in v:
            f.write(str(int(i)) + ' ')
        f.write('\n')
