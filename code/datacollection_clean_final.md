#code 
```.py
import time
from datetime import datetime, timedelta
Bold_red = "\33[1;31m"
end_code = "\033[00m"
failure_count = 0
#Server api-----------------------------------------------------------------------------------

import requests
server_ip = '192.168.4.137'
user = {'username':'shorn&ridi', 'password':'I_told_you_so'}
r = requests.post(f'http://{server_ip}/login',json=user)
access_token = r.json()['access_token']
print(access_token)
auth = {"Authorization":f"Bearer {access_token}"}

new_dht_temp_sensor = {"type": 'Temperature',
              'location': 'RIDI house',
              'name': 'DHT temp sensor1.2',
              'unit': 'C'}
#comment out after first run
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_dht_temp_sensor, headers=auth)
#print(r.json())

new_dht_hum_sensor = {"type": 'Humidity',
              'location': 'RIDI house',
              'name': 'DHT hum sensor2.2',
              'unit': '%'}
#comment out after first run
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_dht_hum_sensor, headers=auth)
#print(r.json())

new_bmp_temp_sensor = {"type": 'Temperature',
              'location': 'RIDI house',
              'name': 'BMP temp sensor3.2',
              'unit': 'C'}
#comment out
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_bmp_temp_sensor, headers=auth)
#print(r.json())

new_bmp_pres_sensor = {"type": 'Pressure',
              'location': 'RIDI house',
              'name': 'BMP press sensor4.2',
              'unit': 'hpa'}
#comment out after first run
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_bmp_pres_sensor, headers=auth)
#print(r.json())

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

end_time = datetime.now() + timedelta(minutes=623)

#FUNCTIONS----------------------------------------------------------------------------------------------

def log_data(file, dht_temp, dht_hum, bmp_temp, bmp_press):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    data_file.write(f"{timestamp}, {dht_temp:.2f}, {dht_hum:.2f}, {bmp_temp:.2f}, {bmp_press:.2f}\n")

def check_connection(host=server_ip, port=80, timeout=3):
    try:
        socket.setdefaulttimeout(timeout)
        socket.create_connection((host, port))
        return True
    except OSError:
        return False

#MAIN---------------------------------------------------------------------------------------------------

#switch to colleted data 2 after the first run
with open("Collected _dat2.csv", "w") as data_file:
    #header
    data_file.write("Timestamp, DHT11 Temp (°C), DHT11 Humidity (%), BMP280 Temp (°C), BMP280 Pressure (hPa)\n")
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
            log_data(data_file, dht_temp, dht_hum, bmp_temp, bmp_pres)
            print(f"Logged: {time_stamp}, {dht_temp} °C, {dht_hum} %, {bmp_temp} °C, {bmp_pres} hPa")
        else:
            print("Where is my data?")

#Server Upload--------------------------------------------------------------------------------------------------
        dht_temp_id =245
        dht_hum_id =246
        bmp_temp_id =247
        bmp_pres_id =248

        import socket

        if check_connection():
            try:
                r = requests.post(f'http://{server_ip}/login', json=user)
                access_token = r.json()['access_token']
                auth = {"Authorization": f"Bearer {access_token}"}
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
            except Exception as e:
                print(f'{Bold_red}Connection ERROR{end_code}')
                failure_count = failure_count + 1
                print(f'Failure #{failure_count}')
            time.sleep(60)
        else:
            try:
                r = requests.post(f'http://{server_ip}/login', json=user)
                access_token = r.json()['access_token']
                auth = {"Authorization": f"Bearer {access_token}"}
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
            except Exception as e:
                print(f'{Bold_red}Connection ERROR{end_code}')
                print("No server connection saving locally")
                failure_count = failure_count + 1
                print(f'Failure #{failure_count}')
            time.sleep(60)
    else:
        print("No server connection saving locally")
        time.sleep(60)

arduino_serial.close()
print("Collection is complete Master, TIME IS UP!")
print(f'The upload failed {failure_count} times')
if failure_count > 0:
    print('You should do something about that.')
```
