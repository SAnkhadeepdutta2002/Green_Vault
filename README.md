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
- 👁️‍🗨️ **Edge-AI spoilage detection (Raspberry Pi + YOLOv8):** Captures images on-device & identifies spoiled produce automatically to prevent unnoticed rot.
- ☁️📱 **Cloud connectivity (Firebase + App):** Streams sensor data & AI results to Firebase for remote monitoring, alerts, & control logic — accessible via a companion mobile app for live dashboard and control.
- ⚙️📟 **Actuation & display:** LCD shows live readings; relay enables automated temperature control based on threshold values.
- ⚡🔆**Solar-ready design:** built for off-grid farms & clean-energy operation.  

