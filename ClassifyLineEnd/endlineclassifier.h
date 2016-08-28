#pragma once

#ifndef ENDLINECLASSIFIER_H
#define ENDLINECLASSIFIER_H

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <msclr\marshal_cppstd.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include "zhangsuen.h"



using namespace cv;

using namespace std;
using namespace System::IO;
using namespace System::Windows;
using namespace System::Diagnostics;

class lineimage
{
	
//private:
	Mat origImage;
	Mat grayScaleImage;
	Mat binaryImage;
	Mat morphedImage;
	Mat skeletonImage;
	vector<cv::Point> skelPoints;
	vector<cv::Point> endPoints;
	vector<int> endAreas;

	Mat edgeImage;
	Mat overlapImage;
	Mat contourImage;

	
	
	cv::String  origImgPath;
	cv::String defaultPath = "Results\\";
	cv::String filename;

public:
	void setOrigImg(System::String ^ imgpath, System::String ^ savepath) {
		origImgPath = strToCv(imgpath);

		std::wstring resultspath = strToWstr("Results\\");
		CreateDirectory(resultspath.c_str(), NULL);

		origImage = imread(origImgPath, CV_LOAD_IMAGE_COLOR);
		//blur(origImage, origImage, Size(3, 3));
		//medianBlur(origImage, origImage, 15);
		cv::resize(origImage, origImage, cv::Size(), 0.30, 0.30);
		filename = strToCv(Path::GetFileName(imgpath));

		imwrite(defaultPath + "orig_" +  filename, origImage);
	}

	Mat getOrigImg() {
		return origImage;
	}
	///////////////////////
	void setGrayImg(Mat img) {
		Mat res;
		cvtColor(img, res, COLOR_RGB2GRAY);
		
		grayScaleImage = res;
		imwrite(defaultPath + "gray_" + filename, res);

	}
	
	Mat getGrayImg() {
		return grayScaleImage;
	}

	///////////////////////
	void setBinImg(Mat img) {
		Mat res;
		//adaptiveThreshold(img, res, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11,-5);

		threshold(img, res, 0, 255, CV_THRESH_OTSU);

		imwrite(defaultPath + "bin_" + filename,res);
		binaryImage = res;
	}

	Mat getBinImg() {
		return binaryImage;
	}
	///////////////////////

	void setMorpImg(Mat img) {
		Mat res = img.clone();
		int morph_elem = 2;
		int morph_size = 3;
		int morph_operator = 0;
		//threshold(img, img, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

		//Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
		
		//erode(img, res, element, cv::Point(-1, -1), 7);

		morphologyEx(res, res, MORPH_OPEN, element, cv::Point(-1,-1),1);

		imwrite(defaultPath + "morp_" + filename, res);
		morphedImage = res;
	}

	Mat getMorpImg() {
		return morphedImage;
	}

	///////////////////////
	void setSkelImg(Mat img) {
		Mat res = img.clone();

		cvtColor(res, res, COLOR_RGB2GRAY);
		threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);

		thin(res, false, false, false);

		//save thinned image
		imwrite(defaultPath + "thin_" + filename, res);
		
		skeletonImage = res.clone();

		//retrieve the skeleton points.
		threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
		cv::findNonZero(res, skelPoints);
		
	}

	Mat getSkelImg() {
		return skeletonImage;
	}

	vector<cv::Point> getSkelPoints() {
		return skelPoints;
	}

	//////////////////////
	void setEndPoints(Mat img) {
		Mat res = img.clone();
		threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
		vector<cv::Point> n = { cv::Point(-1,1), cv::Point(0,1), cv::Point(1,1),
								cv::Point(-1,0), cv::Point(0, 0), cv::Point(1,0),
								cv::Point(-1,-1), cv::Point(0,-1), cv::Point(1,-1) };

		vector<cv::Point> end;
		//Check neighbor
		//possible bottle neck, convert ko to sa pointers. 
		for (int i = 0; i < skelPoints.size(); i++) {
			int nCount = 0;
			for (int j = 0; j < n.size(); j++) {
				if (res.at<uchar>(skelPoints[i].y + n[j].y,
					skelPoints[i].x + n[j].x) == 255) {
					nCount++;

				}
			}

			if (nCount <= 2) {
				end.push_back(cv::Point(skelPoints[i].x, skelPoints[i].y));
			}

			//res.at<uchar>(skelPoints[i].y, skelPoints[i].x); //tangnang to, dapat pala y,x sa halip x,y
		}


		/*shuuten o tamesu*/
		Debug::WriteLine(end.size());
		cvtColor(res, res, COLOR_GRAY2BGR);
		for (int i = 0; i < end.size(); i++) {
			circle(res,	cv::Point(end[i].x, end[i].y),	32.0, Scalar(0, 0, 255),1,8);
		}

		Debug::WriteLine(n.size());
		//Debug::WriteLine(skelPoints.size());
		//Debug::WriteLine(skelPoints[skelPoints.size()-1].y);
		imwrite(defaultPath + "thinsss_" + filename, res);
		//for (int i = 0; skelPoints.size(); i++) {
		//	Debug::WriteLine(skelPoints[i].y);
		//}

		endPoints = end;
	}

	vector<cv::Point> getEndPoints() {
		return endPoints;
	}




	//////////////////////
	void setEdgeImg(Mat img) {
		
		Mat src = grayScaleImage.clone();
		Mat src_gray= grayScaleImage.clone();
		Mat dst, detected_edges;

		int edgeThresh = 1;
		int lowThreshold = 100;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;

		/// Reduce noise with a kernel 3x3
		blur(binaryImage, detected_edges, Size(3, 3));

		/// Canny detector
		Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
		blur(detected_edges, detected_edges, Size(3, 3));
		Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
		//Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
		/// Using Canny's output as a mask, we display our result
		dst = Scalar::all(0);
		
		src.copyTo(dst, detected_edges);
		//cvtColor(dst, dst, COLOR_BGR2GRAY);
		//threshold(dst, dst, 0, 255, THRESH_OTSU);
		imwrite(defaultPath + "edge_" + filename, dst);
	}
	Mat getCannyEdge() {
		return edgeImage;
	}
	///////////////////////
	void setContourImg(Mat img) {

		Mat src = img.clone();
		Mat drawing;
		vector<vector<cv::Point>> contours;
		vector<Vec4i> heirachy;
		threshold(src, src, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		//medianBlur(src, src, 11);
		
		cv::findContours(src, contours, heirachy, RETR_CCOMP, CV_CHAIN_APPROX_TC89_KCOS, cv::Point(0, 0));

		
		

		/*
		//Draw contours
		int largestContour = 0;
		for (int i = 0; i<contours.size(); i++)
		{
			if (contourArea(contours[i]) > contourArea(contours[largestContour])) {
				largestContour = i;
			}
		}
		*/

		//Setup the output into black
		drawing = Mat::zeros(src.size(), CV_8UC3);

		for (size_t idx = 0; idx < contours.size(); idx++) {
			Debug::Write("Heirarchy index: ");
			Debug::WriteLine(heirachy[idx][3]);

			Debug::Write("Countour area: ");
			Debug::WriteLine(contourArea(contours[idx]));
			
			if (heirachy[idx][3] == -1 && contourArea(contours[idx]) > 5) {
				//cv::drawContours(drawing, contours, idx, cv::Scalar(0, 0, 0), CV_FILLED);
				cv::drawContours(drawing, contours, idx, cv::Scalar(255, 255, 255), CV_FILLED, 8, heirachy);
			}
		}
		Debug::Write("Heirarchy is: ");
		Debug::WriteLine(heirachy.size());
		//Debug::WriteLine(contourArea(contours[largestContour]));
		//cv::drawContours(drawing, contours, -1, Scalar(255, 255, 255), 1, 8, heirachy, 1, cv::Point());

		imwrite(defaultPath + "cont_" + filename, drawing);
		contourImage = drawing;
	}

	Mat getContourImg() {return contourImage;}
	////////////////////////


	///////////////////////
	void setLineEndArea(Mat img, vector<cv::Point> p) {
		Mat res = img.clone();
		Mat croppedImage;
		Rect ROI;

		threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
		for (int i = 0; i < p.size(); i++) {
			ROI = squareAroundCentroid(cv::Point(p[i].x, p[i].y), 20, res.cols-1, res.rows-1);
			croppedImage = res(ROI);
			endAreas.push_back (cv::countNonZero(croppedImage));
			Debug::Write("Area: ");
			Debug::WriteLine(endAreas[i]);
			imwrite(defaultPath   + intToStr(i) + "_cropped_" + filename, croppedImage);
			
		}

		
		/*
		cvtColor(res, res, COLOR_GRAY2BGR);
		cv::rectangle(res,ROI,cv::Scalar(0, 0, 255),1,8);
		imwrite(defaultPath + "end_" + filename, res);
		*/
		
		//lineEndArea = res;s
	}

	Mat getLineEndArea() {}//return //lineEndArea;}

	////////////////////////
	void setOverlapImg(Mat img, Mat img2) {
		Mat res;
		imwrite(defaultPath + "b_" + filename, img);
		imwrite(defaultPath + "s_" + filename, img2);

		addWeighted(img, 0.5, img2, 0.5, 0.0, res);
		imwrite(defaultPath + "over_" + filename, res);
		overlapImage = res;
	}

	Mat getOverlap() {
		return overlapImage;
	}

	////////////////////////
	Rect squareAroundCentroid(cv::Point p, int size, int width, int height) {
		cv::Point a, b;
		Rect res;
		int rad = size / 2;
		int x1 = p.x - rad,
			y1 = p.y + rad,
			x2 = p.x + rad,
			y2 = p.y - rad;

		x1 = (x1 < 0) ? 0 : x1;
		y1 = (y1 < 0) ? 0 : y1;
		x2 = (x2 < 0) ? 0 : x2;
		y2 = (y2 < 0) ? 0 : y2;

		Debug::Write(width);
		Debug::Write(" ");
		Debug::WriteLine(height);

		Debug::Write(p.y + rad);
		Debug::Write(" ");
		Debug::WriteLine(p.y - rad);

		a = cv::Point(	(x1 > width) ? width : x1, 
						(y1 > height) ? height : y1);


		b = cv::Point((x2 > width) ? width : x2,
						(y2 > height) ? height : y2 );

		return Rect(a, b);
	}
	///////////////////////
	




	///////////////////////
	//convert of strings
	cv::String strToCv(System::String ^ s) {
		//System::String^ managedString = "test";

		msclr::interop::marshal_context context;
		std::string standardString = context.marshal_as<std::string>(s);
		return standardString;
	}

	std::string strToStd(System::String ^ s) {
		//System::String^ managedString = "test";

		msclr::interop::marshal_context context;
		std::string standardString = context.marshal_as<std::string>(s);
		return standardString;
	}

	std::wstring strToWstr(System::String ^ s) {
		std::wstring result = msclr::interop::marshal_as<std::wstring>(s);
		return result;
	}

	std::string intToStr(int n) {
		std::stringstream ss;
		ss << n;

		std::string str;
		ss >> str;

		return str;
	}
};



#endif ENDLINECLASSIFIER




