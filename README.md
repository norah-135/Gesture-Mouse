Gesture Mouse: AI-Powered Hands-Free Controller
Gesture Mouse is an innovative, hands-free computer peripheral system that allows users to control the mouse cursor using head movements and perform clicks using eye gestures. By combining hardware sensors with AI-powered computer vision, it provides a seamless touchless interaction experience.

Features:
Head-Motion Tracking: Smooth cursor movement controlled by head tilts via an MPU6050 sensor.

AI Eye-Wink Detection: Trigger Left and Right clicks by simply winking your eyes.

Hybrid Architecture: Combines the precision of hardware (Arduino) with the intelligence of software (Python & MediaPipe).

Accessibility Focused: Designed as an assistive technology for individuals with limited hand mobility.

 Hardware Requirements
Arduino Uno (or any compatible board).

MPU6050 Sensor (Gyroscope & Accelerometer).

Standard Webcam.

4 wires .

 Software Requirements:
Python 3.12+

Arduino IDE (for uploading the C++ sketch).

Required Python Libraries:
pip install mediapipe opencv-python pyautogui pyserial

project Structure:
eyeDeductionCode.py: The main Python script for eye-tracking and clicking.

arduinoMainControlMovmentCode.ino: The Arduino sketch for head-motion tracking.

face_landmarker.task: The MediaPipe AI model for facial landmark detection.

Setup & Installation:
Hardware: Connect the MPU6050 to the Arduino (VCC to 3.3V, GND to GND, SCL to SCL, SDA to SDA).

Arduino: Upload the provided .ino code to your Arduino board.

Model: Ensure the face_landmarker.task file is in the same directory as your Python script.

How to Use?
Move Cursor: Tilt your head up, down, left, or right.

Left Click: Wink your Left Eye.

Right Click: Wink your Right Eye.


This project is open-source. Feel free to use and modify it :) !
