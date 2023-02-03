# ESP32 Logging Data in Google Sheets
This software has the ability to send information vectors from an esp32 to a google spreadsheet through wifi. In this code data is being sent from a Humidity 
Sensor, Temperature Sensor and an LDR. <br> To do so, a POST is executed using the 'doPost' function in google script. There are still functions 'deleteRows' 
and 'dataCopy' that are executed from a GET.
