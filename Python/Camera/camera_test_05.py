import cv2
import os
import time
import concurrent.futures

if __name__ == "__main__":
    cap = cv2.VideoCapture('nvarguscamerasrc sensor_mode=4 ! video/x-raw(memory:NVMM), width=1280, height=720, format=(string)NV12, framerate=(fraction)60/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink')

    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter('output.avi', fourcc, 60.0, (1280, 720))

    start_time = time.time()
    while(int(time.time() - start_time) < 30):
        ret, frame = cap.read()
        if ret==True:
            out.write(frame)
        else:
            break

    cap.release()
    out.release()

    # 2

    cap = cv2.VideoCapture('output.avi')


    frame_counter = 0

    while(cap.isOpened()):
        ret, frame = cap.read()
	    
        if ret == True:
            cv2.imwrite(f'frame{frame_counter}.jpg', frame)
            frame_counter += 1
        else:
            break

    cap.release()
    cv2.destroyAllWindows()
