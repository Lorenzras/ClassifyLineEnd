#pragma once

#ifndef ENDLINECLASSIFIER_H
#define ENDLINECLASSIFIER_H

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <msclr\marshal_cppstd.h>
#include <iostream>
#include <windows.h>
#include "zhangsuen.h"



using namespace cv;

using namespace std;
using namespace System::IO;
using namespace System::Windows;

class lineimage
{
	

	Mat origImage;
	Mat grayScaleImage;
	Mat binaryImage;
	Mat morphedImage;
	Mat skeletonImage;
	Mat edgeImage;
	Mat overlapImage;
	Mat contourImage;
	Mat lineEndImage;
	
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
		medianBlur(origImage, origImage, 15);

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
		adaptiveThreshold(img, res, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11,-5);

		//threshold(img, res, 0, 255, THRESH_OTSU);
		//medianBlur(res, res, 15);
		imwrite(defaultPath + "bin_" + filename,res);
		binaryImage = res;
	}

	Mat getBinImg() {
		return binaryImage;
	}
	///////////////////////

	void setMorpImg(Mat img) {
		Mat res;
		int morph_elem = 2;
		int morph_size = 15;
		int morph_operator = 0;
		

		//Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
		
		morphologyEx(img, res, MORPH_OPEN, element);
		//morphologyEx(res, res, MORPH_OPEN, element);
		

		imwrite(defaultPath + "morp_" + filename, res);
		morphedImage = res;
	}

	Mat getMorpImg() {
		return morphedImage;
	}

	///////////////////////
	void setSkelImg(Mat img) {
		Mat res = img.clone();
		
		thin(res, true, true, true);
		imwrite(defaultPath + "thin_" + filename, res);
		skeletonImage = res;
	}

	Mat getSkelImg() {
		return skeletonImage;
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
		threshold(src, src, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		Mat drawing;

		vector<vector<cv::Point>> contours;
		vector<Vec4i> heirachy;

		medianBlur(src, src, 11);
		
		cv::findContours(src, contours, heirachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		//Setup the output into black
		drawing = Mat::zeros(src.size(), CV_8UC3);


		//Draw contours
		for (int i = 0; i<contours.size(); i++)
		{
			cv::drawContours(drawing, contours, i, Scalar(255, 255, 255), 1, 8, heirachy, 0, cv::Point());
		}

		imwrite(defaultPath + "cont_" + filename, drawing);
		contourImage = drawing;
	}

	Mat getContourImg() {
		return contourImage;
	}
	////////////////////////
	void setLineEndImg(Mat img) {
		Mat res = img.clone();

		
		imwrite(defaultPath + "end_" + filename, res);
		skeletonImage = res;
	}

	Mat getLineEndImg() {



		return skeletonImage;
	}


	////////////////////////
	void setOverlapImg(Mat img, Mat img2) {
		Mat res;
		imwrite(defaultPath + "o_" + filename, img);
		imwrite(defaultPath + "a_" + filename, img2);

		addWeighted(img, 0.5, img2, 0.5, 0.0, res);
		imwrite(defaultPath + "over_" + filename, res);
		overlapImage = res;
	}

	Mat getOverlap() {
		return overlapImage;
	}

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
};



#endif ENDLINECLASSIFIER




