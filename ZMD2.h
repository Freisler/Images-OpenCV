#ifndef ZMD2_H
#define ZMD2_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ZMD2
{
public:
	ZMD2(String fileName);
	uint8_t getAvgR(char lookingFor, int r, int c, Mat img);
	uint8_t getAvgG1(char lookingFor, int r, int c, Mat img);
	uint8_t getAvgG2(char lookingFor, int r, int c, Mat img);
	uint8_t getAvgB(char lookingFor, int r, int c, Mat img);
};
#endif