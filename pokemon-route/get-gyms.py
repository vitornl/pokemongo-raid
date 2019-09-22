from lxml import html, etree
import requests, re, pickle, json
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium.common.exceptions import TimeoutException

from route import Route, Gym

def get_gyms_url(driver, location_url):
    delay = 15
    driver.get(location_url)

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CLASS_NAME, 'closeIcon')))
        driver.find_element_by_class_name('closeIcon').click()
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)

    driver.find_element_by_class_name('loclisttxt').click()

    tree = html.fromstring(driver.page_source)

    gym = tree.xpath('/html/body/div[3]/div[4]/div[1]/div[2]/div/div[1]/div')[0]

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CSS_SELECTOR, '[id*=listitem]')))
        locations = driver.find_elements_by_css_selector("[id*=listitem]")
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)

    gyms_url = []
    for location in locations:
        if('gym' in location.get_attribute('data-url')):
            gyms_url.append('https://www.pokemongomap.info{}'.format(location.get_attribute('data-url')))

    return gyms_url

def get_gym_location(driver, gym_url):
    delay = 15
    driver.get(gym_url)

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CLASS_NAME, 'closeIcon')))
        driver.find_element_by_class_name('closeIcon').click()
    except TimeoutException:
        pass

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'mapinfobox')))
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)

    driver.find_element_by_css_selector('a.tooltip:nth-child(9)').click()

    try:
        WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CLASS_NAME, 'jconfirm-box')))
    except TimeoutException:
        print("Timed out waiting for page to load")
        exit(1)

    location = driver.find_element_by_css_selector('a.popinfo').get_attribute('href')
    print(location)
    location = re.sub('.*destination=', '', location).split(',')

    return float(location[0]), float(location[1])

def main():
    URL_ICARAI='https://www.pokemongomap.info/location/-22,905266/-43,106692/16'

    driver = webdriver.Firefox()
    
    gyms_url = get_gyms_url(driver, URL_ICARAI)

    print(gyms_url)

    route = Route(name='icarai')
    
    for gym_url in gyms_url:
        gym_name = re.sub('\/.*', '', re.sub('.*gym/', '', gym_url))

        la, lo = get_gym_location(driver, gym_url)
        while (la == 0 and lo == 0):
            print("Location Error {}; ({},{})".format(gym_name, la, lo))
            la, lo = get_gym_location(driver, gym_url)   
        
        
        gym = Gym(gym_name, gym_url, la, lo)
        gym.view()
        
        route.add_gym(gym)

    route.export_gyms()
    driver.quit()

if __name__ == "__main__":
    main()