// ref : https://docs.opencv.org/3.4/d4/d70/tutorial_hough_circle.html

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <queue> 

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std; 
using namespace cv; 

const char* winname = "Compare Threshold"; 

double diff_distance(Point p1, Point p2, double creterion);

int main() 
{
    Mat src_gray; 
    Mat normal_th; 

    String imageName("./imgs/20230309_114849335_FAIL.JPG");
    Mat src = imread(samples::findFile(imageName), IMREAD_COLOR); 
    if (src.empty()) { 
        cout << "Cannot read the image: " << imageName << endl; 
        return -1; 
    }
    printf("original src width: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels()); 
    resize(src, src, Size(0, 0), 0.4, 0.4); 
    printf("resized src width: %d, height: %d, channels: %d\n", src.cols, src.rows, src.channels()); 

    src = src(Range(240, 560), Range(130, 930)); 
    imshow("src crop", src);

    Mat color_src = src.clone();

    if (src.channels() == 3) { 
        cvtColor(src, src_gray, COLOR_BGR2GRAY); 
    } else { 
        src_gray = src.clone(); 
    }
    
    threshold(src_gray, normal_th, 100, 255, THRESH_BINARY_INV);
    
    vector<Vec3f> circles; 
    HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, 
                1, 
                100, 30, 
                5, 100
    );

    vector<Vec3i> bigCircles; 
    for (size_t i=0; i<circles.size(); i++) { 
        Vec3i c = circles[i]; 
        Point center = Point(c[0], c[1]); 
        int radius = c[2]; 

        if (i > 1) { 
            double dist = diff_distance(Point(circles[i-1][0], circles[i-1][1]), Point(circles[i][0], circles[i][1]), 10);
            if (dist < 10) { 
                int r1 = circles[i-1][2]; 
                int r2 = circles[i][2]; 
            } 


        }

        // // Circle center 
        // circle(color_src, center, 1, Scalar(0, 0, 255), 5, LINE_AA); 
        // // Circle outline 
        // circle(color_src, center, radius, Scalar(0, 0, 255), 1, LINE_AA); 
    }
    printf("circles.size(): %d\n", circles.size());
    while (pQueue.empty()) {
        size_t qsize = pQueue.size(); 
        for (size_t i=0; i<qsize; i++) { 
            if (i > 0) { 
                Point p1 = pQueue.front(); 
                Point p2 = pQueue.front(); 
            }
        }
    }

    imshow("src", color_src); 
    imshow("threshold", normal_th);
    imshow("src_gray", src_gray);
    

    waitKey(); 

    destroyAllWindows(); 
    return 0;
}

double diff_distance(Point p1, Point p2, double creterion) 
{
    double sq = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    return abs(creterion - sq); 
}