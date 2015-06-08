#pragma once


#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


//
//#define ORIGCOL2COL CV_BGR2HLS
//#define COL2ORIGCOL CV_HLS2BGR
//#define NSAMPLES 7
#define PI 3.14159

#include "myImage.h"

using namespace cv;
using namespace std;

class HandGesture{
	public:
		MyImage m;
		HandGesture();
		vector<vector<Point> > contours;
		vector<vector<int> >hullI;
		vector<vector<Point> >hullP;
		vector<vector<Vec4i> > defects;	
		vector <Point> fingerTips;
		Rect rect;
		int moveHorizonBeforeX;
		int moveHorizonBeforeY;
		float beforeAngle;
		int sizeSqure;
		void printGestureInfo(Mat src);
		int cIdx;
		int frameNumber;
		int mostFrequentFingerNumber;
		int nrOfDefects;
		Rect bRect;
		double bRect_width;
		double bRect_height;
		bool isHand;
		bool detectIfHand();
		void initVectors();
		void getFingerNumber(MyImage *m);
		void eleminateDefects(MyImage *m);
		void getFingerTips(MyImage *m);
		void drawFingerTips(MyImage *m);
	private:
		int fingerN1;
		int fingerN2;
		string statusFinger;
		string bool2string(bool tf);
		int fontFace;
		int prevNrFingerTips;
		void checkForOneFinger(MyImage *m);
		float getAngle(Point s,Point f,Point e);
		int getAngle(int x, int y, int bx, int by);
		vector<int> fingerNumbers;
		void analyzeContours();
		string intToString(int number);
		void computeFingerNumber();
		void drawNewNumber(MyImage *m);
		void addNumberToImg(MyImage *m);
		void addTextToImg(MyImage *m);
		vector<int> numbers2Display;
		void addFingerNumberToVector();
		Scalar numberColor;
		int nrNoFinger;
		float distanceP2P(Point a,Point b);
		void removeRedundantEndPoints(vector<Vec4i> newDefects,MyImage *m);
		void removeRedundantFingerTips();
};

