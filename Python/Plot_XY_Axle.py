import matplotlib.pyplot as plt


#with open("KeyFrameTrajectory_copy.txt", "r") as file:
with open("z3.txt", "r") as file:
    lines = file.readlines()

timestamps = [] 
x_translation = []
y_translation = []
z_translation = []
x_rotation = []
y_rotation = []
z_rotation = []
w_rotation = []

for line in lines:
    data = line.strip().split() 
    timestamps.append(float(data[0])) 
    x_translation.append(float(data[1]))
    y_translation.append(float(data[2]))
    z_translation.append(float(data[3]))
    x_rotation.append(float(data[4]))
    y_rotation.append(float(data[5]))
    z_rotation.append(float(data[6]))
    w_rotation.append(float(data[7]))


plt.figure()
plt.plot(y_translation, z_translation)
plt.title("SLAM Trajectory")
plt.xlabel("X Translation")
plt.ylabel("Y Translation")
plt.show()

plt.figure()
plt.plot(timestamps, x_translation)  
plt.title("X Translation Over Time")
plt.xlabel("Time")
plt.ylabel("X Translation")
plt.show()

plt.figure()
plt.plot(timestamps, y_translation)  
plt.title("Y Translation Over Time")
plt.xlabel("Time")
plt.ylabel("Y Translation")
plt.show()

plt.figure()
plt.plot(timestamps, z_translation)  
plt.title("Z Translation Over Time")
plt.xlabel("Time")
plt.ylabel("Z Translation")
plt.show()

plt.figure()
plt.plot(timestamps, x_rotation)  
plt.title("X Rotation Over Time")
plt.xlabel("Time")
plt.ylabel("X Rotation")
plt.show()

plt.figure()
plt.plot(timestamps, y_rotation)  
plt.title("Y Rotation Over Time")
plt.xlabel("Time")
plt.ylabel("Y Rotation")
plt.show()

plt.figure()
plt.plot(timestamps, z_rotation)  
plt.title("Z Rotation Over Time")
plt.xlabel("Time")
plt.ylabel("Z Rotation")
plt.show()

plt.figure()
plt.plot(timestamps, w_rotation)  
plt.title("W Rotation Over Time")
plt.xlabel("Time")
plt.ylabel("W Rotation")
plt.show()


