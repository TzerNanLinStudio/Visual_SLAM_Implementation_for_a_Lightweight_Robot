#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>


using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    std::cout << "Start!" << std::endl;

    VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");

    
    //
    if(!cap.isOpened()){
        cout << "Error opening video stream" << endl;
        return -1;
    }
   
   while(1){

        Mat frame;

        // Capture frame-by-frame
        cap >> frame;
  
        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Display the resulting frame
        imshow( "Live Video", frame );

        // Press 'q' on keyboard to exit
        char c=(char)waitKey(25);
        if(c==27){  // ascii value for 'ESC' key is 27
            std::cout << "Esc" << std::endl;
            break;
        }
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    std::cout << "End" << std::endl;

    return 0;
}
