// ref : https://docs.opencv.org/3.4/d4/d70/tutorial_hough_circle.html

#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std; 
using namespace cv; 

int main() 
{
    printf("Blob Detection\n");

    const char* imgPath = "./imgs/IMG_4907.JPG"; 
    Mat src = imread(samples::findFile(imgPath), IMREAD_COLOR); 
    if (src.empty()) { 
        printf("Failed to open image file\n : %s\n", imgPath); 
        return -1; 
    }
    printf("src witdh: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels());

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
                100,      // min distance 
                100, 30,    
                50, 100    // min radius, maxradius 
    ); 

    for (size_t i=0; i<circles.size(); i++) { 
        Vec3i c = circles[i]; 
        Point center = Point(c[0], c[1]); 
        // Circle center 
        circle(src, center, 1, Scalar(0, 255, 0), 2, LINE_AA); 
        // Circle outline 
        int radius = c[2]; 
        circle(src, center, radius, Scalar(0, 0, 255), 1, LINE_AA); 
    }

    imshow("origin", org);
    imshow("gray", gray);
    imshow("Circles", src); 
    waitKey(); 

    return 0;
}