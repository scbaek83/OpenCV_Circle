// ref : https://docs.opencv.org/3.4/d4/d70/tutorial_hough_circle.html

#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std; 
using namespace cv; 

int main() 
{
    printf("Blob Detection\n");

    const char* imgPath = "./imgs/20230309_133206863_PASS.JPG"; 
    Mat src = imread(samples::findFile(imgPath), IMREAD_COLOR); 
    if (src.empty()) { 
        printf("Failed to open image file\n : %s\n", imgPath); 
        return -1; 
    }
    printf("src witdh: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels());

    resize(src, src, Size(0, 0), 0.4, 0.4); 
    printf("resized src width: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels()); 

    Mat org = src.clone(); 

    Mat gray; 
    if (src.channels() == 3) { 
        cvtColor(src, gray, COLOR_BGR2GRAY); 
    } else { 
        gray = src.clone(); 
    }

    // medianBlur(gray, gray, 3); 

    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                50,      // min distance 
                100, 30,    
                10, 50    // min radius, maxradius 
    ); 

    for (size_t i=0; i<circles.size(); i++) { 
        Vec3i c = circles[i]; 
        Point center = Point(c[0], c[1]); 
        // Circle center 
        circle(src, center, 1, Scalar(0, 255, 0), 2, LINE_AA); 
        // Circle outline 
        int radius = c[2]; 
        circle(src, center, radius, Scalar(0, 0, 255), 1, LINE_AA); 

        char str[32]; 
        snprintf(str, 32, "x:%d y:%d r:%d",c[0], c[1], radius); 
        putText(src, str, Point(c[0] -25, c[1] - radius-10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }

    line(src, Point(20, 200), Point(30, 200), Scalar(0, 255, 0), 3);
    line(src, Point(20, 600), Point(30, 600), Scalar(0, 255, 0), 3);

    imshow("origin", org);
    imshow("gray", gray);
    imshow("Circles", src); 
    waitKey(); 

    return 0;
}