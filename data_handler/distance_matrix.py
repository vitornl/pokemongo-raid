import json
import requests

def build_batch(data, element, batch_size):
    out = []
    batch = []
    for d in data.keys():
        batch.append(d)
        if(len(batch)%batch_size == 0):
            out.append(batch)
            batch = []
    
    if(len(batch) > 0):
        out.append(batch)
    
    return out

def build_url(data, origin, batch):
    s = 'origins='
    s += str(data[origin]['latitude']) + ','
    s += str(data[origin]['longitude']) + '&'
    s += 'destinations='
    for b in batch:
        s += str(data[b]['latitude']) + ','
        s += str(data[b]['longitude']) + '|'
    
    return s[:-1]

def main(API_KEY):
    with open('data/niteroi_gyms.json', 'r') as f:
        data = json.load(f)

    with open('data/d_space.lhp', 'w') as f_space, open('data/d_time.lhp', 'w') as f_time:
        f_space.write('D_SPACE.LHP\n')
        f_time.write('D_TIME.LHP\n')
        for d in data.keys():
            print(d)
            line_space = ''
            line_time = ''
            for batch in build_batch(data, d, 25):
                url = API_BASIC_URL + '&' + build_url(data, d, batch) + '&key=' + API_KEY
                response = requests.get(url=url).json()
                if(response['status'] != 'OK'):
                    print(url)
                    exit()
                for element in response['rows'][0]['elements']:
                    line_space += str(element['distance']['value']) + ' '
                    line_time += str(element['duration']['value']) + ' '
            line_space = line_space[:-1] + '\n'
            line_time = line_time[:-1] + '\n'
            f_space.write(line_space)
            f_time.write(line_time)

if __name__ == "__main__":
    with open('.api_key', 'r') as f:
        API_KEY = f.readline()

    API_BASIC_URL = 'https://maps.googleapis.com/maps/api/distancematrix/json?mode=walking'
    main(API_KEY)