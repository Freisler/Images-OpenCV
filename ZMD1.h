#ifndef ZMD1_H
#define ZMD1_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#define WIDTH 300
#define HEIGHT 200

using namespace std;
using namespace cv;

class ZMD1
{
public:
	ZMD1(cv::String fileName);
};
#endif