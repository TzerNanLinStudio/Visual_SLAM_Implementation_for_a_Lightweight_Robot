import numpy as np

def read_data_from_txt(file_path):
    points = []
    with open(file_path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            data = line.split()
            point = (float(data[1]), float(data[2]), float(data[3]))  # Extract the x, y, z values
            points.append(point)
    return points

def calculate_ate(points):
    start_point = np.array(points[0])
    end_point = np.array(points[-1])

    line_vector = end_point - start_point

    distances = []
    for point in points:
        point = np.array(point)
        point_vector = point - start_point

        t = np.dot(line_vector, point_vector) / np.dot(line_vector, line_vector)
        projection = start_point + t * line_vector
        distance = np.linalg.norm(point - projection)
        distances.append(distance)

    ate = sum(distances) / len(distances)
    return ate

file_path = 'x14.txt'  
points = read_data_from_txt(file_path)
ate = calculate_ate(points)
print(f"Absolute Trajectory Error (ATE) is: {ate}")
