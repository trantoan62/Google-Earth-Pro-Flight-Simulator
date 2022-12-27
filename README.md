# Google-Earth-Pro-Flight-Simulator
Google Earth Pro Flight Simulator code for study

* Google Earth Pro Flight Simulator
*
* Name              StudentID
* Tran Quoc Toan 	  19146090
* Le Hoang Hoang	  19146122
* Nguyen Minh Huy 	19146006
* 
* Date: 27/12/2022

Descrpition: A flight simulator device with Joystick using IMU MPU6050 module, buttons and switchs, using Raspberry Pi 3 as a microcontroller to read and send input though UART serial by CP2102 to pyAutoGUI on th computer, to fly a plane in Google Earth Pro Flight Simulator

References: 

Author http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/f2018/sb2276/sb2276/sb2276/index.html

Google Earth https://www.google.com/earth/versions/#earth-pro

MPU6050 register map: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

pyAutoGUI reference: https://pyautogui.readthedocs.io/en/latest/

Hardware: Raspberry Pi 3 (x1), MPU6050 (x1), USB to TTL CP2102 (x1), Switchs (x6), Buttons (x7), Joystick & base

Wiring: MPU6050 to I2C1 of RPi3, CP2102 to UART of RPi3 and computer COM port, Switchs & buttons GPIO on c code.

Software: compile maybay.c to RPi3 and run, then maylaptop.py on computer
