#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<System.h>

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>

#include <sstream>
//#include <chrono>


using namespace cv;
using namespace std;

void WaitForInput();

int main(int argc, char* argv[]) {
    std::cout << "=====Start==============================" << std::endl;

    if(argc != 3)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    WaitForInput();
    std::cout << "=====Start Camera==============================" << std::endl;


    VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    WaitForInput();
    std::cout << "=====Stage 01==============================" << std::endl;

    if(!cap.isOpened()){
        cout << "Error opening video stream" << endl;
        return -1;
    }
    WaitForInput();
    std::cout << "=====Stage 02: IsOpened==============================" << std::endl;

    int index = 0;
    while(1){
        Mat frame;
        //std::cout << "=====Stage 03==============================" << index  << std::endl;

        // Capture frame-by-frame
        cap >> frame;
       // std::cout << "=====Stage 04==============================" << std::endl;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;
        //std::cout << "=====Stage 05: Is frame empty==============================" << index << std::endl;

        // Display the resulting frame
        imshow( "Live Video", frame );
        //std::cout << "=====Stage 06: imshow==============================" << std::endl;

        std::ostringstream filename;
        filename << "/home/jetson/ORB_SLAM2_2/Examples/Monocular/A_PNG/" << index << ".png";
        cv::imwrite(filename.str(), frame);

        // Press 'q' on keyboard to exit
        char c=(char)waitKey(25);
        if(c==27)  // ascii value for 'ESC' key is 27
            break;
        //std::cout << "=====Stage 07==============================" << std::endl;
        index++;
    }


    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    std::cout << "=====Start SLAM Setting==============================" << std::endl;
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);
    std::cout << "=====Wait for end SLAM==============================" << std::endl;
    WaitForInput();
    SLAM.Shutdown();
    std::cout << "=====End SLAM==============================" << std::endl;


    //SLAM.Shutdown();
    //sleep(10);
    //std::cout << "=====Stage 08==============================" << std::endl;


    // When everything done, release the video capture object
    cap.release();
    std::cout << "=====Stage 09: Camera Release==============================" << std::endl;
     WaitForInput();


    // Closes all the frames
    destroyAllWindows();

    std::cout << "End" << std::endl;

    return 0;
}


void WaitForInput()
{
    while (true) {
        std::cout << "Input: ";
        char c; std::cin >> c;
        if (c == 'n')
            break;
    }
}
