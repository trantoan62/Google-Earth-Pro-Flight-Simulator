# Google Earth Pro Flight Simulator (run in laptop using pyAutoGUI)
# Team member:      Student ID
# Tran Quoc Toan    19146090
# Le Hoang Hoa      19146122
# Nguyen Minh Huy   19146006
#
# python code, using UART communication, when u run, u should Ctrl Tab to google Earth in 1s
# when you want to stop the code not using hardware, Ctrl Tab to python code, Ctrl `, Ctrl C or stop sending serial data
# receive serial code every 0.5s, then do the code in less than 0.5s
# mean that delay time might be about 0 to 1s 
#########################################################

import serial # import module for serial communication
import pyautogui # import module for keyboard/mouse control
import io # import module to to use universal newline mode
import time # to measure time

# open Serial Port 5
ser = serial.Serial(
	port = 'COM5',
	baudrate = 9600,
	timeout = 0.1
)
ser.close()
ser.open() # open serial communication
print('COM 5 Open: ', ser.is_open)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
ser.flush() # wait until all data is written

pyautogui.FAILSAFE = False # set failsafe for manual exit
width, height = pyautogui.size() # set size of screen
print('Press Ctrl-C to quit.')
pyautogui.MINIMUM_DURATION = 0 # Default: 0.1
pyautogui.MINIMUM_SLEEP = 0 # Default: 0.05
pyautogui.PAUSE = 0  # Default: 0.1

flap_up_prev = 0
flap_down_prev = 0
pause_prev = 0
landing_gear_prev = 0
throttle_prev = 0
rudder_prev = 12
exitValue = 1
bientam = 1

xn_prev = 0
yn_prev = 0
brake_left_prev = 0
brake_right_prev = 0
x_prev = 0
y_prev = 0
x = 0
y = 0
t = time.time() # ham delay = 0.5
while (time.time() - t < 0.5):
    pass
t = time.time()
pyautogui.moveTo(width/2, height/2) # set mouse to middle
pyautogui.click() # click to using mouse controlling mode
print('hey')
while (exitValue):
    while (time.time() - t < 0.5):
        pass
    t = time.time()  # ham delay = 0.5, timeout luc doc serial = 0.1, tang giam gia tri = 0 den 0.2
    line = sio.readline()
    line = line.rstrip()
    data = line.split()
    print("yo")
    print(data)

    temp = int(data[0]) # luu cac gia tri thu 1

    ##### 3 buttons:
    # center switch
    if int(int(temp>>3) % 2):
        pyautogui.press('c')
    
    # exit control
    if int(int(temp>>7) % 2):
        exitValue = 0 # out of while() loop
        pyautogui.click()

    # pause button   vi la isr
    if int(int(temp>>2) % 2)!=pause_prev:
        if (bientam == 1):
            pyautogui.press('space') # hold thu cong
        bientam = bientam ^ 1
        pause_prev = int(int(temp>>2) % 2)
        # stop controlling the mouse & keyword
        # neu bientam == 0 ^ 1 = 1 o 2 dong tren: (do trinh bay de hieu)
        
        if (bientam == 1):
            # flaps up   gia tri = 1 => increase
            if int(int(temp>>0) % 2):
                pyautogui.typewrite('[')

            # flaps down   gia tri = 1 => increase
            if int(int(temp>>1) % 2):
                pyautogui.typewrite(']')

            # landing gear   gia tri = 1 nhung viet nhu tac gia cung duoc
            if int(int(temp>>4) % 2)!=landing_gear_prev:
                pyautogui.press('g')
                landing_gear_prev = int(int(temp>>4) % 2)

            # left brake   gia tri = 1 + hold brake
            if int(int(temp>>5) % 2)!=brake_left_prev:
                if int(int(temp>>5) % 2)!=0:
                    pyautogui.keyDown(',')
                else:
                    pyautogui.keyUp(',')
            brake_left_prev = int(int(temp>>5) % 2)

            # right brake   gia tri = 1 + hold brake
            if int(int(temp>>6) % 2)!=brake_right_prev:
                if int(int(temp>>6) % 2)!=0:
                    pyautogui.keyDown('.')
                else:
                    pyautogui.keyUp('.')
            brake_right_prev = int(int(temp>>6) % 2)

            # rudder  vi nhan tin hieu moi 0.5s, nen tang giam gia tri tu tu 0.1s tranh bi anh huong task
            # rudder moi ben can 1.2s tinh tu vi tri chinh gia, tuc 25 thang moi lan hold 0.1s
            rudder = int(data[3])
            if (rudder-rudder_prev)<0:
                temptime = time.time()
                pyautogui.keyDown('shift')
                pyautogui.keyDown('right')
                while (time.time() - temptime < 0.1): # hold 0.1s
                    pass
                pyautogui.keyUp('shift')
                pyautogui.keyUp('right')
                rudder_prev = rudder_prev - 1
            if (rudder-rudder_prev)>0:
                temptime = time.time()
                pyautogui.keyDown('shift')
                pyautogui.keyDown('right')
                while (time.time() - temptime < 0.1):
                    pass
                pyautogui.keyUp('shift')
                pyautogui.keyUp('right')
                rudder_prev = rudder_prev + 1

            # throttle  tuong tu rudder, nhung chi co 12 gia tri, xuat phat tu 0
            throttle = int(data[4])
            if (throttle-throttle_prev)<0:
                temptime = time.time()
                pyautogui.keyDown('pagedown')
                while (time.time() - temptime < 0.1): # hold 0.1s
                    pass
                pyautogui.keyUp('pagedown')
                throttle_prev = throttle_prev - 1
            if (throttle-throttle_prev)>0:
                temptime = time.time()
                pyautogui.keyDown('pageup')
                while (time.time() - temptime < 0.1):
                    pass
                pyautogui.keyUp('pageup')
                throttle_prev = throttle_prev + 1

            # roll, pitch
            x = int(data[1])
            y = int(data[2])
            if abs(x) < 10: #tranh sai so o vi tri xuat phat
                x = 0
            if abs(y) < 10: 
                y = 0
            if abs(x - x_prev) < 4: #tranh sai so sensor
                x = x_prev
            if abs(y - y_prev) < 4:
                y = y_prev
            if x > 60: #gioi han goc
                x = 60
            if x < -60:
                x = -60
            if y > 60:
                y = 60
            if x < -60:
                x = -60
            xn = -x*4 + width/2
            yn = y*4 + height/2
            pyautogui.moveTo(xn, yn) # move the mouse
            x_prev = x
            y_prev = y