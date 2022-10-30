#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <thread>
#include <future>



class Binocular_calibration
{
	

public:
	Binocular_calibration();
	~Binocular_calibration();

	void Dateprintf();
	bool DateWriteyml(std::string path);
	void calibration(std::string pathl, std::string pathr, cv::Size boardsize, cv::Size squaresize);
	std::vector<cv::Mat> Get_Polar_rectification_images();
	std::vector<cv::Mat> Get_Original_images_L();
	std::vector<cv::Mat> Get_Original_images_R();
    double Get_usingtime(){return usingtime;}


private:

	bool Get_images(std::string pathL, std::string pathR);
	virtual std::vector<cv::Point2f> Getimagepoints(cv::Mat img, cv::Size boardsize) = 0;
	virtual void Getimagespoints(std::vector<cv::Mat> imgs, std::vector<cv::Mat> imgsr, cv::Size boardsize) = 0;
	void Getobjectpoints(cv::Size boardsize, cv::Size squaresize);	
	void Calibration();



protected:
	std::vector<cv::Mat> images_L;
	std::vector<cv::Mat> images_R;
	std::vector<cv::Mat> images_vconcat;

	std::vector<std::vector<cv::Point2f>> imagepoints_L;//图片角点坐标
	std::vector<std::vector<cv::Point2f>> imagepoints_R;//图片角点坐标
	std::future<std::vector<cv::Point2f>> results_L[50];//线程容量
	std::future<std::vector<cv::Point2f>> results_R[50];

private:
	std::vector<std::vector<cv::Point3f>> objectpoints;//物体客观世界坐标
	cv::Mat matrixl, distl, matrixr, distr, rvecsl, tvecsl, rvecsr, tvecsr;

	cv::Mat R, T, E, F;
	cv::Mat R1, R2, P1, P2, Q;

    double usingtime;
};


class Chessboard_Bin:public Binocular_calibration
{
public:

	virtual std::vector<cv::Point2f> Getimagepoints(cv::Mat img, cv::Size boardsize);
	virtual void Getimagespoints(std::vector<cv::Mat> imgsl, std::vector<cv::Mat> imgsr, cv::Size boardsize);

private:

};


