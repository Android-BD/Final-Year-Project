// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Utilities.h"

Mat loadImage(char* location, char* file);
Mat* loadImages(int numberOfImages, char* location, char** files);
void showImage(char* name, Mat image);
Mat findSign(Mat image);
Mat binaryImage(Mat image);
Mat backProjection(Mat image, Mat yellow);
Mat getHue(Mat image);
Mat templateMatching(Mat image, Mat templateImage);
void digitRecognition(Mat image);

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Using OpenCV " << CV_VERSION << endl;
	char* testLocation = "Media/Test Images/";
	char* testFiles[] = {
		"2809 a.jpg",
		"2809 b.jpg",
		"2830 a.jpg",
		"2830 b.jpg",
		"2838 a.jpg",
		"2838 b.jpg",
		"2838 c.jpg",
		"2838 d.jpg",
		"2839 a.jpg",
		"2839 b.jpg",
		"2839 c.jpg",
		"2839 d.jpg"
	};

	char* templateLocation = "Media/Templates/";
	char* templateFiles[] = {
		"stop.png",
		"stop&no.png",
		"stop2.png",
		"stop&no2.png",
		"yellow.png"
	};

	Mat sign = loadImage(testLocation, testFiles[6]);
	Mat templateSign = loadImage(templateLocation, templateFiles[2]);
	Mat yellow = loadImage(templateLocation, templateFiles[4]);

	showImage("Bus Stop Sign", sign);
	Mat hsvSign = findSign(sign);
	Mat binary = binaryImage(sign);
	Mat backProjSign = backProjection(sign, yellow);
	Mat templateMatch = templateMatching(sign, templateSign);

	showImage("HSV", hsvSign);
	showImage("Binary", binary);
	showImage("Back Projection", backProjSign);
	showImage("Template Matching", templateMatch);
	waitKey(0);

	/*int numberOfTestImages = sizeof(testFiles) / sizeof(testFiles[0]);
	Mat* busStops = loadImages(numberOfTestImages, testLocation, testFiles);
	int numberOfTemplateImages = sizeof(templateFiles) / sizeof(templateFiles[0]);
	Mat* templates = loadImages(numberOfTemplateImages, templateLocation, templateFiles);

	for (int i = 0; i < numberOfTestImages; i++) {
		cout << "Processing image " << (i + 1) << endl;
		showImage("Sign", busStops[i]);
		Mat hsvSign = findSign(busStops[i]);
		Mat binary = binaryImage(busStops[i]);
		Mat backProjSign = backProjection(busStops[i], templates[4]);
		Mat templateMatch;
		if (i == 0 || i == 1) {
			// templates[0] or templates[1]
			templateMatch = templateMatching(busStops[i], templates[0]);

		}
		else {
			// templates[2] or templates[3]
			templateMatch = templateMatching(busStops[i], templates[2]);

		}

		showImage("HSV", hsvSign);
		showImage("Binary", binary);
		showImage("Back Projection", backProjSign);
		showImage("Template Matching", templateMatch);
		
		waitKey(0);
	}*/

	return 0;
}

Mat loadImage(char* location, char* file) {
	string filename(location);
	filename.append(file);
	Mat image = imread(filename, 1);

	return image;
}

Mat* loadImages(int numberOfImages, char* location, char** files) {
	Mat* images = new Mat[numberOfImages];
	for (int i = 0; i < numberOfImages; i++) {
		string filename(location);
		filename.append(files[i]);
		cout << "Loading image " << (i + 1) << ": " << filename << endl;
		images[i] = imread(filename, 1);
	}
	return images;
}

void showImage(char* name, Mat image) {
	resize(image, image, Size(image.cols / 4, image.rows / 4));
	imshow(name, image);

	return;
}

Mat findSign(Mat image) {
	Mat hsv;
	cvtColor(image, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(15, 135, 140), Scalar(30, 255, 255), hsv);

	return hsv;
}

Mat binaryImage(Mat image) {
	Mat greyImage, binaryResult;
	cvtColor(image, greyImage, CV_BGR2GRAY);
	threshold(greyImage, binaryResult, 200, 255, THRESH_BINARY);

	return binaryResult;
}

Mat backProjection(Mat image, Mat yellow) {
	Mat imageHue = getHue(image);
	Mat yellowHue = getHue(yellow);

	Mat hist;
	int histSize = 180;
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	calcHist(&yellowHue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	Mat backProject;
	calcBackProject(&imageHue, 1, 0, hist, backProject, &ranges, 1, true);

	return backProject;
}

Mat getHue(Mat image) {
	Mat hsv, hue;
	cvtColor(image, hsv, CV_BGR2HSV);

	hue.create(hsv.size(), hsv.depth());
	int ch[] = { 0, 0 };
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	return hue;
}

Mat templateMatching(Mat image, Mat templateImage) {
	// TODO: try templateMatching as a method for finding signs
	Mat result;

	Mat imageDisplay;
	image.copyTo(imageDisplay);

	int rows = image.rows - templateImage.rows + 1;
	int cols = image.cols - templateImage.cols + 1;

	result.create(rows, cols, CV_32FC1);

	matchTemplate(image, templateImage, result, CV_TM_SQDIFF);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	matchLoc = minLoc;

	rectangle(imageDisplay, matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows), Scalar::all(0), 2, 8, 0);

	return result;
}

void digitRecognition(Mat image) {
	// TODO: recognise numbers from sign

	return;
}