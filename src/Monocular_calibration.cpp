#include "Monocular_calibration.h"



Monocular_calibration::Monocular_calibration()
{

}

Monocular_calibration::~Monocular_calibration()
{

}
/*!
 * 获取图像
 * @param path 图像文件路径
 * @return
 */
bool Monocular_calibration::Get_images(std::string path)
{
    std::string imagename;
    std::ifstream fin(path);
    if (!fin.is_open())
    {
        std::cout << path << "打开错误" << std::endl;
        return false;
    }
    while (std::getline(fin, imagename))
    {
        cv::Mat image = cv::imread(imagename, cv::IMREAD_GRAYSCALE);
        if (image.empty())
        {
            std::cout << imagename << "  打开错误" << std::endl;
            return false;
            break;
        }
        else
        {
            std::cout << imagename << " 打开" << std::endl;
            images.push_back(image);
        }
    }
    return true;
}

std::vector<cv::Point2f> Getimagepoint(cv::Mat img, cv::Size boardsize)
{
    std::vector<cv::Point2f> points;

    return points;
}
/*!
 * 获取图像标定角点世界坐标
 * @param boardsize 角点排列
 * @param squaresize 角点距离
 */
void Monocular_calibration::Getobjectpoints(cv::Size boardsize,cv::Size squaresize)
{
    for (int i = 0; i < images.size(); i++)
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
/*!
 * 标定函数
 */
void Monocular_calibration::Calibration()
{
    cv::Size imagesize;
    imagesize.width = images[0].cols;
    imagesize.height = images[0].rows;
    calibrateCamera(objectpoints, imagepoints, imagesize, matrix, dist, rvecs, tvecs, 0);
}
/*!
 * @brief 输出标定数据
 */
void Monocular_calibration::Dateprintf()
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "标定未完成" << std::endl;
        return;
    }
    std::cout << "相机内参数矩阵" << std::endl << matrix << std::endl;
    std::cout << "畸变矩阵" << std::endl << dist << std::endl;
}
/*!
 * @brief 把数据写入到.yml
 * @param path 文件写入路径
 * @return
 */
bool Monocular_calibration::DateWriteyml(std::string path)
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "标定未完成" << std::endl;
        return false;
    }
    cv::FileStorage fs(path, cv::FileStorage::WRITE);
    fs << "M" << matrix;
    fs << "D" << dist;
    return true;
}
/*!
 * 误差分析
 * @param boardsize 角点size
 */
void Monocular_calibration::Error_analysis(cv::Size boardsize)
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "标定未完成" << std::endl;
        return;
    }
    std::cout << "开始评价标定结果………………\n" << std::endl;;
    int point_number = boardsize.width * boardsize.height;
    double total_err = 0.0; /* 所有图像的平均误差的总和 */
    double err = 0.0; /* 每幅图像的平均误差 */
    std::vector<cv::Point2f> image_points2; /* 保存重新计算得到的投影点 */
    std::cout << "\t每幅图像的标定误差：\n" << std::endl;;
    for (int i = 0; i < images.size(); i++)
    {
        std::vector<cv::Point3f> tempPointSet = objectpoints[i];
        /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
        projectPoints(tempPointSet, rvecs[i], tvecs[i], matrix, dist, image_points2);
        /* 计算新的投影点和旧的投影点之间的误差*/
        std::vector<cv::Point2f> tempImagePoint = imagepoints[i];
        cv::Mat tempImagePointMat = cv::Mat(1, tempImagePoint.size(), CV_32FC2);
        cv::Mat image_points2Mat = cv::Mat(1, image_points2.size(), CV_32FC2);
        for (int j = 0; j < tempImagePoint.size(); j++)
        {
            image_points2Mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points2[j].x, image_points2[j].y);
            tempImagePointMat.at<cv::Vec2f>(0, j) = cv::Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
        }
        err = norm(image_points2Mat, tempImagePointMat, cv::NORM_L2);
        total_err += err /= point_number;
        std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << std::endl;
        errors.push_back(err);
    }
    total_error = total_err;
    std::cout << "总体平均误差：" << total_err / images.size() << "像素" << std::endl;
    std::cout << "评价完成！" << std::endl;
}
/*!
 * 标定总函数
 * @param path 图像文件路径
 * @param boardsize 角点size
 * @param squaresize 角点距离
 */
void Monocular_calibration::calibration(std::string path, cv::Size boardsize, cv::Size squaresize)
{
    if (!Get_images(path))
    {
        std::cout << "图片未加载成功" << std::endl;
        return;
    }
    double start = clock();
    Getimagespoints(images, boardsize);
    Getobjectpoints(boardsize, squaresize);
    Calibration();
    double stop = clock();
    usingtime=((double)(stop - start)) / CLK_TCK;
}

//棋盘格
std::vector<cv::Point2f> Chessboard::Getimagepoints(cv::Mat img, cv::Size boardsize)
{
    mymutex.lock();
    images_draw.push_back(img);
    mymutex.unlock();

    std::vector<cv::Point2f> points;
    cv::Mat im = img;

    findChessboardCorners(img, boardsize, points);
    find4QuadCornerSubpix(img, points, cv::Size(3, 3));
    drawChessboardCorners(im, boardsize, points, true);

    std::cout << "角点检测成功" << std::endl;

    return points;
}

void Chessboard::Getimagespoints(std::vector<cv::Mat> imgs, cv::Size boardsize)
{
    /*for (int i = 0; i < imgs.size(); i++)
    {
        imagepoints.push_back(Getimagepoints(imgs[i], boardsize));
        std::cout << "第" << i + 1 << "张检测成功" << std::endl;
    }*/
    if (imgs.size() == 0)
    {
        std::cout << "图片未加载成功" << std::endl;
        return;
    }
    for (int i = 0; i < imgs.size(); i++)
    {
        results[i] = std::async(std::launch::async, &Chessboard::Getimagepoints,new Chessboard(),imgs[i], boardsize);
    }
    for (int i = 0; i < imgs.size(); i++)
    {
        imagepoints.push_back(results[i].get());
    }
}








