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

![pic1](https://github.com/trantoan62/Google-Earth-Pro-Flight-Simulator/assets/106214673/4a99ce33-2a20-482b-9e27-cf8b24cb6b49)

![pic2](https://github.com/trantoan62/Google-Earth-Pro-Flight-Simulator/assets/106214673/64d253c9-8528-429c-b888-a298fb269fd4)

![pic3](https://github.com/trantoan62/Google-Earth-Pro-Flight-Simulator/assets/106214673/2d6334d3-af65-45d4-b844-58d2583c4075)

![pic4](https://github.com/trantoan62/Google-Earth-Pro-Flight-Simulator/assets/106214673/64eaa196-d529-4451-9062-0367381f8861)

![pic5](https://github.com/trantoan62/Google-Earth-Pro-Flight-Simulator/assets/106214673/560c9d96-d645-401d-8203-239b26ac0830)


References: 

Author http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/f2018/sb2276/sb2276/sb2276/index.html

Google Earth https://www.google.com/earth/versions/#earth-pro

MPU6050 register map: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

pyAutoGUI reference: https://pyautogui.readthedocs.io/en/latest/

Hardware: Raspberry Pi 3 (x1), MPU6050 (x1), USB to TTL CP2102 (x1), Switchs (x6), Buttons (x7), Joystick & base

Wiring: MPU6050 to I2C1 of RPi3, CP2102 to UART of RPi3 and computer COM port, Switchs & buttons GPIO on c code.

Software: compile maybay.c to RPi3 and run, then maylaptop.py on computer
