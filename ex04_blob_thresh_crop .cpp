// ref : https://docs.opencv.org/3.4/d4/d70/tutorial_hough_circle.html

#include <stdio.h>
#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std; 
using namespace cv; 

int threshold_value = 0; 
int threshold_type = 3; 
int const max_value = 255; 
int const max_type = 4; 
int const max_binary_value = 255; 

int bar_high = 0; 
int bar_low = 0;
int bar_max = 819;  

int bar_left = 0; 
int bar_right = 0; 
int bar_hMax = 1024; 

Mat src, src_gray, dst; 
const char* window_name = "Threshold Demo"; 

const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value"; 

static void Threshold_Demo(int, void*)
{
    /*  0: Binary
        1: Binary Inverted
        2: Threshold Truncated
        3: Threshold to Zero
        4: Threshold to Zero Inverted
    */
   threshold(src_gray, dst, threshold_value, max_binary_value, threshold_type); 
   imshow(window_name, dst); 
}

static void move_bar(int, void*)
{
    Mat cloned = dst.clone(); 
    line(cloned, Point(500, bar_high), Point(510, bar_high), Scalar(255, 255, 255), 3); 
    line(cloned, Point(500, bar_low), Point(510, bar_low), Scalar(255, 255, 255), 3);
    line(cloned, Point(bar_left, 400), Point(bar_left, 410), Scalar(255, 255, 255), 3); 
    line(cloned, Point(bar_right, 400), Point(bar_right, 410), Scalar(255, 255, 255), 3); 
    imshow(window_name, cloned); 
}


int main() 
{
    String imageName("./imgs/20230309_114849335_FAIL.JPG");
    src = imread(samples::findFile(imageName), IMREAD_COLOR); 
    if (src.empty()) { 
        cout << "Cannot read the image: " << imageName << endl; 
        return -1; 
    }
    printf("original src width: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels()); 
    resize(src, src, Size(0, 0), 0.4, 0.4); 
    printf("resized src width: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels()); 

    if (src.channels() == 3) { 
        cvtColor(src, src_gray, COLOR_BGR2GRAY); 
    } else { 
        src_gray = src.clone(); 
    }

    namedWindow(window_name, WINDOW_AUTOSIZE); 

    createTrackbar(trackbar_type, window_name, &threshold_type, max_type, Threshold_Demo); 
    createTrackbar(trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo); 
    createTrackbar("High bar", window_name, &bar_high, bar_max, move_bar); 
    createTrackbar("Low bar", window_name, &bar_low, bar_max, move_bar);
    createTrackbar("Left_bar", window_name, &bar_left, bar_hMax, move_bar); 
    createTrackbar("Right_bar", window_name, &bar_right, bar_hMax, move_bar);

    Threshold_Demo(0, 0); 
    move_bar(0, 0); 

    waitKey(); 

    return 0;
}