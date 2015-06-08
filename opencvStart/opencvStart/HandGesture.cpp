/*

HandGesture::HandGesture(){
precondition : none
postcondition : init the first value

void HandGesture::initVectors(){
precondition : none
postcondition : init the contours 

void HandGesture::analyzeContours(){
precondition : none
postcondition : analyze the contours size

string HandGesture::bool2string(bool tf){
precondtion : if is not null
postcondition : none

string HandGesture::intToString(int number){
precondition : value is not null
postcondition : change the int value to stringstream
return : string number to string value

void HandGesture::printGestureInfo(Mat src){
precondition : src is not null 
postcondition : print the gesture infomation

bool HandGesture::detectIfHand(){
precondition : none
postcondition : if the hand is detected print the location
return : where is the hand

float HandGesture::distanceP2P(Point a, Point b){
precontition : hand is detected
postcondition : return the value distance
return : distance

void HandGesture::removeRedundantFingerTips(){
precondition : fingertip is detected
postcondition : remove the fingertip

void HandGesture::computeFingerNumber(){
precondition : none
postcondition : compute how many finger is

void HandGesture::addFingerNumberToVector(){
precondiition : none
postcondition : add finger number to vector

void HandGesture::addNumberToImg(MyImage *m){
precondition : none
postcondition : add number to image

void HandGesture::getFingerNumber(MyImage *m){
precondition : *m is not null
postcondition : get the finger number

float HandGesture::getAngle(Point s, Point f, Point e){
precondition : point is not null
postcondition : get the angle value
return : angle

void HandGesture::eleminateDefects(MyImage *m){
precondition : m is not null
postcondition : eleminate the defect

void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects,MyImage *m){
precondition : value is not null
postcondition : remove redundant end point

void HandGesture::checkForOneFinger(MyImage *m){
precondition : m is not null
postcondition : check if the finger is one

void HandGesture::drawFingerTips(MyImage *m){
precondition : image is not null
postcondition : draw the where is finger tip

void HandGesture::getFingerTips(MyImage *m){
precondition : m is not null
postcondition : get finget tip

*/
#include "StdAfx.h"
#include "HandGesture.h"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

HandGesture::HandGesture(){
	frameNumber=0;
	nrNoFinger=0;
	fingerN1 = 0;
	fingerN2 = 0;
	sizeSqure = 0;
	fontFace = FONT_HERSHEY_PLAIN;
	moveHorizonBeforeX=0;
	moveHorizonBeforeY=0;
	beforeAngle = 0;
}

void HandGesture::initVectors(){
	hullI=vector<vector<int> >(contours.size());
	hullP=vector<vector<Point> >(contours.size());
	defects=vector<vector<Vec4i> > (contours.size());	
}

void HandGesture::analyzeContours(){
	bRect_height=bRect.height;
	bRect_width=bRect.width;
}

string HandGesture::bool2string(bool tf){
	if(tf)
		return "true";
	else
		return "false";
}

string HandGesture::intToString(int number){
		stringstream ss;
		ss << number;
		string str = ss.str();
		return str;
}

void HandGesture::printGestureInfo(Mat src){
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245,200,200);
	int xpos=src.cols/1.5;
	int ypos=src.rows/1.6;
	float fontSize=0.7f;
	int lineChange=14;
	string info= "Figure info:";
	putText(src,info,Point(ypos,xpos),fontFace,fontSize,fColor);
	xpos+=lineChange;
	info=string("Number of defects: ") + string(intToString(nrOfDefects)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
	xpos+=lineChange;
	info=string("bounding box height, width ") + string(intToString(bRect_height)) + string(" , ") +  string(intToString(bRect_width)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
	xpos+=lineChange;
	info=string("Is hand: ") + string(bool2string(isHand));
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
}

bool HandGesture::detectIfHand(){
	analyzeContours();
	double h = bRect_height; 
	double w = bRect_width;
	isHand=true;
	if(fingerTips.size() > 5 ){
		isHand=false;
	}else if(h==0 || w == 0){
		isHand=false;
	}else if(h/w > 4 || w/h >4){
		isHand=false;	
	}else if(bRect.x<20){
		isHand=false;	
	}	
	return isHand;
}

float HandGesture::distanceP2P(Point a, Point b){
	float d= sqrt(fabs( pow((float)a.x-b.x,2) + pow((float)a.y-b.y,2) )) ;  
	return d;
}

// remove fingertips that are too close to 
// eachother
void HandGesture::removeRedundantFingerTips(){
	vector<Point> newFingers;
	for(int i=0;i<fingerTips.size();i++){
		for(int j=i;j<fingerTips.size();j++){
			if(distanceP2P(fingerTips[i],fingerTips[j])<10 && i!=j){
			}else{
				newFingers.push_back(fingerTips[i]);	
				break;
			}	
		}	
	}
	fingerTips.swap(newFingers);
}

void HandGesture::computeFingerNumber(){
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int frequentNr;	
	int thisNumberFreq=1;
	int highestFreq=1;
	frequentNr=fingerNumbers[0];
	for(int i=1;i<fingerNumbers.size(); i++){
		if(fingerNumbers[i-1]!=fingerNumbers[i]){
			if(thisNumberFreq>highestFreq){
				frequentNr=fingerNumbers[i-1];	
				highestFreq=thisNumberFreq;
			}
			thisNumberFreq=0;	
		}
		thisNumberFreq++;	
	}
	if(thisNumberFreq>highestFreq){
		frequentNr=fingerNumbers[fingerNumbers.size()-1];	
	}
	mostFrequentFingerNumber=frequentNr;	
}

void HandGesture::addFingerNumberToVector(){
	int i=fingerTips.size();	
	fingerNumbers.push_back(i);
}

// add the calculated number of fingers to image m->src
void HandGesture::addNumberToImg(MyImage *m){
	int xPos=10;
	int yPos=10;
	int offset=30;
	float fontSize=1.5f;
	int fontFace = FONT_HERSHEY_PLAIN;
	for(int i=0;i<numbers2Display.size();i++){
		rectangle(m->src,Point(xPos,yPos),Point(xPos+offset,yPos+offset),numberColor, 2);	
		putText(m->src, intToString(numbers2Display[i]),Point(xPos+7,yPos+offset-3),fontFace,fontSize,numberColor);
		xPos+=40;
		if(xPos>(m->src.cols-m->src.cols/3.2)){
			yPos+=40;
			xPos=10;
		}
	}
}
void HandGesture::addTextToImg(MyImage *m){
	int xPos = 20;
	int yPos = 20;
	int offset = 30;
	float fontSize = 1.0f;
	int fontFace = FONT_HERSHEY_PLAIN;
	float angle;
	Scalar fColor(245,200,200);

	int lineChange=18;
	string info= "Figure info:";
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize,fColor);
	xPos+=lineChange;
	info=string("Number of defects: ") + string(intToString(fingerN2)) +" " + string(intToString(fingerN1))+" "+ string(intToString(nrOfDefects)) ;
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize  ,fColor);

	xPos+=lineChange;
	info=string("second problem command : ");	
	if((fingerN1==0) && (nrOfDefects==1))
	{
		info = string("second problem command : power point start");	
	}
	else if((fingerN2==0) && (fingerN1==2) && (nrOfDefects==1))
	{
		info = string("second problem command : full screen mode");
	}
	else 
	{
		info=string("sencond problem command : none");
	}
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize  ,fColor);

	xPos+=lineChange;
	info=string("location hand : ") + string(intToString(moveHorizonBeforeX)) + string(" , ") + string(intToString(moveHorizonBeforeY)) + string(" , ") 
		+ string(intToString(bRect.x)) + string(" , ") +  string(intToString(bRect.y)) ;
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);

	xPos+=lineChange;
	if(bRect.x > moveHorizonBeforeX)
		info=string("thrid-1 problem command : ->");
	else 
		info=string("thrid-1 problem command : <- ");
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);

	xPos+=lineChange;
	angle = getAngle( bRect.x, bRect.y, moveHorizonBeforeX, moveHorizonBeforeY);
	info=string("angle hand : ") + string(intToString(angle) ) ;
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);
	moveHorizonBeforeX=bRect.x;
	moveHorizonBeforeY=bRect.y;

	xPos+=lineChange;
	if( angle > beforeAngle) 
		info=string("thrid-2 problem command : O") ;
	else
		info=string("thrid-2 problem command : X") ;
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);
	beforeAngle = angle;

	xPos+=lineChange;
	info=string("bounding box height, width ") + string(intToString(bRect_height)) + string(" , ") +  string(intToString(bRect_width)) ;
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);

	xPos+=lineChange;
	if( sizeSqure > bRect_height*bRect_width)
		info=string("thrid-3 problem command : --");
	else 	
		info=string("thrid-3 problem command : ++");

	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize ,fColor);
	sizeSqure = bRect_height*bRect_width;

	xPos+=lineChange;
	info=string("Is hand: ") + string(bool2string(isHand));
	putText(m->src,info,Point(yPos,xPos),fontFace,fontSize  ,fColor);

	for(int i=2;i<numbers2Display.size();i++){
		
	
			xPos+=310;
		if(xPos>(m->src.cols-m->src.cols/3.2)){
			yPos+=40;
			xPos=10;
		}

	}

}

// calculate most frequent numbers of fingers 
// over 20 frames
void HandGesture::getFingerNumber(MyImage *m){
	removeRedundantFingerTips();
	if(bRect.height > m->src.rows/2 && nrNoFinger>12 && isHand ){
		numberColor=Scalar(0,200,0);
		addFingerNumberToVector();
		if(frameNumber>12){
			nrNoFinger=0;
			frameNumber=0;	
			computeFingerNumber();	
			numbers2Display.push_back(mostFrequentFingerNumber);
			fingerNumbers.clear();
		}else{
			frameNumber++;
		}
	}else{
		nrNoFinger++;
		numberColor=Scalar(200,200,200);
	}
	//addNumberToImg(m);
	addTextToImg(m);
}

float HandGesture::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f,s);
	float l2 = distanceP2P(f,e);
	float dot=(s.x-f.x)*(e.x-f.x) + (s.y-f.y)*(e.y-f.y);
	float angle = acos(dot/(l1*l2));
	angle=angle*180/PI;
	
	return angle;
}

int HandGesture::getAngle(int x, int y, int bx, int by){
	double dx = x - bx;
	double dy = y - by;
	double rad = atan2(dy,dx);
	int degree = rad*180/PI;
	if(degree < 0)degree = degree + 360;
	return degree;
}


void HandGesture::eleminateDefects(MyImage *m){
	fingerN2 = fingerN1;
	fingerN1 = nrOfDefects;
	int tolerance =  bRect_height/5;
	float angleTol=95;
	vector<Vec4i> newDefects;
	int startidx, endidx, faridx;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
		if(distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance && getAngle(ptStart, ptFar, ptEnd  ) < angleTol ){
			if( ptEnd.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else if( ptStart.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else {
				newDefects.push_back(v);		
			}
		}	
		d++;
	}
	nrOfDefects=newDefects.size();
	defects[cIdx].swap(newDefects);
	removeRedundantEndPoints(defects[cIdx], m);
}

// remove endpoint of convexity defects if they are at the same fingertip
void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects,MyImage *m){
	Vec4i temp;
	float avgX, avgY;
	float tolerance=bRect_width/6;
	int startidx, endidx, faridx;
	int startidx2, endidx2;
	for(int i=0;i<newDefects.size();i++){
		for(int j=i;j<newDefects.size();j++){
	    	startidx=newDefects[i][0]; Point ptStart(contours[cIdx][startidx] );
	   		endidx=newDefects[i][1]; Point ptEnd(contours[cIdx][endidx] );
	    	startidx2=newDefects[j][0]; Point ptStart2(contours[cIdx][startidx2] );
	   		endidx2=newDefects[j][1]; Point ptEnd2(contours[cIdx][endidx2] );
			if(distanceP2P(ptStart,ptEnd2) < tolerance ){
				contours[cIdx][startidx]=ptEnd2;
				break;
			}if(distanceP2P(ptEnd,ptStart2) < tolerance ){
				contours[cIdx][startidx2]=ptEnd;
			}
		}
	}
}

// convexity defects does not check for one finger
// so another method has to check when there are no
// convexity defects
void HandGesture::checkForOneFinger(MyImage *m){
	int yTol=bRect.height/6;
	Point highestP;
	highestP.y=m->src.rows;
	vector<Point>::iterator d=contours[cIdx].begin();
	while( d!=contours[cIdx].end() ) {
   	    Point v=(*d);
		if(v.y<highestP.y){
			highestP=v;
			cout<<highestP.y<<endl;
		}
		d++;	
	}int n=0;
	d=hullP[cIdx].begin();
	while( d!=hullP[cIdx].end() ) {
   	    Point v=(*d);
			cout<<"x " << v.x << " y "<<  v.y << " highestpY " << highestP.y<< "ytol "<<yTol<<endl;
		if(v.y<highestP.y+yTol && v.y!=highestP.y && v.x!=highestP.x){
			n++;
		}
		d++;	
	}if(n==0){
		fingerTips.push_back(highestP);
	}
}

void HandGesture::drawFingerTips(MyImage *m){
	Point p;
	int k=0;
	for(int i=0;i<fingerTips.size();i++){
		p=fingerTips[i];
		putText(m->src,intToString(i),p-Point(0,30),fontFace, 1.2f,Scalar(200,200,200),2);
   		circle( m->src,p,   5, Scalar(100,255,100), 4 );
   	 }
}

void HandGesture::getFingerTips(MyImage *m){
	fingerTips.clear();
	int i=0;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    int startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		int endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    int faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
		if(i==0){
			fingerTips.push_back(ptStart);
			i++;
		}
		fingerTips.push_back(ptEnd);
		d++;
		i++;
   	}
	if(fingerTips.size()==0){
		checkForOneFinger(m);
	}
}