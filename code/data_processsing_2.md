```.py
from matplotlib import pyplot as plt
import matplotlib
import requests
from matplotlib.gridspec import GridSpec
import numpy as np


def moving_average(windowSize:int, x:list)->list:
    x_smooth = []
    for i in range(0, len(x)-windowSize):
        x_section = x[i:i+windowSize]
        x_average = sum(x_section)/windowSize
        x_smooth.append(x_average)
    return x_smooth

def model(x,a,b,c,d):
    return a*x**3 + b*x**2 + c*x + d

plt.style.use('ggplot')
matplotlib.use('macosx')

server_ip = '192.168.4.137'

request = requests.get(f'http://{server_ip}/readings')
data = request.json() #java object (dictionary)

readings = data['readings'][0]

ids = []
for r in readings:
    if r["sensor_id"] not in ids:
        ids.append(r["sensor_id"])

#create a dictionary to contain the data from the sensors
y = {}
for id in ids:
    y[id] = []

for r in readings:
    y[r["sensor_id"]].append((r["value"]))

#create visualisation
fig = plt.figure(figsize=(10,8))
grid = GridSpec(nrows=3, ncols=4, figure=fig)
plt.subplots_adjust(hspace=0.36)#give space between the graphs
plt.subplots_adjust(wspace=0.36)
plt.subplots_adjust(left=0.06)
plt.subplots_adjust(right=0.98)
plt.subplots_adjust(top=0.95)
plt.subplots_adjust(bottom=0.065)

#Calcualate avg temp
TT = []
for values in y[245]:
    TT.append(values)

TTT = []
for values in y[247]:
    TTT.append(values)

SUM = []
for i in range(0,len(TT)):
    total = TT[i]+TTT[i]
    SUM.append(total)

AVG = []
for i in range(0,len(TT)):
    avg = SUM[i]/2
    AVG.append(avg)

#First data set==================================================================================

#temperature-------------------------------------------------------------------------------

#dht temperature
box1 = fig.add_subplot(grid[0,0])
plt.plot(y[245], color ='salmon',label='DHT temp')
plt.ylabel("RAW Temperature (C)")
plt.xlabel('Time(minutes)')
#Bme Temperature
plt.plot(y[247], color ='cornflowerblue', label='BME temp')
#Average temperature
plt.plot(AVG, color = 'mediumpurple', label='Average temp')
plt.legend(loc=1)

#Dht
box2 = fig.add_subplot(grid[0:3,1:3])
temp_smooth = moving_average(windowSize=10, x=y[245])
plt.plot(temp_smooth, color='salmon', label='DHT temp')
plt.ylabel('Temperatures (C)')
#Bme
temp_smooth = moving_average(windowSize=10, x=y[247])
plt.plot(temp_smooth, color='cornflowerblue', label='BME temp')
#avg
temp_smooth = moving_average(windowSize=10, x=AVG)
plt.plot(temp_smooth, color='mediumpurple', label='Avg temp')
plt.legend(loc=1)

#DHT humidity--------------------------------------------------------------------------------

box3 =fig.add_subplot(grid[1,0])
plt.plot(y[246], color ='lightcyan')

hum_smooth = moving_average(windowSize=10, x=y[246])
plt.plot(hum_smooth, color="blue")
plt.ylabel('Humidity (%)')
plt.xlabel('Time(minutes)')

#Bme Pressure---------------------------------------------------------------------------------

box4= fig.add_subplot(grid[2,0])
plt.plot(y[248], color ='Thistle')

pres_smooth = moving_average(windowSize=10, x=y[248])
plt.plot(pres_smooth, color='darkviolet')
plt.ylabel('Pressure(KPa)')
plt.xlabel('Time(minutes)')

#Temp Model-----------------------------------------------------------------------------------

x = []
for i in range (0,len(AVG)):
    x.append(i)

a,b,c,d = np.polyfit(x,AVG,3)

y_model = []
for i in range(0,len(AVG)):
    y_model.append(model(x[i],a,b,c,d))
box7 = fig.add_subplot(grid[2,3])
plt.plot(y_model,color='green')
plt.ylabel('temp data model')

plt.show()

#Second data set==================================================================================



#Legend---------------------------------------------------------------------------------------

#shorn room
#print(y[211])#temp
#print(y[212])#hum
#print(y[213])#temp
#print(y[214])#pres

#other room
#print(y[245])
#print(y[246])
#print(y[247])
#print(y[248])
```
