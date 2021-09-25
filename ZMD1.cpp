#include "ZMD1.h"

ZMD1::ZMD1(cv::String fileName) {
	Mat img(HEIGHT, WIDTH, CV_32FC3);
	img = imread(fileName);

	if (img.empty())
	{
		cout << "Error reading img " << fileName << endl;
		return;
	}

	imshow("RGB", img);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			float R = img.at<Vec3b>(i, j)[0];
			float G = img.at<Vec3b>(i, j)[1];
			float B = img.at<Vec3b>(i, j)[2];

			float Y = 0.299f * R + 0.587f * G + 0.114f * B;
			float U = -0.147f * R - 0.289f * G + 0.436f * B + 128;
			float V = 0.615f * R - 0.515f * G - 0.100f * B + 128;

			Vec3f yuvVector(Y, U, V);
			img.at<Vec3b>(i, j) = yuvVector;
		}
	}

	//Rozdělení na složky
	Mat channels[3];
	split(img, channels);

	Mat Y;
	Mat U, V = Mat::zeros(channels[0].rows, channels[0].cols, CV_8UC3);

	vector<Mat> channelsU;
	vector<Mat> channelsV;

	for (int i = 0; i < 3; i++) {
		channelsU.push_back(channels[1]);
		channelsV.push_back(channels[2]);
	}

	//Zobrazení Y U V kanálů
	merge(channelsU, U);
	merge(channelsV, V);

	cvtColor(U, U, COLOR_YUV2RGB);
	cvtColor(V, V, COLOR_YUV2RGB);


	imshow("Y", channels[0]);
	imshow("U", U);
	imshow("V", V);

	//Složení
	Mat fin;
	merge(channels, 3, fin);
	cvtColor(fin, fin, COLOR_YUV2RGB);

	imshow("Final", fin);

	
}