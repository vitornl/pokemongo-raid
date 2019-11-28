IN = 'ga.txt'
OUT = 'ga.dat'

with open(IN, 'r') as f, open(OUT, 'w') as g:
    f.readline()
    for line in f.readlines():
        sol = int(line.split(',')[1])
        if sol >= 13:
            g.write(line.split(',')[2])
        else:
            print(line)