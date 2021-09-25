#include "ZMD2.h"

//	R	G	R	G	R
//	G	B	G	B	G
//	R	G	R	G	R
//	G	B	G	B	G
//	R	G	R	G	R

string mask[2][2] = { {"r", "g2"},
					  {"g1", "b"} };

ZMD2::ZMD2(String fileName) {

	Mat inputImg = imread(fileName, CV_8UC1);

	imshow("Input", inputImg);

	Mat outputImg(inputImg.rows, inputImg.cols, CV_8UC3);

	for (int row = 0; row < inputImg.rows; row++) {
		for (int col = 0; col < inputImg.cols; col++) {
			if(mask[col % 2][row % 2] == "r") {
				//B - average of the four blue pixels that are diagonal
				//G - average of the neighbouring green pixels to the left and right
				//R - x, y
				outputImg.at<Vec3b>(row, col)[0] = getAvgR('b', row, col, inputImg);
				outputImg.at<Vec3b>(row, col)[1] = getAvgR('g', row, col, inputImg);
				outputImg.at<Vec3b>(row, col)[2] = inputImg.at<uint8_t>(row, col);
			}
			else if (mask[col % 2][row % 2] == "g1") {
				//B - pro liché øádky: average of the neighbouring blue pixels above and below
				//G - x, y
				//R - pro liché øádky: average of the neighbouring red pixels left and right
				outputImg.at<Vec3b>(row, col)[0] = getAvgG1('b', row, col, inputImg);
				outputImg.at<Vec3b>(row, col)[1] = inputImg.at<uint8_t>(row, col);
				outputImg.at<Vec3b>(row, col)[2] = getAvgG1('r', row, col, inputImg);
			}
			else if (mask[col % 2][row % 2] == "g2") {
				//B - pro sudé øádky:  average of the neighbouring blue pixels left and right
				//G - x, y
				//R - pro sudé øádky:  average of the neighbouring red pixels above and below
				outputImg.at<Vec3b>(row, col)[0] = getAvgG2('b', row, col, inputImg);
				outputImg.at<Vec3b>(row, col)[1] = inputImg.at<uint8_t>(row, col);
				outputImg.at<Vec3b>(row, col)[2] = getAvgG2('r', row, col, inputImg);
			}
			else if (mask[col % 2][row % 2] == "b") {
				//B - x, y
				//G - average of the neighbouring green pixels to the left and right
				//R - average of the four red pixels that are diagonal
				outputImg.at<Vec3b>(row, col)[0] = inputImg.at<uint8_t>(row, col);
				outputImg.at<Vec3b>(row, col)[1] = getAvgB('g', row, col, inputImg);
				outputImg.at<Vec3b>(row, col)[2] = getAvgB('r', row, col, inputImg);
			}
		}
	}

	imshow("Output", outputImg);
}

uint8_t ZMD2::getAvgR(char lookingFor, int r, int c, Mat img) {
	int avg = 0;
	int count = 0;
	for (int row = r - 1; row <= r + 1; row++) {
		for (int col = c - 1; col <= c + 1; col++) {
			if ((0 <= col && col < img.cols) && (0 <= row && row < img.cols)) {
				if (mask[col % 2][row % 2] == "r") {
					//BLUE DIAGONAL
					if (lookingFor == 'b') {
						if (col - 1 >= 0 && row - 1 >= 0) {
							avg += img.at<uint8_t>(row - 1, col - 1);
							count++;
						}
						if (col + 1 <= img.cols && row - 1 >= 0) {
							avg += img.at<uint8_t>(row - 1, col + 1);
							count++;
						}
						if (col - 1 >= 0 && row + 1 <= img.rows) {
							avg += img.at<uint8_t>(row + 1, col - 1);
							count++;
						}
						if (col + 1 <= img.cols && row + 1 <= img.rows) {
							avg += img.at<uint8_t>(row + 1, col + 1);
							count++;
						}
					}
					//GREEN LEFT RIGHT
					if (lookingFor == 'g') {
						if (col - 1 >= 0) {
							avg += img.at<uint8_t>(row, col - 1);
							count++;
						}
						if (col + 1 <= img.cols) {
							avg += img.at<uint8_t>(row, col + 1);
							count++;
						}
					}
				}
			}
		}
	}
	uint8_t result = avg / count;
	return result;
}

uint8_t ZMD2::getAvgG1(char lookingFor, int r, int c, Mat img) {
	int avg = 0;
	int count = 0;
	for (int row = r - 1; row <= r + 1; row++) {
		for (int col = c - 1; col <= c + 1; col++) {
			if ((0 <= col && col < img.cols) && (0 <= row && row < img.cols)) {
				if (mask[col % 2][row % 2] == "g1") {
					//BLUE
					if (lookingFor == 'b') {
						//EVEN ROWS: ABOVE AND BELOW
						if (row % 2 == 0) {
							if (row - 1 >= 0) {
								avg += img.at<uint8_t>(row - 1, col);
								count++;
							}
							if (row + 1 < img.rows) {
								avg += img.at<uint8_t>(row + 1, col);
								count++;
							}
						}
					}
					//RED
					if (lookingFor == 'r') {
						//EVEN ROWS: LEFT AND RIGHT
						if (row % 2 == 0) {
							if (col - 1 >= 0) {
								avg += img.at<uint8_t>(row, col - 1);
								count++;
							}
							if (col + 1 < img.cols) {
								avg += img.at<uint8_t>(row, col + 1);
								count++;
							}
						}
					}
				}
			}
		}
	}
	uint8_t result = avg / count;
	return result;
}

uint8_t ZMD2::getAvgG2(char lookingFor, int r, int c, Mat img) {
	int avg = 0;
	int count = 0;
	for (int row = r - 1; row <= r + 1; row++) {
		for (int col = c - 1; col <= c + 1; col++) {
			if ((0 <= col && col < img.cols) && (0 <= row && row < img.cols)) {
				if (mask[col % 2][row % 2] == "g2") {
					//BLUE
					if (lookingFor == 'b') {
						//ODD ROWS: LEFT AND RIGHT
						if (row % 2 == 1) {
							if (col - 1 >= 0) {
								avg += img.at<uint8_t>(row, col - 1);
								count++;
							}
							if (col + 1 < img.cols) {
								avg += img.at<uint8_t>(row, col + 1);
								count++;
							}
						}
					}
					//RED
					if (lookingFor == 'r') {
						//ODD ROWS: ABOVE AND BELOW
						if (row % 2 == 1) {
							if (row + 1 < img.rows) {
								avg += img.at<uint8_t>(row + 1, col);
								count++;
							}
							if (row - 1 >= 0) {
								avg += img.at<uint8_t>(row - 1, col);
								count++;
							}
						}
					}
				}
			}
		}
	}
	uint8_t result = avg / count;
	return result;
}

uint8_t ZMD2::getAvgB(char lookingFor, int r, int c, Mat img) {
	int avg = 0;
	int count = 0;
	for (int row = r - 1; row <= r + 1; row++) {
		for (int col = c - 1; col <= c + 1; col++) {
			if ((0 <= col && col < img.cols) && (0 <= row && row < img.cols)) {
				if (mask[col % 2][row % 2] == "b") {
					//RED DIAGONAL
					if (lookingFor == 'r') {
						if (col - 1 >= 0 && row - 1 >= 0) {
							avg += img.at<uint8_t>(row - 1, col - 1);
							count++;
						}
						if (col + 1 < img.cols && row - 1 >= 0) {
							avg += img.at<uint8_t>(row - 1, col + 1);
							count++;
						}
						if (col - 1 >= 0 && row + 1 < img.rows) {
							avg += img.at<uint8_t>(row + 1, col - 1);
							count++;
						}
						if (col + 1 < img.cols && row + 1 < img.rows) {
							avg += img.at<uint8_t>(row + 1, col + 1);
							count++;
						}
					}
					//GREEN LEFT RIGHT
					if (lookingFor == 'g') {
						if (col - 1 >= 0) {
							avg += img.at<uint8_t>(row, col - 1);
							count++;
						}
						if (col + 1 < img.cols) {
							avg += img.at<uint8_t>(row, col + 1);
							count++;
						}
					}
				}
			}
		}
	}
	uint8_t result = avg / count;
	return result;
}