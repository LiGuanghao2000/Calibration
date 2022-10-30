//
// Created by 16663 on 2022/10/23.
//

#ifndef CAMERA_DEMO_CALIBRATION_THREAD_H
#define CAMERA_DEMO_CALIBRATION_THREAD_H


#include <qthread.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include "Monocular_calibration.h"
#include "Binocular_calibration.h"

class thread_single :
        public QThread
{
Q_OBJECT

public:
    thread_single(QString path, cv::Size boardsize,cv::Size squaresize,int method);
    ~thread_single();

protected:
    void run();

signals:
    void Monocular_over();

public:
    Monocular_calibration *Mo= nullptr;

private:
    QString Path;
    cv::Size Boardsize;
    cv::Size Squaresize;

    int Method;

public:

};

class thread_double :
        public QThread
{
Q_OBJECT

public:
    thread_double(QString path_L,QString path_R, cv::Size boardsize,cv::Size squaresize,int method);
    ~thread_double();

public:
    Binocular_calibration *Bi = nullptr;

protected:
    void run();


signals:
    void Binocular_over();

private:
    QString Path_L;
    QString Path_R;
    cv::Size Boardsize;
    cv::Size Squaresize;

    int Method;

};

#endif //CAMERA_DEMO_CALIBRATION_THREAD_H
