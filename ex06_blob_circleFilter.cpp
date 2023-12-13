// ref : https://docs.opencv.org/3.4/d4/d70/tutorial_hough_circle.html

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <list>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std; 
using namespace cv; 

const char* winname = "Compare Threshold"; 

bool less_than_distance(Point p1, Point p2, double creterion);
bool is_finded_index(size_t index_i, vector<size_t> indexes); 

int main() 
{
    Mat src_gray; 
    Mat normal_th; 

    String imageName("./imgs/IMG_4908.JPG");
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
    Mat color_src2 = src.clone(); 

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
                1, 100
    );
    
    printf("circles.size(): %d\n", (int)circles.size());

    list<vector<Vec3i>> clusters; 
    vector<size_t> finded_index; 
    int count = 0; 
    for (size_t i=0; i<circles.size()-1; i++) { 
        Vec3i cI = circles[i]; 
        Point centerI = Point(cI[0], cI[1]);
        int rI = cI[2]; 

        circle(color_src2, centerI, 1, Scalar(0, 255, 0), 2, LINE_AA);
        circle(color_src2, centerI, rI, Scalar(0, 255, 0), 1, LINE_AA);

        if (is_finded_index(i, finded_index)) {
            continue; 
        }
        vector<Vec3i> cluster; 
        cluster.push_back(cI); 
        finded_index.push_back(i);
        count++; 

        Vec3i biggest = cI;
        for (size_t j=0; j<circles.size(); j++) { 
            if (is_finded_index(j, finded_index)) {
                continue; 
            }

            Vec3i cJ = circles[j]; 
            Point centerJ = Point(cJ[0], cJ[1]); 

            if (less_than_distance(centerI, centerJ, 10)) { 
                cluster.push_back(cJ);
                finded_index.push_back(j); 
            }            
        }

        clusters.push_back(cluster); 
    }
    printf("clusters.size(): %d\n", (int)clusters.size());

    // vector<size_t>::iterator iter_find; 
    // for (iter_find = finded_index.begin(); iter_find != finded_index.end(); iter_find++) { 
    //     printf("finded_index: %d\n", int(*iter_find));
    // }

    int cluster_no = 0; 
    int inc = 0; 
    list<vector<Vec3i>>::iterator iter_list; 
    for (iter_list = clusters.begin(); iter_list != clusters.end(); iter_list++) { 

        vector<Vec3i>::iterator iter_v = (*iter_list).begin(); 

        Vec3i biggest = *iter_v;
        int xb = biggest[0]; 
        int yb = biggest[1];  
        int bigR = biggest[2];

        char str[16]; 
        snprintf(str, 16, "#%d(%d, %d)", cluster_no, xb ,yb);
        putText(src_gray, str, Point(xb, yb + inc), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
        inc -= 15; 

        iter_v++;
        for (; iter_v != (*iter_list).end(); iter_v++) { 
            if (bigR < int((*iter_v)[2])) { 
                bigR = int((*iter_v)[2]); 
                biggest = *iter_v; 

            }
        }

        int xp = biggest[0]; 
        int yp = biggest[1]; 
        int r = biggest[2]; 
        
        circle(color_src, Point(xp, yp), 1, Scalar(0, 255, 0), 2); 
        circle(color_src, Point(xp, yp), r, Scalar(0, 255, 0)); 

    }

    imshow("src2", color_src2);
    imshow("src", color_src); 
    imshow("threshold", normal_th);
    imshow("src_gray", src_gray);
    

    waitKey(); 

    destroyAllWindows(); 
    return 0;
}

bool less_than_distance(Point p1, Point p2, double creterion) 
{
    if (p1.x == p2.x && p1.y == p2.y) 
        return true; 
    double sq = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    return sq <= creterion; 
}

bool is_finded_index(size_t index_i, vector<size_t> indexes) 
{
    vector<size_t>::iterator iter; 
    for (iter = indexes.begin(); iter != indexes.end(); iter++) {
        if (index_i == *iter) {
            return true; 
        }
    }
    return false; 
}