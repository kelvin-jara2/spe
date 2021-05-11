import roboticstoolbox as rtb
import spatialmath as sp
import PythonRobotics
import numpy as np
import threading
import serial
import time
import string
import random


robot = rtb.models.DH.Planar2()
robot.links[0].a = 20
robot.links[1].a = 15.5
positions = []
# arduino = serial.Serial('COM5', 115200, timeout=.1)
msg = ""

init = [35.5, 0]
final = [35.5, -15.5]
# final = [0, 35.5]
Lenght = np.sqrt((init[0] - final[0])**2 + (init[1] - final[1])**2)
N = 20
T = sp.SE3(init[0], init[1], 0)
T2 = sp.SE3(final[0], final[1], 0)
sol = robot.ikine_min(T).q
sol2 = robot.ikine_min(T2).q
qt = rtb.jtraj(sol, sol2, N)
# robot.plot(qt.y, block=False, jointaxes=True, shadow=True)
print(qt.y*360.0/(2*np.pi))
# robot.plot(qt.y, block=False, jointaxes=True, shadow=True)
# print(len(qt.x))
for i in range(1, len(qt.x)):
    dif = qt.x[i] - qt.x[i-1]
    # print(dif)


start_time = time.time()
last_time = 0
counter = 0
arduino = serial.Serial('COM5', 115200, timeout=.1)
time.sleep(1)  # give the connection a second to settle
while counter < len(qt.y):
    current_time = time.time()
    diff = current_time - last_time
    l = True
    #if diff > 0.01:
    if l:
        deg1 = qt.y[counter][0]*(360.0/(2*np.pi))
        deg2 = qt.y[counter][1]*(360.0/(2*np.pi))

        # while True:
        msg = '#' + str(deg1) + ":" + str(deg2) + '#'
        # print(msg)
        this = str.encode(str(msg))
        arduino.write(this)
        data = arduino.readline()
        if data:
            print(data)
            pass
        time.sleep(0.00001)

        last_time = current_time
        counter += 1
print("I am entering" )

while True:
    data = arduino.readline()
    if data:
        print(data)
        pass







"""
data = self.arduino.readline()
if data:
    print(data.decode())
"""

