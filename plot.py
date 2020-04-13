import matplotlib.pyplot as plt
import serial
import numpy as np

time=np.arange(0,100)            #for plotting X axes 
X=np.arange(0,100)
Y=np.arange(0,100)
Z=np.arange(0,100)   #for storing every accel value in 10 sec
tilt = np.arange(0,100)

serdev = '/dev/ttyACM1'
s = serial.Serial(serdev, baudrate=115200)

for i in range(0, 100):
    print("i="+str(i)+"\n")
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    X[i] = float(line)
    line=s.readline() 
    Y[i] = float(line)
    line=s.readline() 
    Z[i] = float(line)
    line=s.readline() 
    tilt[i] = float(line)
    print("X="+ str(X[i])+"Y="+str(Y[i])+"Z="+str(Z[i]))

fig, ax = plt.subplots(2, 1)
ax[0].plot(time, X, color="blue", label='X')
ax[0].plot(time, Y, color="red", label='Y')
ax[0].plot(time, Z, color="green", label='Z')
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Accel value')

for i in range(0,100):
    if tilt[i]==1:
        print("i="+str(i)+"tilt")
        ax[1].plot([i,i],[0,1],color="blue", linestyle='--') # plotting the spectrum
        ax[1].scatter([i,],[1,],50, color="blue")
# t =2*np.pi/3
# ax[1].plot([t,t],[0,np.cos(t)],color ='blue',  linewidth=1.5, linestyle="--")
ax[1].set_xlabel('Time')
ax[1].set_ylabel('Tilt')

plt.show()

s.close()
