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

std::atomic<bool> flag(true); 

void getUserInput();

int main(int argc, char* argv[]) {
    if(argc != 3)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);

    cv::VideoCapture cap(0);
    
    std::thread inputThread(getUserInput);    

    while(true)
    {
	Mat img;
        cap >> img;

	auto now = std::chrono::system_clock::now();
	auto epoch = now.time_since_epoch();
        std::chrono::duration<double> seconds = std::chrono::duration_cast<std::chrono::duration<double>>(epoch);
	double tframe = seconds.count();
	
        if(img.empty())
        {
            std::cerr << "ERROR: Unable to read the image\n";
            return 1;
        }

	SLAM.TrackMonocular(img,tframe);

        usleep(5);

	if (!flag) 
	    break; 
    }

    SLAM.Shutdown();

    cap.release();

    destroyAllWindows();

    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    inputThread.join();

    return 0;
}

void getUserInput() {
    char c;
    while (true) {
        std::cin >> c;
        if (c == 'q') {
            flag = !flag;
            break;
        }
    }
}
