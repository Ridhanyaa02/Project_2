#code
```.py
#Checklist
#1  ADD final emd time to final check

#IDS-------------------------------------------------------------------------------------------
#owner id = 53
#dht_temp_id = 177
#dht_hum_id = 178
#bmp_temp_id = 179
#bmp_pres_id = 181

import time
from datetime import datetime, timedelta

#Server api-----------------------------------------------------------------------------------
import requests
server_ip = '192.168.4.137'

user = {'username':'shorn&ridi', 'password':'I_told_you_so'}
#comment out after first run
#r = requests.post(f'http://{server_ip}/register', json=user)
#print(r.json())

r = requests.post(f'http://{server_ip}/login',json=user)
access_token = r.json()['access_token']
print(access_token)

auth = {"Authorization":f"Bearer {access_token}"}

new_dht_temp_sensor = {"type": 'Temperature',
              'location': 'SHORNs house',
              'name': 'Shorn sensor1',
              'unit': 'C'}
#comment out after first run
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_dht_temp_sensor, headers=auth)
#print(r.json())

new_dht_hum_sensor = {"type": 'Humidity',
              'location': 'SHORNs house',
              'name': 'Shorn sensor2',
              'unit': '%'}
#comment out after first run
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_dht_hum_sensor, headers=auth)
#print(r.json())

new_bmp_temp_sensor = {"type": 'Temperature',
              'location': 'SHORNs house',
              'name': 'Shorn sensor3',
              'unit': 'C'}
#comment out
#r =  requests.post(f'http://{server_ip}/sensor/new', json=new_bmp_temp_sensor, headers=auth)
#print(r.json())

new_bmp_pres_sensor = {"type": 'Pressure',
              'location': 'SHORNs house',
              'name': 'Shorn sensor4.true',
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
time.sleep(2)  # Trying for the program to not break on me again
print ('Connection Successful')

#ADD to final check
end_time = datetime.now() + timedelta(hours=24)
#Temporary end time
end_time_test = datetime.now() + timedelta(minutes=5)

#it = util.Iterator(my_arduino)
#it = pyfirmata.util.Iterator(my_arduino)
#it.start()

#FUNCTIONS-----------------------------------------------------------------------------

#Log data from sensors
def log_data(file, dht_temp, dht_hum, bmp_temp, bmp_press):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    data.write(f"{timestamp}, {dht_temp:.2f}, {dht_hum:.2f}, {bmp_temp:.2f}, {bmp_press:.2f}\n")

#Taking data from the bmp
#def read_bmp280():
    #temperature
    #my_arduino.i2c_write(BMP280_ADDRESS, [0xFA])  # TEMP_DATA start address
    #temp_data = my_arduino.i2c_read(BMP280_ADDRESS, 3)
    #raw_temp = (temp_data[0] << 12) | (temp_data[1] << 4) | (temp_data[2] >> 4)

    #pressure
    #my_arduino.i2c_write(BMP280_ADDRESS, [0xF7])  # PRESSURE_DATA start address
    #pressure_data = my_arduino.i2c_read(BMP280_ADDRESS, 3)
    #raw_pres = (pressure_data[0] << 12) | (pressure_data[1] << 4) | (pressure_data[2] >> 4)

    #(replace with calibration formulas for accuracy)
    #temperature = raw_temp / 5120.0
    #pressure = raw_pressure / 25600.0

    #return raw_temp, raw_pres

#Taking data from DHT via the serial
#def read_dht11():
    #while arduino_serial.in_waiting > 0:
        #line = arduino_serial.readline().decode('utf-8').strip()
        #if "Temperature" in line and "Humidity" in line:
            # Parse the temperature and humidity from the Arduino output
            #parts = line.split(",")
            #temp = float(parts[0].split(":")[1].strip()[:-2])
            #hum = float(parts[1].split(":")[1].strip()[:-1])
            #return temp, hum
    #return None, None

#DHT SENSOR SECTION--------------------------------------------------------------------

#initial
#DHT = my_arduino.digital[8]
#DHT.mode = 1

#BMP SENSOR SECTION--------------------------------------------------------------------

#BMP address
#BMP280_ADDRESS = 0x76
#my_arduino.i2c_config(delay=0)

#Configure BMP
#my_arduino.i2c_write(BMP280_ADDRESS, [0xF4, 0x27])  #Normal mode, temp/pressure
#my_arduino.i2c_write(BMP280_ADDRESS, [0xF5, 0xA0])  #Standby time = 1000ms, filter = x4

#MAIN----------------------------------------------------------------------------------

#switch to colleted data 2 after the first run
with open("Collected_dat.csv", "w") as data:
    #header
    data.write("Timestamp, DHT11 Temp (°C), DHT11 Humidity (%), BMP280 Temp (°C), BMP280 Pressure (hPa)\n")
    #collecting data and printing for monitoring
    print("Starting data collection for 24 hours...")

    while datetime.now() < end_time_test:
        #bmp_temp, bmp_pres = read_bmp280()
        #dht_temp, dht_hum = read_dht11()

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
        try:
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
        finally:
            time.sleep(60)

        #if dht_temp is not None and dht_hum is not None:
            #log_data(data, dht_temp, dht_hum, bmp_temp, bmp_pres)
            #print(f"Logged: DHT-> Temp: {dht_temp:.2f} °C, Hum: {dht_hum:.2f} % | "
                  #f"BMP-> Temp: {bmp_temp:.2f} °C, Pres: {bmp_pres:.2f} hPa")
            #time.sleep(60)

arduino_serial.close()
print("Collection is complete Master, TIME IS UP!")
```
