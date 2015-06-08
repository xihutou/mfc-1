#include "StdAfx.h"
#include "MyImage.h"
/*MyImage::MyImage(int webCamera){
precondition : webCamera is not null
postcondition : capture image
*/
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;

MyImage::MyImage(){
}

MyImage::MyImage(int webCamera){
	cameraSrc=webCamera;
	cap=VideoCapture(webCamera);
}