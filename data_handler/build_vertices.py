import json

with open('data/niteroi_gyms.json', 'r') as f:
    data = json.load(f)

with open('data/vertices_niteroi.lhp', 'w') as f:
    f.write('VERTICES_NITEROI.LHP\n')
    f.write(str(len(data)) + '\n')
    f.write('ID NAME NEIGHBOR LATITUDE LONGITUDE\n')
    for d in data.keys():
        s = ''
        s += str(data[d]['id']) + ' '
        s += str(data[d]['name']) + ' '
        s += str(data[d]['neighbor']) + ' '
        s += str(data[d]['latitude']) + ' '
        s += str(data[d]['longitude']) + '\n'
        f.write(s)
