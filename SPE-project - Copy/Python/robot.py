import roboticstoolbox as rtb
import spatialmath as sp
import PythonRobotics
import numpy as np
import threading
import serial
import time
import string
import random


class RobotPositioner():
    def __init__(self):
        self.robot = rtb.models.DH.Planar2()
        self.robot.links[0].a = 20
        self.robot.links[1].a = 15.5
        self.positions = []
        #self.arduino = serial.Serial('COM5', 115200, timeout=.1)
        self.msg = ""
        self.arduino = serial.Serial('COM5', 115200, timeout=.1)
        #self.y = threading.Thread(target=self.serialRobot)
        #self.y.start()


    def addPosition(self, l1, l2):

        self.positions.append(l1)
        self.positions.append(l2)

    def running(self):
        lastq = self.robot.q
        while True:
            if len(self.positions) > 0:
                cordinates = self.positions.pop(0)
                self.msg = str(cordinates[0]) + " : " + str(cordinates[1])
                print("PRINTING ONCE: ",cordinates[0], " : ", cordinates[1])
                T = sp.SE3(cordinates[0], cordinates[1], 0)
                sol = self.robot.ikine_min(T).q
                qt = rtb.jtraj(lastq, sol, 20)
                # print(qt.yd)
                # self.robot.plot(qt.y, block=False, jointaxes=True, shadow=True)
                start_time = time.time()
                last_time = 0
                counter = 0
                # arduino = serial.Serial('COM5', 115200, timeout=.1)
                time.sleep(1)  # give the connection a second to settle
                while counter < len(qt.y):
                    current_time = time.time()
                    diff = current_time - last_time
                    if diff > 0.1:
                        deg1 = qt.y[counter][0] * (360.0 / (2 * np.pi))
                        deg2 = qt.y[counter][1] * (360.0 / (2 * np.pi))

                        # while True:
                        msg = '#' + str(deg1) + ":" + str(deg2) + '#'

                        # print(msg)
                        this = str.encode(str(msg))
                        self.arduino.write(this)
                        data = self.arduino.readline()
                        if data:
                            print( 'sent   ', msg, ' received:   ',data.decode())
                            pass
                        time.sleep(0.000001)

                        last_time = current_time
                        counter += 1
                now = True
                while True:
                    data = self.arduino.readline()
                    current_time = time.time()
                    diff = current_time - last_time
                    if data:
                        print(data)
                        pass
                    else :
                        if now:
                            last_time = current_time
                            now = False
                        if  diff > 5:
                            print('ready for next')
                            break

                # print("done ------")
                lastq = sol

    def serialRobot(self):

        while True:
            self.arduino.write(str.encode(self.msg))
            data = self.arduino.readline()
            if data:
                #print(data.decode())
                pass
