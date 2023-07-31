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


using namespace cv;
using namespace std;

#include <thread>
#include <atomic>

std::atomic<bool> keepRunning(true);

void WaitForInput();

void getUserInput() {
    char x;
    while (true) {
        std::cin >> x;
        if (x == 'x') {
            keepRunning = false;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=====Start==============================" << std::endl;

    if(argc != 3)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    // Initialize the image counter
    int img_counter = 20;

    // Assume we know the number of images
    int num_images = 300;

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    std::cout << "=====Start SLAM Setting==============================" << std::endl;
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);


    std::cout << "=====Start Load Images==============================" << std::endl;
    WaitForInput();

	//VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    cv::VideoCapture cap("nvarguscamerasrc sensor_mode=4 ! video/x-raw(memory:NVMM), width=1280, height=720, format=(string)NV12, framerate=(fraction)60/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    

    std::thread inputThread(getUserInput);    

    std::cout << std::fixed << std::setprecision(6); 

    //while(img_counter < num_images)
    while(1)
    {
	Mat img;
        cap >> img;



	// Get the current time
	auto now = std::chrono::system_clock::now();

	// Convert it to a time_point for the Epoch (1970-01-01 00:00:00)
	auto epoch = now.time_since_epoch();

	// Convert that to seconds
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);

	// Convert to a double
	double tframe = seconds.count();
	std::cout << "tframe: " << tframe << std::endl;

        // Check if the image was successfully read
        if(img.empty())
        {
            std::cerr << "ERROR: Unable to read the image\n";
            return 1;
        }

	SLAM.TrackMonocular(img,tframe);

        sleep(1);

	if (!keepRunning) break;

        img_counter++;  
    }
    std::cout << "=====End Load Images==============================" << std::endl;

    SLAM.Shutdown();
    std::cout << "=====End SLAM==============================" << std::endl;

    // When everything done, release the video capture object
    cap.release();


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
