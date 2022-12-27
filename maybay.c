/* Google Earth Pro Flight Simulator (on Raspberry Pi 3)
*
* Ten thanh vien:	MSSV
* Tran Quoc Toan 	19146090
* Le Hoang Hoang	19146122
* Nguyen Minh Huy 	19146006
*
* Review: Nhom su dung MPU6050, 6 switch, 7 button, giao tiep serial UART thong qua CP2102
* doc va gui data thong moi 500ms
* button = irq
*
* gcc -Wall maybay.c -l wiringPi -lm -o maybay
* ./maybay
*********************************************************************************/

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <errno.h>

// MPU6050 I2C define:
#define SMPLRT_DIV 				25
#define CONFIG 					26
#define GYRO_CONFIG				27
#define ACCEL_CONFIG			28
#define INT_ENABLE				56
#define ACCEL_XOUT_H			59
#define ACCEL_YOUT_H			61
#define ACCEL_ZOUT_H			63
#define GYRO_XOUT_H				67
#define GYRO_YOUT_H				69
#define GYRO_ZOUT_H				71
#define PWR_MGMT_1				107
int mpu6050; // I2Csetup
//float pitch = 0, roll = 0;

// GPIO pin:
#define FLAP_UP_BT 		 0 // pin gpio 11 //switch
#define FLAP_DOWN_BT 	 2 // pin gpio 13 //switch
#define PAUSE_BT 		 3 // pin gpio 15 //button
#define CENTER_BT 		23 // pin gpio 33 //button
#define LANDING_GEAR_BT	24 // pin gpio 35 //switch
#define BRAKE_LEFT_BT	25 // pin gpio 37 //switch
#define BRAKE_RIGHT_BT	 1 // pin gpio 12 //switch
#define RUDDER_LEFT_BT	 4 // pin gpio 16 //switch
#define RUDDER_RIGHT_BT	 5 // pin gpio 18 //switch
#define EXIT_BT			 6 // pin gpio 22 //button
#define THROTTLE_UP_BT	26 // pin gpio 29 //switch
#define THROTTLE_DOWN_BT 27// pin gpio 31 //switch
#define THROTTLE_LEVEL1	26 // pin gpio 32 //button + hold
#define THROTTLE_LEVEL2	27 // pin gpio 36 //button + hold
#define THROTTLE_LEVEL3	28 // pin gpio 38 //button + hold
#define THROTTLE_LEVEL4	29 // pin gpio 40 //button + hold

uint8_t flapUp = 0, flapDown = 0, pauseValue = 0, center = 0, landingGear = 0;
uint8_t brakeLeft = 0, brakeRight = 0, rudderLeft = 0, rudderRight = 0, rudder = 12;
uint8_t exitValue = 0, throttleUp = 0, throttleDown = 0, throttle = 0, level = 1;
void Init(void) {
    // polling rate 2000Hz cho nhanh, 2000 = 8000/(1 + 3)
    wiringPiI2CWriteReg8(mpu6050, SMPLRT_DIV, 3);
    // no external Frame Sync (FS) and no Digital low pass filter (DLPF)
    wiringPiI2CWriteReg8(mpu6050, CONFIG, 0);
	// gyro FS: 250 degree/sec => gyro to lsb => /131 (khong xai)
    wiringPiI2CWriteReg8(mpu6050, GYRO_CONFIG, 0x00);
	// acc FS: 8g acc to lsb => / 4096
    wiringPiI2CWriteReg8(mpu6050, ACCEL_CONFIG, 0x010);
	// setup interrupt pin
    wiringPiI2CWriteReg8(mpu6050, INT_ENABLE, 1);
	// Phase-locked loop with X axis gyroscope reference
    wiringPiI2CWriteReg8(mpu6050, PWR_MGMT_1, 1);
}

// read any IMU sensor value
int16_t readSensor(unsigned char sensorPin) {
	int16_t high, low, data;
	high = wiringPiI2CReadReg8(mpu6050, sensorPin);
	low = wiringPiI2CReadReg8(mpu6050, sensorPin + 1);
	data = (high << 8) | low;
	return data;
}

void pauseISR(void){ pauseValue = 1;} //logic xor
void centerISR(void){ center = 1; }
void exitISR(void){ exitValue = 1; }

int main(void){
    // setup I2C mpu6050
	mpu6050 = wiringPiI2CSetup(0x68);
	Init();
	printf("Done I2Csetup\n");
	
	// setup GPIO
	wiringPiSetup();
	pullUpDnControl(THROTTLE_UP_BT, PUD_DOWN);
	pullUpDnControl(THROTTLE_DOWN_BT, PUD_DOWN);
	
	pinMode(FLAP_UP_BT, INPUT);
	pinMode(FLAP_DOWN_BT, INPUT);
	pinMode(PAUSE_BT, INPUT);
	pinMode(CENTER_BT, INPUT);
	pinMode(LANDING_GEAR_BT, INPUT);
	pinMode(BRAKE_LEFT_BT, INPUT);
	pinMode(BRAKE_RIGHT_BT, INPUT);
	pinMode(RUDDER_LEFT_BT, INPUT);
	pinMode(RUDDER_RIGHT_BT, INPUT);
	pinMode(EXIT_BT, INPUT);
	pinMode(THROTTLE_UP_BT, INPUT);
	pinMode(THROTTLE_DOWN_BT, INPUT);
	pinMode(THROTTLE_LEVEL1, INPUT);
	pinMode(THROTTLE_LEVEL2, INPUT);
	pinMode(THROTTLE_LEVEL3, INPUT);
	pinMode(THROTTLE_LEVEL4, INPUT);
	
	wiringPiISR(PAUSE_BT, INT_EDGE_RISING, &pauseISR);
	wiringPiISR(CENTER_BT, INT_EDGE_RISING, &decrease_flap);
	wiringPiISR(EXIT_BT, INT_EDGE_RISING, &pause_mode);
	printf("Done GPIO Setup\n");
	
	//serial CP2102 (su dung pin 14 15 UART) (USB to TTL)
	int fd;
		printf("Raspberry's sending : \n");
 
	if((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0 ){
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
	}
	
	int t = millis();
    while(1){
		while (millis() - t < 500){} //delay moi 500ms de sensor MPU doc
		t = millis();
		//// doc cac tin hieu GPIO: //(hold button hoac switch khong dung ISR)
		flapUp = digitalRead(FLAP_UP_BT);
		flapDown = digitalRead(FLAP_DOWN_BT);
		//pauseValue = digitalRead(PAUSE_BT); //dung ISR
		//center = digitalRead(CENTER_BT); //dung ISR
		landingGear = digitalRead(LANDING_GEAR_BT);
		brakeLeft = digitalRead(BRAKE_LEFT_BT);
		brakeRight = digitalRead(BRAKE_RIGHT_BT);
		rudderLeft = digitalRead(RUDDER_LEFT_BT);
		rudderRight = digitalRead(RUDDER_RIGHT_BT);
		//exitValue = digitalRead(EXIT_BT); //dung ISR
		throttleUp = digitalRead(THROTTLE_UP_BT);
		throttleDown = digitalRead(THROTTLE_DOWN_BT);
		if (digitalRead(THROTTLE_LEVEL4) == 1){level = 5;}
		else if (digitalRead(THROTTLE_LEVEL3) == 1){level = 4;}
		else if (digitalRead(THROTTLE_LEVEL2) == 1){level = 3;}
		else if (digitalRead(THROTTLE_LEVEL1) == 1){level = 2;}
		else {level = 1;}
		if (rudderRight == 1 && rudder < 24){
			rudder = rudder + level;
			if (rudder > 24) {rudder = 24;}
		}
		else if (rudderLeft == 1 && rudder > 0){
			if (rudder <= level) {rudder = 0;}
			else {rudder = rudder - level;}
		}
		if (throttleUp == 0 && throttle < 12){
			throttle = throttle + level;
			if (throttle > 12) {throttle = 12;}
		}
		else if (throttleDown == 0 && throttle > 0){
			if (throttle <= level) {throttle = 0;}
			else {throttle = throttle - level;}
		}
		float xAccel = (float)readSensor(ACCEL_XOUT_H)/4096.0;
		float yAccel = (float)readSensor(ACCEL_YOUT_H)/4096.0;
		float zAccel = (float)readSensor(ACCEL_ZOUT_H)/4096.0;
		
		// tinh toan pitch roll
		int roll = (int)(atan2f(yAccel, sqrt(xAccel*xAccel + zAccel*zAccel))*180/M_PI);
		int pitch = (int)(atan2f(xAccel, sqrt(yAccel*yAccel + zAccel*zAccel))*180/M_PI);
		
		//// gui data:
		char str[20];
		// chuyen thanh ma nhi phan de data gui nhanh hon
		int bigdata = (flapUp << 0) | (flapDown << 1) | (pauseValue << 2) | (center << 3) | (landingGear << 4) | (brakeLeft << 5) | (brakeRight << 6) | (exitValue << 7);
		center = 0; // reset neu co ISR
		exitValue = 0; // tuong tu
		pauseValue = 0; // tuong tu
 		sprintf(str, "%d %d %d %d %d\r\n", bigdata, roll, pitch, rudder, throttle); //luu gia tri
		serialPuts(fd, str); // gui data
		serialFlush(fd);
		fflush(stdout);
		puts(str); //printf()
    }
	return 0;
}