# 4WD OLED AI Robot Documentation

## Table of Contents
- [Project Overview](#project-overview)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [Software Components](#software-components)
- [Project Structure](#project-structure)
- [Installation Guide](#installation-guide)
- [Development Guide](#development-guide)
- [Contributing](#contributing)
- [License & Contact](#license--contact)

## Project Overview

The 4WD OLED AI Robot is an autonomous vehicle project based on TinyML, utilizing ESP32-CAM and Raspberry Pico for real-time control. The robot features various sensors and motor drivers, enabling autonomous movement and environmental interaction through machine learning algorithms.

### Key Features
- Real-time control using ESP32-CAM and Raspberry Pico
- Multiple sensor integration for environmental awareness
- Mobile application control interface
- Advanced learning algorithms implementation
- SLAM (Simultaneous Localization and Mapping) capability

## System Architecture

### 1. Web Interface (Vue.js & JavaScript)
- **Dashboard**: Overall robot status and basic controls
- **Motor Dashboard**: Motor control and monitoring
- **Sensor Configuration**: Sensor setup and calibration
- **Robot Design**: Robot configuration interface
- **SLAM Interface**: Mapping and navigation controls

### 2. Backend (Python)
- Motor control module
- Sensor data processing
- SLAM implementation
- WebSocket server for real-time communication
- REST API endpoints

### 3. Hardware Control (C++)
- Arduino/microcontroller firmware
- Motor control systems
- OLED display management
- Sensor integrations

## Hardware Components

### Sensors
1. **MPU6050 6-Axis Accelerometer & Gyroscope**
   - Purpose: Motion and orientation detection
   - Usage: Balance control and navigation

2. **L298 Mini DC & Stepper Motor Driver**
   - Purpose: Motor control
   - Usage: Robot movement control

3. **OLED Display (SSD1306)**
   - Purpose: Status display
   - Usage: Operating status and sensor data visualization

4. **Additional Sensors**
   - Vibration sensor
   - Photoresistor
   - Contact switch
   - Tilt switch
   - 3-color LED module

### Power Management
- **TP4056 LiPo Charger**
  - Purpose: Battery charging
  - Usage: Power supply management

## Software Components

### Directory Structure
```
project-root/
├── esp32cam/           # ESP32-CAM Python files
│   └── main.py
├── frontend/          # Vue.js frontend files
│   └── src/
├── future/           # Future development files
├── pico/            # Raspberry Pico files
│   └── main.py
├── src/             # C/C++ source files
├── tools/           # Utility tools
│   └── esp32_uploader.py
├── web/             # Web-related files
├── @vue_cli.json
└── package.json
```

## Installation Guide

### Prerequisites
- Node.js and npm
- Python 3.x
- Arduino IDE
- Required hardware components

### Setup Steps
1. **Clone the Repository**
   ```bash
   git clone <repo_link>
   cd <project_directory>
   ```

2. **Install Dependencies**
   ```bash
   npm install
   ```

3. **Start the Project**
   ```bash
   npm run serve
   ```

4. **Hardware Setup**
   - Connect electronic components
   - Verify power supply
   - Calibrate sensors

## Development Guide

### System Security Recommendations
- Implement user authentication
- Add API security layer
- Encrypt WebSocket connections

### Interface Improvements
- Optimize real-time data flow
- Enhance error handling
- Improve responsive design

### Sensor Integration
- Create modular structure for new sensor types
- Enhance sensor data processing
- Implement automatic calibration

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/new-feature`
3. Commit changes: `git commit -m "Add new feature"`
4. Push to branch: `git push origin feature/new-feature`
5. Submit a Pull Request

## License & Contact

### License
This project is licensed under the MIT License.

### Contact
- **Project Owner**: Selçuk Aydın
- **Email**: selcukaydintr@gmail.com
- **Issues**: [GitHub Issues](https://github.com/selcukaydintr/4wd-oled-ai-robot/issues)
