#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <thread>
#include <future>

class Monocular_calibration
{
public:
	Monocular_calibration();
	~Monocular_calibration();

	void Dateprintf(); 
	bool DateWriteyml(std::string path);
	void Error_analysis(cv::Size boardsize);

	void calibration(std::string path, cv::Size boardsize, cv::Size squaresize);


	std::vector<double> Get_errors()	{		return  errors;	}
	double Get_totalerror()         	{		return total_error;	}
	cv::Mat Get_matrix()                {		return  matrix;	}
	cv::Mat Get_dist()	                {		return  dist;	}

private:
	

	void Set_image_path(std::string path)
	{
		image_path = path;
	}
	bool Get_images(std::string path);
	virtual std::vector<cv::Point2f> Getimagepoints(cv::Mat img, cv::Size boardsize) = 0;
	virtual void Getimagespoints(std::vector<cv::Mat> imgs, cv::Size boardsize) = 0;
	void Getobjectpoints(cv::Size boardsize,cv::Size squaresize);
	void Calibration();
	


protected:

	std::vector<std::vector<cv::Point2f>> imagepoints;//ͼƬ�ǵ�����
	std::future<std::vector<cv::Point2f>> results[50];
	std::vector<cv::Mat> images;
	

private:
	std::string image_path;
	
	
	std::vector<std::vector<cv::Point3f>> objectpoints;//����͹���������
	
	cv::Mat matrix, dist;
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	std::vector<double>errors;//ÿ��ͼƬ���
	double total_error;//ƽ�����

};


//���̸�
class Chessboard : public Monocular_calibration
{
public:


	virtual std::vector<cv::Point2f> Getimagepoints(cv::Mat img, cv::Size boardsize);
	virtual void Getimagespoints(std::vector<cv::Mat> imgs, cv::Size boardsize);
	//virtual void Getimagespoints(std::vector<cv::Mat> imgs, cv::Size boardsize);

private:

	std::vector<cv::Mat> images_draw;
	
	std::mutex mymutex;
	
};
