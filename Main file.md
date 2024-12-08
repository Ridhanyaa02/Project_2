# Criterion A: Planning

## Problem Definition:
### My client is Bread and Bakes, a local student-led business in the ISAK community that makes different kinds of bread which they later sell to the student population. Bread has an optimum humidity and temperature for proofing and baking. This is around 75% humidity levels as well as 22 - 25 degrees celsius for proofing. Due to the decrease of humidity and temperature in Karuizawa during fall, the owners of Bread and Bakes have hired our team to monitor the temperature of the apartment. By measuring the humidity and temperature levels in different houses, R3 and R2, they hope to find the best place to allow their bread to rise, consequently hoping for an increase in business. 

## Proposed Solution:
### Considering the client requirements an adequate solution includes a low cost sensing device for humidity and temperature and a custom data script that processes and analyzes the samples acquired. For a low cost sensing device an adequate alternative is the DHT11 sensor1 which is offered online for less than 5 USD and provides adequate precision and range for the client requirements (Temperature Range: 0°C to 50°C, Humidity Range: 20% to 90%). Similar devices such as the DHT22, AHT20 or the AM2301B 2 have higher specifications, however the DHT11 uses a simple serial communication (SPI) rather than more elaborated protocols such as the I2C used by the alternatives. For the range, precision and accuracy required in this application the DHT11 provides the best compromise. Connecting the DHT11 sensor to a computer requires a device that provides a Serial Port communication. A cheap and often used alternative for prototyping is the Arduino UNO microcontroller 3. "Arduino is an open-source electronics platform based on easy-to-use hardware and software"4. In addition to the low cost of the Arduino (< 6USD), this device is programmable and expandable1. I considered alternatives such as different versions of the original Arduino but their size and price make them a less adequate solution.
### Considering the budgetary constraints of the client and the hardware requirements, the software tool that I proposed for this solution is Python. Python's open-source nature and platform independence contribute to the long-term viability of the system. The use of Python simplifies potential future enhancements or modifications, allowing for seamless scalability without the need for extensive redevelopment 56. In comparison to the alternative C or C++, which share similar features, Python is a High level programming language (HLL) with high abstraction 7. For example, memory management is automatic in Python whereas it is the responsibility of the C/C++ developer to allocate and free up memory 7, this could result in faster applications but also memory problems. In addition, a HLL language will allow me and future developers to extend the solution or solve issues promptly.
### In order to represent our data visually to our client, we will be creating a poster that communicates our findings effectively. 

# Criterion B: Solution Overview

## Test Plan:
| Test Case  | Objective                                  | Input                     | Expected Outcome                                | Estimated Time | Date Completed |
|------------|--------------------------------------------|---------------------------|-------------------------------------------------|----------------|----------------|
| 1          | Validate server login                      | Correct username/password | Access token is received                        | 30 minutes     | Dec 1st        |
| 2          | Verify sensor data collection from Arduino | Sensor data line          | Logged data matches the input                   | 1 hour         | Dec 1st        |
| 3          | Ensure data upload to the server           | Sensor ID and value       | Data upload response confirms success           | 30 minutes     | Dec 2nd        |
| 4          | Handle server disconnection                | Server is unavailable     | Data is saved locally, failure count increments | 3 days         | Dec 5th        |

## Systems Diagram:
<img width="817" alt="Screenshot 2024-12-09 at 0 41 53 AM" src="https://github.com/user-attachments/assets/83c1f726-e108-4a97-9104-48bd5974d1dd">

## Flow Diagrams:
### Server Log In: 
<img width="421" alt="Screenshot 2024-12-09 at 0 53 50 AM" src="https://github.com/user-attachments/assets/ed507c23-655a-4018-a384-249b4234f5b0">

### Sensor Data Processing:
<img width="590" alt="Screenshot 2024-12-09 at 1 01 45 AM" src="https://github.com/user-attachments/assets/f20b310e-5a1c-4c9f-a1cf-f234c6b18c5d">

### Data Upload to Server: 
<img width="584" alt="Screenshot 2024-12-09 at 1 04 23 AM" src="https://github.com/user-attachments/assets/b72fa5f5-6101-4d1b-9d9f-b57df1041309">

## Data Storage Examples:

#### CSV File Format:
```.py
Timestamp, DHT11 Temp (°C), DHT11 Humidity (%), BMP280 Temp (°C), BMP280 Pressure (hPa)
2024-12-08 15:23:45, 22.34, 45.67, 21.78, 1013.25
```
#### JSON Example for Server Upload::
{
  "sensor_id": 245,
  "value": "22.34"
}

## Development Process:

### Purpose:
The project collects environmental data (temperature, humidity, and pressure) using sensors, logs it locally, and uploads it to a server for analysis.

### Development Steps:

#### Setup and Testing:
Configured Arduino and Python serial communication.
Set up server authentication using POST requests.
#### Data Logging:
Captured data from the DHT11 and BMP280 sensors.
Logged data in a CSV file with timestamps.
#### Server Communication:
Implemented API calls for data upload.
Handled server disconnections gracefully by saving data locally.
#### Error Handling:
Added checks for server availability.
Logged failures and provided actionable feedback.

# Criterion C: Development 

## Proposed Solution:
#### The solution integrates hardware (sensors and Arduino) and software (Python with libraries for data handling and REST API communication) to collect environmental data, log it locally, and upload it to a server. This approach ensures data reliability and usability by leveraging established tools and techniques.
## Appropriate Use of Existing Tools:
### Hardware:
##### DHT11 and BMP280 Sensors: Used for measuring temperature, humidity, and pressure. Chosen for their accuracy and compatibility with Arduino.
##### Arduino: Facilitates serial communication with the Python script. Chosen for its simplicity in handling sensor data.
### Software and Libraries:
##### Python: Acts as the central processing unit for handling and uploading data.
##### Requests Library: Used for making HTTP POST requests to the server for authentication and data upload. Enables seamless interaction with REST APIs.
##### Datetime and Timedelta: Used for timestamping and controlling data collection duration.
##### Serial: Used to read data from the Arduino.
##### Socket: Used to check server connectivity and handle fallback mechanisms.
##### CSV Logging: Ensures data persistence when the server is unavailable.

## Adequacy of Techniques:
#### Data Acquisition: Using sensors with Arduino ensures real-time data collection. Parsing data via Python enables efficient handling and validation.
#### Data Logging: Writing to a CSV file ensures data reliability even if server communication fails.
#### Server Communication: API calls provide a scalable solution for uploading data. The Authorization header secures the communication process.
#### Error Handling: Incorporating fallback mechanisms (e.g., logging failures and saving locally) ensures robustness in unreliable network conditions.

## Explanation of Use:
#### Why REST API?
REST APIs allow standardized communication between the Python program and the server.
The requests library simplifies the process of sending sensor data as JSON payloads.
#### Why Local Logging?
Provides a backup when the server is inaccessible, ensuring no data is lost.
#### Why Socket Connectivity Checks?
Ensures that resources aren't wasted attempting uploads when the server is down.
#### Why Python?
Python's extensive libraries and ease of integration with Arduino make it ideal for this application.

## Sources: 
### DHT:
https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
### BME:
https://iotdesignpro.com/projects/interfacing-bmp280-with-arduino-to-read-pressure-temperature-and-altitude
### Socket:
https://docs.python.org/3/library/socket.html
### DATE TIME:
https://docs.python.org/3/library/datetime.html
### PY serial:
https://pyserial.readthedocs.io/en/latest/
### Error Handling:
https://docs.python.org/3/tutorial/errors.html
### Arduino - Python Serial
https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
### Extra:
Past programs done throughout unit 2

# Criterion D: Functionality
## Scientific poster showing findings:
![Beige and White Contemporary Editorial Landscape University Research Poster](https://github.com/user-attachments/assets/24bccb03-e4a8-4890-8832-fbbbdad680d6)
