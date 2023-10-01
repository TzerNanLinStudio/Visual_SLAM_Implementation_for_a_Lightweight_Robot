import numpy as np
import cv2
import glob

objp = np.zeros((6*8,3), np.float32)
objp[:,:2] = np.mgrid[0:6,0:8].T.reshape(-1,2)

objpoints = [] 
imgpoints = [] 

images = glob.glob('OriginImage/*.png') 
count = 0 

for fname in images:
    img = cv2.imread(fname)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, corners = cv2.findChessboardCorners(gray, (6,8), None)

    if ret == True:
        objpoints.append(objp)
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        corners2 = cv2.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
        imgpoints.append(corners2)

        img = cv2.drawChessboardCorners(img, (6,8), corners2, ret)
        cv2.imwrite(f'CalibrationImage/{count}.png', img)
        cv2.imshow('Calibration Image', img)
        cv2.waitKey(1000)
        
        count += 1  

cv2.destroyAllWindows()
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

print("Camera Matrix : \n")
print(mtx, "\n")
print("Distortion : \n")
print(dist, "\n")
