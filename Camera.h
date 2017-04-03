//
// Created by anton sterner on 2017-03-22.
//

#ifndef SIMPLEBLOBDETECTION_BLOBDETECTOR_H
#define SIMPLEBLOBDETECTION_BLOBDETECTOR_H

#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;


class WebCam {

public:

    //video capture container
    Mat frame;
    //video capture object.
    VideoCapture capture;
    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;
    // Detector pointer
    Ptr<SimpleBlobDetector> detector;
    // Storage for blobs
    vector<KeyPoint> keypoints;

    WebCam();


    void updateCamera();

};


#endif //SIMPLEBLOBDETECTION_BLOBDETECTOR_H
