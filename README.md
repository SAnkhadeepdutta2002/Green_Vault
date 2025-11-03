# GREENVAULT — Solar-Powered, Edge-AI & IoT based Smart Storage System
A **solar-powered**, **IoT-connected** cold-storage system that runs **YOLOv8** vision to detect spoilage & remotely monitor internal environment
[![Python](https://img.shields.io/badge/Python-3.10-blue)]()
[![Arduino](https://img.shields.io/badge/Arduino-ESP8266-orange)]()
[![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-4-green)]()
[![Firebase](https://img.shields.io/badge/Firebase-RealtimeDB-yellow)]()
![YOLO](https://img.shields.io/badge/YOLO-v8-orange)

## ⚠️ Problem Statement
• **Perishable produce** spoils fast without continuous monitoring, manual checks miss early signs.                                                         
• No affordable system combining **IoT sensing + visual based** spoilage detection.  
• Lower **grid-dependence** enables clean, self-sufficient storage for rural farms.

## 🚀 System Overview — What GreenVault Does & Why
- 🌡️ **Real-time sensing (ESP8266 + DHT11 + MQ2):** monitors temperature, humidity, & gas levels to maintain ideal storage conditions.
- 👁️‍🗨️ **Spoilage Detection (Raspberry Pi + YOLOv8):** Captures images on-device & identifies spoiled produce automatically to prevent rot.
- ☁️📱 **Cloud connectivity (Firebase + App):** Streams sensor data & AI results to Firebase for remote monitoring, alerts, & control logic — accessible via a companion mobile app for live dashboard and control.
- ⚙️📟 **Actuation & Display:** LCD shows live readings; relay enables automated temperature control based on threshold values.
- ⚡🔆**Solar-ready Design:** built for off-grid farms & clean-energy operation.  

## System Architecture
<p align="center">
  <img src="https://github.com/user-attachments/assets/ca6dd29d-d6ca-416a-803c-029e1261d671" width="760">
</p>


## ⚙️ Tech Stack

<div align="center">

<table>
<tr>
<th>Component</th><th>Stack</th>
</tr>

<tr>
<td><b>Embedded MCU</b></td>
<td>ESP8266 (Arduino IDE) &nbsp; <img src="https://skillicons.dev/icons?i=arduino" width="22"/></td>
</tr>

<tr>
<td><b>Sensors</b></td>
<td>DHT11 🌡️ &nbsp; , &nbsp; MQ-4 Gas Sensor 🧪</td>
</tr>

<tr>
<td><b>SBC (Vision) </b></td>
<td>Raspberry Pi 4 + Pi Camera &nbsp; <img src="https://skillicons.dev/icons?i=raspberrypi" width="22"/></td>
</tr>

<tr>
<td><b>ML Model</b></td>
<td>YOLOv8 (Edge inference) &nbsp; <img width="24" height="22" src="https://github.com/user-attachments/assets/9246dd4b-eec3-4037-b1da-cdd3816453ca" /></td>
</tr>

<tr>
<td><b>Programming</b></td>
<td>
Embedded C/C++ &nbsp; 
<img src="https://skillicons.dev/icons?i=c" width="20"/> 
<img src="https://skillicons.dev/icons?i=cpp" width="20"/> 
&nbsp; + Python (Pi) &nbsp; 
<img src="https://skillicons.dev/icons?i=python" width="22"/>
</td>
</tr>

<tr>
<td><b>Mobile App</b></td>
<td>MIT App Inventor &nbsp; <img width="22" height="22" src="https://github.com/user-attachments/assets/786bf347-ef42-43c8-9b26-63e7c7b82448" /></td>
</tr>

<tr>
<td><b>Cloud Backend</b></td>
<td>Firebase Realtime DB &nbsp; <img src="https://skillicons.dev/icons?i=firebase" width="22"/></td>
</tr>
</table>

</div>













