//
// Created by anton sterner on 2017-03-22.
//

#include "Camera.h"

using namespace cv;
using namespace std;

WebCam::WebCam(){
    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 255;

    // Filter by color
    params.filterByColor = false;
    params.blobColor = 255;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 200;
    params.maxArea = 100000;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.75;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = false;
    params.minInertiaRatio = 0.01;

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    //video capture object.
    VideoCapture capture(0);

    if(!capture.isOpened()){
        cout << "ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return;
    }
};


void WebCam::updateCamera(){

    // read frame
    capture >> frame;

    // Detect blobs
    detector->detect( frame, keypoints );

};





