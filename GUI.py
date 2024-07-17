# Imports #############################################################################################

import socket

import matplotlib.pyplot as plt
# NOTE IF USING LAB PC: At writing, I pip install matplotlib globally (I am not sure
# too tbh), so next time if error occur, might need to pip install matplotlib)

import numpy as np

import math

# Imports END -----------------------------------------------------------------------------------------


# Set up socket to connect with TCP ############################################

TCP_IP = "192.168.1.1"
TCP_PORT = 288

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# -----------------------------------------------------------------------------------------------------

# Start socket connection
try:
    s.connect((TCP_IP, TCP_PORT))
    print("Successful connection!")
except:
    print("Connection Failed")
while (True):
    command = input("Enter a command: ")
    s.send(command.encode())

#-------------Scan the objects and display with matplotlibb---------------------
    if(command == 'n'):
        # Numpy Arrays to store the degrees and distances received from Cybot
        degArr = np.array([])
        radArr = np.array([])
        distancesArr = np.array([])
        areaArr = np.array([])
        widthArr = np.array([])
        d1 = np.array([])
        d2 = np.array([]) 
        totalD = np.array([])
        xcood = np.array([])
        ycood = np.array([])

        y = 0

        #-------Read data from Cybot-----------
        while True:
            data = s.recv(32).decode() 

            print(data)
            dataList = data.split(", ")

            data_deg = dataList[0] # deg = degree
            data_dis = dataList[1]
            data_wid = dataList[2]
            data_area = dataList[3]

            # Check if no more data is being sent
            if (data_deg == "end"):
                # Break out of loop
                break

            processed_data_deg = (int) (data_deg)
            processed_data_dis = (int) (data_dis)
            processed_data_wid = (int) (data_wid)
            processed_data_area = (int) (data_area)

            # processed_data_deg but in radian
            processed_data_rad = (int) (data_deg) * np.pi / 180

            degArr = np.append(degArr, processed_data_deg)
            radArr = np.append(radArr, processed_data_rad)
            distancesArr = np.append(distancesArr, processed_data_dis)
            widthArr = np.append(widthArr, processed_data_wid)
            areaArr = np.append(areaArr, processed_data_area*20)
            print("degree: " + str(processed_data_deg) + "   dis: " + str(distancesArr[y]) +"    Width: "+ str(widthArr[y]) + "   Area: " + str(areaArr[y]))
            y+=1

        #--------Calculate the distance between point A and point B----------
        i = 0
        while(i<len(degArr)-1):
            end = float(degArr[i] + widthArr[i]/2)
            begin = float(degArr[i+1] - widthArr[i+1]/2)
            angle = (end-begin)/2
            angle = np.deg2rad(angle)
            xd1 = abs(np.sin(angle)*distancesArr[i])
            np.append(d1, np.deg2rad(end))
            xd2 = abs(np.sin(angle)*distancesArr[i+1])
            np.append(d2, np.deg2rad(begin))
            xtotalD = int(xd1+xd2)
            np.append(totalD, xtotalD)

            # centerX = (degreesArr[i] + degreesArr[i+1])/2
            # centerY = (distancesArr[i] + distancesArr[i+1])/2
            # np.append(xcood, centerX)
            # np.append(ycood, centerY)

            print(f'P1: [{end},{distancesArr[i]}]   P2: [{begin}, {distancesArr[i+1]}]   Dist: {xtotalD} cm\n')
            i += 1

        print("Processing GUI...")

        fig = plt.figure()
        ax = fig.add_subplot(projection='polar')

        # Scatter plot on polar axis confined to a sector
        ax.set_thetamin(0)
        ax.set_thetamax(180)
        i=0

        #-------Plot--------
        for curve in [[radArr, distancesArr]]:
            ax.scatter(radArr, distancesArr, areaArr, "red", marker="o")
            ax.plot(curve[i], curve[i+1])
            i+=1

        plt.show(block=False)
        plt.pause(5) #wait for 5 seconds
        plt.close() #close the figure, otherwise, the python program will pause
        print("Figure close waiting for latest figure...")

    #-----Make sure to print out every time uart_sendStr has been called-------
    else:
        while True:
            receivedData = s.recv(64).decode()
            if(receivedData == "end"):
                break
            else:
                print(receivedData)
