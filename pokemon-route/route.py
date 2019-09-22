import json

class Route(object):
    def __init__(self, name=None):
        self.name = name
        self.gyms = []
    
    def add_gym(self, gym):
        self.gyms.append(gym)

    def export_gyms(self):
        pass
        def _build_list():
            json_list = []
            for gym in self.gyms:
                json_list.append(json.dumps(gym.__dict__))
            
            return json_list

        with open('{}-gym_loc.json'.format(self.name), 'w') as f:
            json.dump(_build_list(), f)

class Gym(object):
    def __init__(self, name, url, latitude, longitude):
        self.name = name
        self.url = url
        self.latitude = latitude
        self.longitude = longitude

    def view(self):
        print("+ Gym {}:\n\t-URL: {}\n\t-Latitude: {}\n\t-Longitude: {}".format(self.name, self.url, self.latitude, self.longitude))