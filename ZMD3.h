#ifndef ZMD3_H
#define ZMD3_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ZMD3
{
public:
	ZMD3(String fileNames[]);
	float computeWeight(Vec3b inputPixel);
};
#endif