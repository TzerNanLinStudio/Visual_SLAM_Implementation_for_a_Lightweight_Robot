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
#include <thread>
#include <atomic>

using namespace cv;
using namespace std;

std::atomic<bool> f1(true); // Keep running
std::atomic<bool> f2(false); // Show current timestamp

void WaitForInput();
void getUserInput();

int main(int argc, char* argv[]) {
    std::cout << "====================Start Program====================" << std::endl;

    if(argc != 3)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    std::cout << "====================Start SLAM Setting====================" << std::endl;
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);


    //cv::VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    cv::VideoCapture cap("nvarguscamerasrc sensor_mode=4 ! video/x-raw(memory:NVMM), width=1280, height=720, format=(string)NV12, framerate=(fraction)60/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    

    std::thread inputThread(getUserInput);    

    std::cout << std::fixed << std::setprecision(6); 

    while(true)
    {
	Mat img;
        cap >> img;

	// Get the current time
	auto now = std::chrono::system_clock::now();

	// Convert it to a time_point for the Epoch (1970-01-01 00:00:00)
	auto epoch = now.time_since_epoch();

	// Convert that to seconds
        std::chrono::duration<double> seconds = std::chrono::duration_cast<std::chrono::duration<double>>(epoch);

	// Convert to a double
	double tframe = seconds.count();
	
        if (f2) 
	    std::cout << "Frame Timestamp: " << tframe << std::endl;

        // Check if the image was successfully read
        if(img.empty())
        {
            std::cerr << "ERROR: Unable to read the image\n";
            return 1;
        }

	SLAM.TrackMonocular(img,tframe);

        usleep(5);

	if (!f1) 
	    break; 
    }
    std::cout << "====================End Loop====================" << std::endl;

    SLAM.Shutdown();
    std::cout << "====================End SLAM====================" << std::endl;

    // When everything done, release the video capture object
    cap.release();


    // Closes all the frames
    destroyAllWindows();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("Examples/Monocular/Test_A01/Code_03_KeyFrameTrajectory.txt");
    cout << "Save trajectory" << endl;

    // Save camera trajectory
    //SLAM.SaveTrajectoryTUM("CameraTrajectory_copy.txt");
    //cout << "Save trajectory" << endl;

    std::cout << "====================End Program====================" << std::endl;

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

void getUserInput() {
    char c;
    while (true) {
        std::cin >> c;
        if (c == 'q') {
            f1 = !f1;
            break;
        }
	if (c == 't') {
            f2 = !f2;
        }
    }
}
