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

int main(int argc, char **argv)
{
    std::cout << "=====Start==============================" << std::endl;

    if (argc != 3) {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    std::cout << "=====Start Camera==============================" << std::endl;

    VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=3280, height=2464, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");

    if (!cap.isOpened()) {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    std::cout << std::fixed << std::setprecision(6);

    vector<double> vTimestamps0; // Vector to store tframe2 values
    int index = 0;
    while (index < 500) { // Capture 400 frames
        Mat frame;

        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

	imshow( "Live Video", frame );

	if (index > 50) {
	    std::ostringstream filename;
            auto now = std::chrono::system_clock::now();
            auto epoch = now.time_since_epoch();
            std::chrono::duration<double> seconds = std::chrono::duration_cast<std::chrono::duration<double>>(epoch);
            double tframe2 = seconds.count();
            filename << "/home/jetson/ORB_SLAM2_2/Examples/Monocular/A1_Test_01/image/" << std::fixed << std::setprecision(6) << tframe2 << ".png"; // Save image with timestamp as filename
            cv::imwrite(filename.str(), frame);
	    vTimestamps0.push_back(tframe2);
	
            std::cout << "index: " << index << " ; tframe2: " << tframe2 << std::endl;
	}
	else {
	    std::cout << "index: " << index << std::endl;	
	}
	

	char c=(char)waitKey(5);
        if(c==27)  // ascii value for 'ESC' key is 27
            break;

        index++;
    }

    // Release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    /*std::cout << "=====End Camera and Start Video==============================" << std::endl;

    // Show the saved images
    for (int i = 0; i < vTimestamps0.size(); i++) {
        std::ostringstream filename;
        filename << "/home/jetson/ORB_SLAM2_2/Examples/Monocular/A1_Test_01/image/" << std::fixed << std::setprecision(6) << std::to_string(vTimestamps0[i]) << ".png";

        Mat image = imread(filename.str(), IMREAD_UNCHANGED);
        if (image.empty()) {
            std::cout << "Image not found: " << filename.str() << std::endl;
            continue;
        }

        imshow("Live Video 2", image);
        char c = (char)waitKey(5);
        if (c == 27) // Press 'ESC' key to exit
            break;
    }

    // Closes all the frames
    destroyAllWindows();*/

    std::cout << "=====End Video and Decide SLAM or not==============================" << std::endl;

	char c; std::cin >> c;
        if (c == 'n')
        {
            int nImages =  vTimestamps0.size();

	    // Create SLAM system. It initializes all system threads and gets ready to process frames.
	    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);


	    // Vector for tracking time statistics
    	    vector<float> vTimesTrack;
    	    vTimesTrack.resize(nImages);


    	    cout << "Start processing sequence ..." << endl;


		    // std::cout << std::fixed << std::setprecision(6); 

		    // Main loop
		    for(int ni=0; ni<nImages; ni++)
		    {
			// Read image from file
			std::ostringstream filename;
        		filename << "/home/jetson/ORB_SLAM2_2/Examples/Monocular/A1_Test_01/image/" << std::fixed << std::setprecision(6) << std::to_string(vTimestamps0[ni]) << ".png";

        Mat image = imread(filename.str(), IMREAD_UNCHANGED);
			double tframe = vTimestamps0[ni];

			cout << "ni: " << ni << endl;

			if(image.empty())
			{
			    cerr << endl << "Failed to load image at: " << endl;
			    return 1;
			}

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

			// Pass the image to the SLAM system
			SLAM.TrackMonocular(image,tframe);


#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

			double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

			vTimesTrack[ni]=ttrack;

			// Wait to load the next frame
			double T=0;
			if(ni<nImages-1)
			    T = vTimestamps0[ni+1]-tframe;
			else if(ni>0)
			    T = tframe-vTimestamps0[ni-1];

			if(ttrack<T)
			    usleep((T-ttrack)*1e6);
		    }

		    // Stop all threads
		    SLAM.Shutdown();
	}

    return 0;
}

