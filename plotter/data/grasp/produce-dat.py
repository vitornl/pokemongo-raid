IN = 'output/grasp-0.25.txt'
# OUT = 'output/grasp-0.25.dat'

# with open(IN, 'r') as f, open(OUT, 'w') as g:
#     f.readline()
#     for line in f.readlines():
#         sol = int(line.split(',')[1])
#         if sol >= 13:
#             g.write(line.split(',')[2])
#         else:
#             print(line)

for f in ['0.25', '0.50', '0.75']:
    IN = f'grasp-{f}.dat'
    with open(IN, 'r') as f:
        lines = f.readlines()
        k = 0
        for l in lines:
            k += float(l)

        print(k/len(lines))