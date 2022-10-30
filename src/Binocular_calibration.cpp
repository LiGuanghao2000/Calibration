#include "Binocular_calibration.h"

Binocular_calibration::Binocular_calibration()
{

}
Binocular_calibration::~Binocular_calibration()
{

}

bool Binocular_calibration::Get_images(std::string pathL, std::string pathR)
{
    std::string imagename;
    std::ifstream fin(pathL);
    if (!fin.is_open())
    {
        std::cout << pathL << "打开错误" << std::endl;
        return false;
    }
    while (std::getline(fin, imagename))
    {
        cv::Mat image = cv::imread(imagename);
        if (image.empty())
        {
            std::cout << imagename << "  打开错误" << std::endl;
            return false;
            break;
        }
        else
        {
            std::cout << imagename << " 打开" << std::endl;
            images_L.push_back(image);
        }
    }
    fin.close();

    std::ifstream fin1(pathR);
    if (!fin1.is_open())
    {
        std::cout << pathR << "打开错误" << std::endl;
        return false;
    }
    while (std::getline(fin1, imagename))
    {
        cv::Mat image = cv::imread(imagename);
        if (image.empty())
        {
            std::cout << imagename << "  打开错误" << std::endl;
            return false;
            break;
        }
        else
        {
            std::cout << imagename << " 打开" << std::endl;
            images_R.push_back(image);
        }
    }
    fin1.close();
    return true;
}

void Binocular_calibration::Getobjectpoints(cv::Size boardsize, cv::Size squaresize)
{
    for (int i = 0; i < images_L.size(); i++)
    {
        std::vector<cv::Point3f> temppointset;
        for (int j = 0; j < boardsize.height; j++)
        {
            for (int k = 0; k < boardsize.width; k++)
            {
                cv::Point3f realpoint;
                realpoint.x = j * squaresize.width;
                realpoint.y = k * squaresize.height;
                realpoint.z = 0;
                temppointset.push_back(realpoint);
            }
        }
        objectpoints.push_back(temppointset);
    }
}

void Binocular_calibration::Calibration()
{
    cv::Size imagesize;
    imagesize.width = images_L[0].cols;
    imagesize.height = images_L[0].rows;
    calibrateCamera(objectpoints, imagepoints_L, imagesize, matrixl, distl, rvecsl, tvecsl, 0);
    calibrateCamera(objectpoints, imagepoints_R, imagesize, matrixr, distr, rvecsr, tvecsr, 0);

    stereoCalibrate(objectpoints, imagepoints_L, imagepoints_R, matrixl, distl, matrixr, distr, imagesize, R, T, E, F, cv::CALIB_USE_INTRINSIC_GUESS);

    stereoRectify(matrixl, distl, matrixr, distr, imagesize, R, T, R1, R2, P1, P2, Q, 0);
}

void Binocular_calibration::Dateprintf()
{
    if (matrixl.empty() || distl.empty()||Q.empty())
    {
        std::cout << "标定未完成" << std::endl;
        return;
    }
    std::cout << "M1" << std::endl << matrixl << std::endl;
    std::cout << "D1" << std::endl << distl << std::endl;
    std::cout << "M2" << std::endl << matrixr << std::endl;
    std::cout << "D2" << std::endl << distr << std::endl;
    std::cout << "R" << std::endl << R << std::endl;
    std::cout << "T" << std::endl << T << std::endl;
    std::cout << "E" << std::endl << E << std::endl;
    std::cout << "F" << std::endl << F << std::endl;
    std::cout << "Q" << std::endl << Q << std::endl;
    std::cout << "R1" << std::endl << R1 << std::endl;
    std::cout << "P1" << std::endl << P1 << std::endl;
    std::cout << "R2" << std::endl << R2 << std::endl;
    std::cout << "P2" << std::endl << P2 << std::endl;

}
bool Binocular_calibration::DateWriteyml(std::string path)
{
    if (matrixl.empty() || distl.empty() || Q.empty())
    {
        std::cout << "标定未完成" << std::endl;
        return false;
    }
    cv::FileStorage fs(path, cv::FileStorage::WRITE);
    fs << "M1" << matrixl;
    fs << "D1" << distl;
    fs << "M2" << matrixr;
    fs << "D2" << distr;
    fs << "R" << R;
    fs << "T" << T;
    fs << "E" << E;
    fs << "F" << F;
    fs << "Q" << Q;
    fs << "R1" << R1;
    fs << "P1" << P1;
    fs << "R2" << R2;
    fs << "P2" << P2;
    
    return true;
}

//标定总函数
void Binocular_calibration::calibration(std::string pathl, std::string pathr, cv::Size boardsize, cv::Size squaresize)
{
    if (!Get_images(pathl, pathr))
    {
        std::cout << "图片未加载成功" << std::endl;
        return;
    }
    double start = clock();
    Getimagespoints(images_L, images_R, boardsize);
    Getobjectpoints(boardsize, squaresize);
    Calibration();
    double stop = clock();
    usingtime=((double)(stop - start)) / CLK_TCK;
}

//极线矫正
std::vector<cv::Mat> Binocular_calibration::Get_Polar_rectification_images()
{
    if (matrixl.empty() || distl.empty() || Q.empty())
    {
        std::cout << "标定未完成" << std::endl;
        //return ;
    }

    cv::Size imagesize;
    imagesize.width = images_L[0].cols;
    imagesize.height = images_L[0].rows;

    cv::Mat map11, map12, map21, map22;
    initUndistortRectifyMap(matrixl, distl, R1, P1, imagesize, CV_16SC2, map11, map12);
    initUndistortRectifyMap(matrixr, distr, R2, P2, imagesize, CV_16SC2, map21, map22);

    for (int i = 0; i < images_L.size(); i++)
    {
        cv::Mat img1r, img2r;
        cv::Mat ximg1r, ximg2r;
        remap(images_L[i], img1r, map11, map12, cv::INTER_LINEAR);
        remap(images_R[i], img2r, map21, map22, cv::INTER_LINEAR);
        cv::Mat result;
        hconcat(img1r, img2r, result);
        for (int i = 0; i < 20; i++)
        {
            line(result, cv::Point(-1, i*100+100), cv::Point(result.cols, i * 100 + 100), cv::Scalar(0, 0, 255), 2, 8);
            //line(result, cv::Point(i * 100 + 100, -1), cv::Point(i * 100 + 100, result.rows), cv::Scalar(0, 0, 255), 2, 8);
        }
        images_vconcat.push_back(result);
    }
    return images_vconcat;
}

std::vector<cv::Mat> Binocular_calibration::Get_Original_images_L()
{
    return images_L;
}
std::vector<cv::Mat> Binocular_calibration::Get_Original_images_R()
{
    return images_R;
}

//棋盘格
std::vector<cv::Point2f> Chessboard_Bin::Getimagepoints(cv::Mat img, cv::Size boardsize)
{
    std::vector<cv::Point2f> points;
    cv::Mat im;
    cv::cvtColor(img, im, CV_RGB2GRAY);

    findChessboardCorners(im, boardsize, points);
    find4QuadCornerSubpix(im, points, cv::Size(3, 3));
    //drawChessboardCorners(im, boardsize, points, true);
    //mymutex.lock();
    //images_draw.push_back(img);
    //mymutex.unlock();
    std::cout << "检测中..." << std::endl;

    return points;
}
void Chessboard_Bin::Getimagespoints(std::vector<cv::Mat> imgsl, std::vector<cv::Mat> imgsr, cv::Size boardsize)
{
    if (imgsl.size() == 0)
    {
        std::cout << "左图片未加载成功" << std::endl;
        return;
    }
    if (imgsr.size() == 0)
    {
        std::cout << "右图片未加载成功" << std::endl;
        return;
    }
    for (int i = 0; i < imgsl.size(); i++)
    {
        results_L[i] = std::async(std::launch::async, &Chessboard_Bin::Getimagepoints, new Chessboard_Bin(), imgsl[i], boardsize);
        results_R[i] = std::async(std::launch::async, &Chessboard_Bin::Getimagepoints, new Chessboard_Bin(), imgsr[i], boardsize);
    }
    for (int i = 0; i < imgsl.size(); i++)
    {
        imagepoints_L.push_back(results_L[i].get());
        std::cout << "L  " << "第 " << i << " 张检测成功" << std::endl;
        imagepoints_R.push_back(results_R[i].get());
        std::cout << "R  " << "第 " << i << " 张检测成功" << std::endl;
    }
}

