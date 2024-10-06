# Omni-Bot Control System

This project is designed to control an omnidirectional robot (Omni-Bot) using an ESP microcontroller. It sets up a local WiFi access point and a web-based control interface. The robot’s movements are controlled through a WebSocket connection, allowing real-time motor commands from a browser interface.

## Table of Contents
1. [Hardware Setup](#hardware-setup)
2. [Motor Control](#motor-control)
3. [WiFi Setup](#wifi-setup)
4. [WebSocket Interface](#websocket-interface)
5. [HTML Control Interface](#html-control-interface)
6. [Functions Overview](#functions-overview)
7. [Running the Project](#running-the-project)

## Hardware Setup
The system uses an ESP board to connect to WiFi and control three motors. The following pins are assigned to control the motors:

- **Motor A Pins (AIN1, AIN2)**:
  - AIN1: Pin 15
  - AIN2: Pin 2

- **Motor B Pins (BIN1, BIN2)**:
  - BIN1: Pin 16
  - BIN2: Pin 4

- **Motor C Pins (CIN1, CIN2)**:
  - CIN1: Pin 17
  - CIN2: Pin 5

All motor pins are configured as output and controlled using digital signals to move the robot in different directions.

## Motor Control
The robot can perform the following movement operations based on WebSocket commands:

- **Forward**: Moves the robot forward.
- **Backward**: Moves the robot backward.
- **Left**: Rotates the robot to the left.
- **Right**: Rotates the robot to the right.
- **Stop**: Stops all motors.
- **Forward-Right**: Moves diagonally forward and right.
- **Forward-Left**: Moves diagonally forward and left.
- **Backward-Right**: Moves diagonally backward and right.
- **Backward-Left**: Moves diagonally backward and left.
- **Rotate Counterclockwise**: Rotates the robot counterclockwise.
- **Rotate Clockwise**: Rotates the robot clockwise.

These functions are defined in the motor control logic, which checks the received WebSocket commands and adjusts the motor outputs accordingly.

### Motor Functions:
Each motor function sets the motor pins to enable the desired direction of rotation. For example:
- **Moving forward** turns on motor A and motor B in the forward direction.
- **Turning left** rotates motor A forward and motor B backward while engaging motor C to assist with the turn.

### Motor Safety
To prevent erratic behavior, all motors are stopped initially, which sets all motor control pins to a low state.

## WiFi Setup
The ESP operates as an access point (AP), meaning it creates its own WiFi network to which clients can connect. The WiFi credentials are:
- **SSID**: OMNI BOT
- **Password**: 123456789

Once connected to this network, clients can access the web-based control interface by entering the IP address of the ESP, which is displayed in the serial monitor.

## WebSocket Interface
A WebSocket server is created on port 81, which allows for bidirectional communication between the ESP and the client (web browser). This enables real-time control of the robot without needing to refresh the page.

### WebSocket Events
- **Text messages**: Used to send commands from the web interface to control the robot’s movements.
- **Binary messages**: Handled but not used in this project.

### WebSocket Commands:
The WebSocket receives a single character as a command that maps to specific robot movements (e.g., 'F' for forward, 'L' for left, etc.).

## HTML Control Interface
The control interface is hosted directly on the ESP and is served when accessing the root URL ("/"). The page includes buttons for each direction or action, such as forward, backward, left, right, stop, and diagonal moves. It also provides buttons to rotate the robot clockwise and counterclockwise.

### Interface Design:
The buttons are styled with CSS and are assigned different colors depending on their purpose (e.g., green for movement, red for stopping). Each button sends a command to the WebSocket when clicked, triggering the corresponding movement on the robot.

## Functions Overview

### Motor Functions
- **moveForward()**: Drives motors A and B forward.
- **moveBackward()**: Reverses motors A and B.
- **turnLeft()**: Rotates left by running motors in opposite directions.
- **turnRight()**: Rotates right by running motors in opposite directions.
- **stopMotors()**: Stops all motors by setting motor control pins to a low state.

### WebSocket Event Handler
The WebSocket event handler listens for incoming messages and triggers the appropriate motor functions based on the received data.

## Running the Project

1. **Set Up the ESP**:
   - Upload the code to your ESP using the Arduino IDE.
   - Open the Serial Monitor to see the ESP's IP address.

2. **Connect to WiFi**:
   - On your device, connect to the WiFi network `OMNI BOT` using the password `123456789`.

3. **Access the Control Page**:
   - Enter the IP address of the ESP in your browser (displayed in the Serial Monitor).
   - Use the buttons on the page to control the robot in real-time.

---

This guide covers everything needed to set up and control your Omni-Bot via a web interface, using the ESP to manage WiFi and WebSocket communication for real-time robot movement.
##Images:
![image](https://github.com/user-attachments/assets/1bbe7ac1-68e0-4b5f-8792-0e00a5a01693)
![image](https://github.com/user-attachments/assets/0d8ccd75-c73e-442b-8ee8-e33f7ee69a66)

