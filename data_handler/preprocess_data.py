import csv
import json
import ast
import unidecode
import re

niteroi_gyms = {}

UNIQUE_ID = 0

def format_name(name):
    name = unidecode.unidecode(name)
    name = name.lower()
    name = re.sub('-.*', '', name)
    name = re.sub('"', '', name)
    name = re.sub(',', '', name)
    name = re.sub(' ', '-', name)
    
    return name

if __name__ == "__main__":
    
    # icarai json
    with open('data/icarai-gym_loc.json') as f:
        for i in json.load(f):
            d = ast.literal_eval(i)
            niteroi_gyms[d['name']] = {
                'id': UNIQUE_ID,
                'name': d['name'],
                'latitude': d['latitude'],
                'longitude': d['longitude'],
                'neighbor': 'icarai'
            }
            UNIQUE_ID += 1

    # centro + inga csv
    with open('data/centro+inga-gym_loc.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            row = list(filter(lambda x: (x != '' and x != 'Ex'), row))
            if(format_name(name=row[4]) == 'icarai'):
                continue
            gym_name = format_name(name=row[0])
            niteroi_gyms[gym_name] = {
                'id': UNIQUE_ID,
                'name': gym_name,
                'latitude': float(row[1]),
                'longitude': float(row[2]),
                'neighbor': format_name(name=row[4])
            }
            UNIQUE_ID += 1

    with open('data/niteroi_gyms.json', 'w') as f:
        json.dump(niteroi_gyms, f)

