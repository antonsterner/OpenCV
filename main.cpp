/**
 * OpenCV SimpleBlobDetector Example
 *
 * Copyright 2015 by Satya Mallick <spmallick@gmail.com>
 *
 */

#include <iterator>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Camera.h"
#include "../linearalgebra.hh"


// using namespace cv;
using namespace std;


// Working blobdetection code, not class based
int main( int argc, char** argv )
{
    // read camera matrix and extrinsic parameters from out_camera_data.xml
    const string calibrationFile = argc > 1 ? argv[1] : "../out_camera_data.xml";
    FileStorage fs(calibrationFile , FileStorage::READ);
    if (!fs.isOpened())
    {
        cerr << "failed to open " << calibrationFile << endl;
        return 1;
    }
    Mat cameraMatrix, extrinsicParam;
    fs["camera_matrix"] >> cameraMatrix;
    fs["extrinsic_parameters"] >> extrinsicParam;
    fs.release();

    cout << "camera matrix : " << cameraMatrix << endl;

    // camera intrinsic parameter fx ( fx == fy ), calibrated at fullHD 1920x1080
    double fx = cameraMatrix.at<double>(0,0);
    double cx = cameraMatrix.at<double>(0,2);
    double cy = cameraMatrix.at<double>(1,2);

    // separate the rotation and translation of extrinsic parameters
    vector<double> rotvec, transvec;
    for( int i = 0; i < 3; i++)
    {
        double value = extrinsicParam.at<double>(0,i);
        rotvec.push_back(value);
    }

    for(int i = 3; i < 6; i++)
    {
        double value = extrinsicParam.at<double>(0,i);
        transvec.push_back(value);
    }

    Mat Rot(3,3, CV_32FC1);
    // Rodrigues transforms rotation vector from extrinsics to 3x3 array, stored in matrix Rot
    Rodrigues(rotvec, Rot, noArray());

    // invert y and z of Rot
    a3d::Matrix3d Mr(+Rot.at<double>(0,0), +Rot.at<double>(0,1), +Rot.at<double>(0,2),
                     -Rot.at<double>(1,0), -Rot.at<double>(1,1), -Rot.at<double>(1,2),
                     -Rot.at<double>(2,0), -Rot.at<double>(2,1), -Rot.at<double>(2,2));
    // quaternion rotation
    a3d::Quaterniond r = a3d::Quaterniond(Mr);
    cout << "quaternion r : " << r << endl;

    // rotate z with quaternion r
    a3d::Vector3d normal = r.rotate(a3d::Vector3d(0,0,1));
    cout << "normal of plane : " << normal << endl;


    //video capture container
    Mat frame;
    // keypoints container
    Mat im_with_keypoints;
    //video capture object.
    VideoCapture capture(1);

    if(!capture.isOpened()){
        cout << "ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }

    // set
    double Brightness, exposure, Contrast, Saturation, Gain;

    // Det här funkar bra som fan!
    capture.set(CAP_PROP_FRAME_WIDTH, 1920);
    capture.set(CAP_PROP_FRAME_HEIGHT, 1080);
    capture.set(CAP_PROP_GAIN, 0.0);
    capture.set(CAP_PROP_SATURATION, 255.0);
    capture.set(CAP_PROP_BRIGHTNESS, 100.0);
    capture.set(CAP_PROP_EXPOSURE, -5.0);
    capture.set(CAP_PROP_CONTRAST, 255.0);

    Brightness = capture.get(CAP_PROP_BRIGHTNESS);
    Contrast   = capture.get(CAP_PROP_CONTRAST );
    Saturation = capture.get(CAP_PROP_SATURATION);
    Gain       = capture.get(CAP_PROP_GAIN);
    exposure   = capture.get(CAP_PROP_EXPOSURE);

    cout<<"===================================="<<endl<<endl;
    cout<<"Default Brightness--------> "<<Brightness<<endl;
    cout<<"Default Contrast----------> "<<Contrast<<endl;
    cout<<"Default Saturation--------> "<<Saturation<<endl;
    cout<<"Default Gain--------------> "<<Gain<<endl<<endl;
    cout<<"Default Exposure----------> "<<exposure<<endl<<endl;
    cout<<"===================================="<<endl;

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

    capture >> frame;
    cout << "frame size: " << frame.size() << endl;
    while(true){

        // read frame
        capture >> frame;

        // Detect blobs
        detector->detect( frame, keypoints );

        for(int i = 0; i < keypoints.size(); i++) {
            X = keypoints[i].pt.x;
            Y = keypoints[i].pt.y;
        }

        // Calculate projection vector from camera
        vector<double> v;
        v.push_back((X - cx)/fx);
        v.push_back((Y - cy)/fx);
        v.push_back(1);
/*
        cout << "v : ";
        for(int i = 0; i < 3; i++)
        {
            cout << v[i] << endl;
        }
*/
        a3d::Vector3d v2, transvec2, intersection;

        for(auto i : v){
            v2[i] = v[i];
            transvec2[i] = transvec[i];
        }

        // calculate intersection of normal vector and projection vector
        // calculate d = length of line intersecting plane
        double d;
        v2 = v2.normalized();
        d = (transvec2 * normal)/(v2 * normal);
        intersection = d * v2;

        cout << "intersection: " << intersection << endl;



        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob
        drawKeypoints( frame, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        // Show blobs
        namedWindow("keypoints", 0 );
        imshow("keypoints", im_with_keypoints );
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
}


/* // [R|T] matrix with rotation and translation from extrinsic values
 *
cv::Mat componentMat ( transvec, true );
std::cout << componentMat << endl;

Mat RT(3,4, CV_32FC1);

hconcat(Rot, componentMat, RT);

cout << "[R|T] : " << RT << endl;


cout << "transvec: " <<  endl;
for(int i = 0; i < 3; i++)
{
    cout << transvec[i] << " ";
}*/

/*
// Class based main code, not working atm
// Reading camera parameters from XML to compute normal

int main( int argc, char** argv ) {

    const string calibrationFile = argc > 1 ? argv[1] : "../out_camera_data.xml";
    FileStorage fs(calibrationFile , FileStorage::READ);
    if (!fs.isOpened())
    {
        cerr << "failed to open " << calibrationFile << endl;
        return 1;
    }
    Mat cameraMatrix, distCoeffs, avgProjError, perViewReprojError, extrinsicParam, imagePoints;
    fs["camera_matrix"] >> cameraMatrix;
    fs["extrinsic_parameters"] >> extrinsicParam;
    fs.release();


    vector<double> rotvec, transvec;
    for( int i = 0; i < 3; i++)
    {
        double value = extrinsicParam.at<double>(0,i);
        rotvec.push_back(value);
    }

    for(int i = 3; i < 6; i++)
    {
       double value = extrinsicParam.at<double>(0,i);
       transvec.push_back(value);
    }

    Mat Rot(3,3, CV_32FC1);
    Rodrigues(rotvec, Rot, noArray());


    cv::Mat componentMat ( transvec, true );
    std::cout << componentMat << endl;

    Mat RT(3,4, CV_32FC1);

    hconcat(Rot, componentMat, RT);

    cout << "[R|T] : " << RT << endl;


    cout << "transvec: " <<  endl;
    for(int i = 0; i < 3; i++)
    {
        cout << transvec[i] << " ";
    }

    a3d::Matrix3d Mr(+Rot.at<double>(0,0), +Rot.at<double>(0,1), +Rot.at<double>(0,2),
                     -Rot.at<double>(1,0), -Rot.at<double>(1,1), -Rot.at<double>(1,2),
                     -Rot.at<double>(2,0), -Rot.at<double>(2,1), -Rot.at<double>(2,2));

    a3d::Quaterniond r = a3d::Quaterniond(Mr);
    cout << "quaternion r : " << r << endl;

    a3d::Vector3d normal = r.rotate(a3d::Vector3d(0,0,1));

    cout << "normal of plane : " << normal << endl;

    //a3d::Vector3d floor_normal = r * a3d::Vector3d(0,0,1);

    return 0;




    WebCam cam1;
    vector<KeyPoint> pos;


    while(true) {

        // store sphero pos in vector
        cam1.updateCamera();

        // save keypoints
        Mat im_with_keypoints;

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob
        drawKeypoints( cam1.frame, cam1.keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        // Show blobs
        namedWindow("keypoints", 0 );
        imshow("keypoints", im_with_keypoints );
        //resizeWindow("keypoints", 1920, 1080);
        //imshow("binary image", binarizedImage);
        waitKey(1);

        bool pause = false;

        switch (waitKey(10)) {

            case 27: //'esc' key has been pressed, exit program.
                //delete cam1;
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


}*/
