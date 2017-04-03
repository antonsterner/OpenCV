/**
 * OpenCV SimpleBlobDetector Example
 *
 * Copyright 2015 by Satya Mallick <spmallick@gmail.com>
 *
 */

#include <iterator>
#include <iostream>
#include <cmath>
#include "opencv2/opencv.hpp"
#include "Camera.h"



using namespace cv;
using namespace std;
/*
bool compare(float prevx[], float prevy[], float X, float Y);

int main( int argc, char** argv )
{

    //video capture container
    Mat frame;
    //video capture object.
    VideoCapture capture(1);

    if(!capture.isOpened()){
        cout << "ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }
    //capture.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,720);

    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

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


    // Storage for blobs
    vector<KeyPoint> keypoints;

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    bool pause = false;
    float X = 0;
    float Y = 0;
    float prevx[2], prevy[2] = {0};

    // read frame
    capture >> frame;


    // Detect blobs
    detector->detect( frame, keypoints );
    // Store x and y keypoints in two arrays
    for(int i = 0; i < keypoints.size(); i++){
        prevx[i] = keypoints[i].pt.x;
        prevy[i] = keypoints[i].pt.y;
      //  cout << i << ": " << X << ' ' << Y ;
    }

    while(true){

        // read frame
        capture >> frame;

        // Detect blobs
        detector->detect( frame, keypoints );

        for(int i = 0; i < keypoints.size(); i++){
            X = keypoints[i].pt.x;
            Y = keypoints[i].pt.y;

            if(!compare(prevx, prevy, X, Y)){
                prevx[0] = X;
                prevy[0] = Y;
            }
            else{
                prevx[1] = X;
                prevy[1] = Y;
            }

           // cout << i << ": " << X << ' ' << Y << endl;
        }
       // cout << round(prevx[0]) << ' ' << round(prevy[0]) << ' ';
        //cout << round(prevx[1]) << ' ' << round(prevy[1]) ;
        //cout << endl;


        // save keypoints
        Mat im_with_keypoints;

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob
        drawKeypoints( frame, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        // Show blobs
        namedWindow("keypoints", 0 );
        imshow("keypoints", im_with_keypoints );
        //resizeWindow("keypoints", 1920, 1080);
        //imshow("binary image", binarizedImage);
        waitKey(1);

        switch(waitKey(10)){

            case 27: //'esc' key has been pressed, exit program.
                return 0;
            case 112: //'p' has been pressed. this will pause/resume the code.
                pause = !pause;
                if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
                    while (pause == true){
                        //stay in this loop until
                        switch (waitKey()){
                            //a switch statement inside a switch statement? Mind blown.
                            case 112:
                                //change pause back to false
                                pause = false;
                                cout<<"Code resumed."<<endl;
                                break;
                        }
                    }
                }


        }

    }

}*/
/*
bool compare(float prevx[], float prevy[], float X, float Y){
    double dist1 = sqrt(pow(double(X - prevx[0]), 2.0) + pow(double(Y - prevy[0]),2.0));
    double dist2 = sqrt(pow(double(X - prevx[1]), 2.0) + pow(double(Y - prevy[1]),2.0));
    if(dist1 < dist2){
       // cout << dist1 << " less than " << dist2 << endl;
        return true;
    }
    //cout << dist1 << " more than " << dist2 << endl;
    return false;

}

*/

int main( int argc, char** argv ) {


    WebCam* cam1 = new WebCam();
    vector<KeyPoint> pos;


    while(true) {

        // store sphero pos in vector
        cam1->updateCamera();

        // save keypoints
        Mat im_with_keypoints;

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob
        drawKeypoints( cam1->frame, cam1->keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        // Show blobs
        namedWindow("keypoints", 0 );
        imshow("keypoints", im_with_keypoints );
        //resizeWindow("keypoints", 1920, 1080);
        //imshow("binary image", binarizedImage);
        waitKey(1);

        bool pause = false;

        switch (waitKey(10)) {

            case 27: //'esc' key has been pressed, exit program.
                delete cam1;
                return 0;
            case 112: //'p' has been pressed. this will pause/resume the code.
                pause = !pause;
                if (pause == true) {
                    cout << "Code paused, press 'p' again to resume" << endl;
                    while (pause == true) {
                        //stay in this loop until
                        switch (waitKey()) {
                            //a switch statement inside a switch statement? Mind blown.
                            case 112:
                                //change pause back to false
                                pause = false;
                                cout << "Code resumed." << endl;
                                break;
                        }
                    }
                }


        }
    }
}