import matplotlib.pyplot as plt


with open("KeyFrameTrajectory_copy.txt", "r") as file:
    lines = file.readlines()


x_translation = []
y_translation = []


for line in lines:
    data = line.strip().split() 
    x_translation.append(float(data[1]))
    y_translation.append(float(data[2]))


plt.figure()


plt.plot(x_translation, y_translation)

plt.title("SLAM Trajectory")
plt.xlabel("X Translation")
plt.ylabel("Y Translation")

plt.show()
