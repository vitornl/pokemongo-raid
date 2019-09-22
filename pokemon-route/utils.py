import re

class Converter(object):

    @staticmethod
    def convert_duration(str_duration):
        """
        returns duration in minutes
        """
        data = str_duration.split(' ')
        
        if(len(data) == 4): #h and min
            duration = int(data[0])*60
            duration += int(data[2])
        elif(len(data) == 2): #min
            duration = int(data[0])
        else: raise(ValueError)
        
        return duration

    @staticmethod
    def convert_distance(str_distance):
        """
        returns duration in minutes
        """
        data = str_distance.split(' ')

        if(data[1] == "km"):
            data = data[0].split(',')
            if(len(data) == 2): #x.y km
                distance = int(data[0])*1000
                distance += int(data[1])*100
            elif(len(data) == 1): #x km
                distance = int(data[0])*1000
            else: raise(ValueError)
        elif(data[1] == "m"):
            distance = int(data[0])
        else: raise(ValueError)

        return distance
