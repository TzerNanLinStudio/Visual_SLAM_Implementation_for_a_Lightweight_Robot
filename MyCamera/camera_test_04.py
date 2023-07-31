import cv2
import os
import time
import concurrent.futures

def save_frame(frame, output_path):
    cv2.imwrite(output_path, frame)

if __name__ == "__main__":
    cap = cv2.VideoCapture('nvarguscamerasrc sensor_mode=4 ! video/x-raw(memory:NVMM), width=1280, height=720, format=(string)NV12, framerate=(fraction)60/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink')

    output_dir = "image"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    frame_counter = 0
    frame_counter_last_sec = 0
    start_time = time.time()
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame. Exiting ...")
            break

        cv2.imwrite(f'/home/jetson/ORB_SLAM2_2/Examples/Monocular/A1_Test_01/image/{frame_counter}.png',frame)
        frame_counter += 1

        # if one second has passed
        if time.time() - start_time >= 1:
            print(f"Saved {frame_counter - frame_counter_last_sec} frames in the last second.")
            frame_counter_last_sec = frame_counter
            start_time = time.time()    

    #with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
        

            #cv2.imshow('Live Video', frame) 
            #if cv2.waitKey(1) == ord('q'): 
                #break

    cap.release()
    cv2.destroyAllWindows()
