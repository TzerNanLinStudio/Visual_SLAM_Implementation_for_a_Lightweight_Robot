import cv2
import os

if __name__ == "__main__":
    print("\nThe program is starting\n")

    cap = cv2.VideoCapture('nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink')

    image_count = 0  

    print("\nThe loop is starting\n")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
        cv2.imshow('Live Video', frame) 

        key = cv2.waitKey(1)
        if key == ord('q'): 
            break
        elif key == ord('m'):  
            cv2.imwrite(f'Picture/{image_count}.png', frame)
            print(f'Saved Picture/{image_count}.png')
            image_count += 1  

    cap.release()
    cv2.destroyAllWindows()

    print("\nThe program is ending.\n")
