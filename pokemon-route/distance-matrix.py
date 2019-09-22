import json
import numpy as np
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium.common.exceptions import TimeoutException

from utils import Converter

#URL_BASE = "https://www.google.com/maps/dir/'{}'/'{}'".format()

def retrieve_deslocation(driver, gym1, gym2):
    delay=300
    print("{} x {}".format(json.loads(gym1)['name'], json.loads(gym2)['name']))
    ll_1 = "{},{}".format(str(json.loads(gym1)['latitude']), str(json.loads(gym1)['longitude']))
    ll_2 = "{},{}".format(str(json.loads(gym2)['latitude']), str(json.loads(gym2)['longitude']))
    
    driver.get("https://www.google.com/maps/dir/'{}'/'{}'".format(ll_1,ll_2))

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CSS_SELECTOR, '.directions-walk-icon')))
        driver.find_element_by_css_selector('.directions-walk-icon').click()
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CLASS_NAME, 'section-directions-trip-numbers')))
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)
    duration = driver.find_element_by_class_name('section-directions-trip-duration').text
    distance = driver.find_element_by_class_name('section-directions-trip-distance').text

    return duration, distance

def main():
    with open('data/icarai-gym_loc.json', 'r') as f:
        gyms_loc = json.load(f)

    dist_mat = np.zeros((len(gyms_loc), len(gyms_loc)), np.float_)
    dur_mat = np.zeros((len(gyms_loc), len(gyms_loc)), np.float_)
    np.fill_diagonal(dist_mat, 0)
    np.fill_diagonal(dur_mat, 0)

    for gym1,i in zip(gyms_loc, range(len(gyms_loc)-1)):
        driver = webdriver.Firefox()
        for gym2,j in zip(gyms_loc[i+1:], range(len(gyms_loc[i+1:]))):
            duration, distance = retrieve_deslocation(driver, gym1, gym2)
            try:
                duration = Converter.convert_duration(duration)
            except ValueError:
                print("Could not convert duration")
                exit(1)
            try:
                distance = Converter.convert_distance(distance)
            except ValueError:
                print("Could not convert distance")
                exit(1)
            
            print(duration, distance)
            dur_mat[i][i+1+j] = duration
            dur_mat[i+1+j][i] = duration

            dist_mat[i][i+1+j] = distance
            dist_mat[i+1+j][i] = distance
        driver.quit()

    np.save('data/duration_mat.npy', dur_mat)
    np.save('data/distance_mat.npy', dist_mat)
    print(dur_mat)
    print("---")
    print(dist_mat)

if __name__ == "__main__":
    main()