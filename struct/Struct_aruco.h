#ifndef STRUCT_ARUCO_H
#define STRUCT_ARUCO_H

#endif // STRUCT_ARUCO_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace std;

struct Str_Aruco{
    int index;
    int on_member_number;
    cv::Mat rotationMatrix;
    Str_Aruco(int index): index(index), {};
};
