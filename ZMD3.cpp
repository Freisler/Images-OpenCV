#include "ZMD3.h"

#define MU 0.5
#define SIGMA 0.2

ZMD3::ZMD3(cv::String fileNames[]) {

	vector<Mat> inputImgs;

	for (int i = 0; i < 5; i++) {
		inputImgs.push_back(imread(fileNames[i], IMREAD_COLOR));
		imshow(to_string(i), inputImgs[i]);
	}

	Mat outputRgb(inputImgs[0].rows, inputImgs[0].cols, CV_8UC3);
	Mat outputGrayscale(inputImgs[0].rows, inputImgs[0].cols, CV_8UC1);

	//For each pixel
	for (int row = 0; row < inputImgs[0].rows; row++) {
		for (int col = 0; col < inputImgs[0].cols; col++) {
			//Compute weight
			float* weights = new float[inputImgs.size()];

			//Weights have to be normalized to the sum of values for each pixel
			float sumOfValues = 0;

			//For each image
			for (int i = 0; i < inputImgs.size(); i++) {
				weights[i] = computeWeight(inputImgs[i].at<Vec3b>(row, col));
				sumOfValues += weights[i];	
			}

			Vec3b finalRadiance;
			uint8_t finalRadianceGrayscale = 0;

			for (int i = 0; i < inputImgs.size(); i++) {
				weights[i] /= sumOfValues;

				Vec3b inputPixel = inputImgs[i].at<Vec3b>(row, col);

				//Sum of weighted pixel colors over set of images
				finalRadiance[2] += weights[i] * inputPixel[2];
				finalRadiance[1] += weights[i] * inputPixel[1];
				finalRadiance[0] += weights[i] * inputPixel[0];

				finalRadianceGrayscale += weights[i] * (0.299f * inputPixel[2] + 0.587f * inputPixel[1] + 0.114f * inputPixel[0]);
			}

			outputRgb.at<Vec3b>(row, col) = finalRadiance;
			outputGrayscale.at<uint8_t>(row, col) = finalRadianceGrayscale;
		}
	}

	imshow("HDR RGB", outputRgb);
	imshow("HDR Grayscale", outputGrayscale);
}

float ZMD3::computeWeight(Vec3b inputPixel) {
	float top = (float)pow(0.299f * inputPixel[2] + 0.587f * inputPixel[1] + 0.114f * inputPixel[0] - 255 * MU, 2);
	float bot = 2.0f * (float)pow(255 * SIGMA, 2);
	return exp(-(top / bot));
}