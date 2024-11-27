#code
```.py
import time
from datetime import datetime, timedelta

#Server api-----------------------------------------------------------------------------------

import requests
server_ip = '192.168.4.137'
user = {'username':'shorn&ridi', 'password':'I_told_you_so'}
r = requests.post(f'http://{server_ip}/login',json=user)
access_token = r.json()['access_token']
auth = {"Authorization":f"Bearer {access_token}"}

#End of API----------------------------------------------------------------------------------------------

#Initialise
import serial
port = '/dev/cu.usbserial-10'
arduino_baud_rate = 9600
serial_timeout = 1

# Open serial connection for DHT11
arduino_serial = serial.Serial(port, arduino_baud_rate, timeout=serial_timeout)
time.sleep(2)
print ('Connection Successful')

end_time = datetime.now() + timedelta(hours=24)

#FUNCTIONS----------------------------------------------------------------------------------------------

def log_data(file, dht_temp, dht_hum, bmp_temp, bmp_press):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    data.write(f"{timestamp}, {dht_temp:.2f}, {dht_hum:.2f}, {bmp_temp:.2f}, {bmp_press:.2f}\n")

#MAIN---------------------------------------------------------------------------------------------------

#switch to colleted data 2 after the first run
with open("Collected_dat.csv", "w") as data:
    #header
    data.write("Timestamp, DHT11 Temp (°C), DHT11 Humidity (%), BMP280 Temp (°C), BMP280 Pressure (hPa)\n")
    #collecting data and printing for monitoring
    print("Starting data collection for 24 hours...")

    while datetime.now() < end_time:
        if arduino_serial.in_waiting > 0:
            line = arduino_serial.readline().decode('utf-8').strip()
            print("Received:", line)

        if "DHT11" in line and "BMP280" in line:
            parts = line.split(",")
            dht_temp = float(parts[0].split(":")[1].strip()[:-2])
            dht_hum = float(parts[1].split(":")[1].strip()[:-1])
            bmp_temp = float(parts[2].split(":")[1].strip()[:-2])
            bmp_pres = float(parts[3].split(":")[1].strip()[:-3])

            time_stamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            log_data(data, dht_temp, dht_hum, bmp_temp, bmp_pres)
            print(f"Logged: {time_stamp}, {dht_temp} °C, {dht_hum} %, {bmp_temp} °C, {bmp_pres} hPa")
        else:
            print("Where is my data?")

#Server Upload--------------------------------------------------------------------------------------------------
        dht_temp_id = 177
        dht_hum_id = 178
        bmp_temp_id = 179
        bmp_pres_id = 181

        data = {'sensor_id': dht_temp_id, 'value': f'{dht_temp:2f}'}
        r = requests.post(f'http://{server_ip}/reading/new', json=data, headers=auth)
        print(r.json())

        data = {'sensor_id': dht_hum_id, 'value': f'{dht_hum:2f}'}
        r = requests.post(f'http://{server_ip}/reading/new', json=data, headers=auth)
        print(r.json())

        data = {'sensor_id': bmp_temp_id, 'value': f'{bmp_temp:2f}'}
        r = requests.post(f'http://{server_ip}/reading/new', json=data, headers=auth)
        print(r.json())

        data = {'sensor_id': bmp_pres_id, 'value': f'{bmp_pres:2f}'}
        r = requests.post(f'http://{server_ip}/reading/new', json=data, headers=auth)
        print(r.json())
        time.sleep(60)

arduino_serial.close()
print("Collection is complete Master, TIME IS UP!")
```
