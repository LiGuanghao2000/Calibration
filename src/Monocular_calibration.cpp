#include "Monocular_calibration.h"



Monocular_calibration::Monocular_calibration()
{

}

Monocular_calibration::~Monocular_calibration()
{

}
/*!
 * ��ȡͼ��
 * @param path ͼ���ļ�·��
 * @return
 */
bool Monocular_calibration::Get_images(std::string path)
{
    std::string imagename;
    std::ifstream fin(path);
    if (!fin.is_open())
    {
        std::cout << path << "�򿪴���" << std::endl;
        return false;
    }
    while (std::getline(fin, imagename))
    {
        cv::Mat image = cv::imread(imagename, cv::IMREAD_GRAYSCALE);
        if (image.empty())
        {
            std::cout << imagename << "  �򿪴���" << std::endl;
            return false;
            break;
        }
        else
        {
            std::cout << imagename << " ��" << std::endl;
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
 * ��ȡͼ��궨�ǵ���������
 * @param boardsize �ǵ�����
 * @param squaresize �ǵ����
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
 * �궨����
 */
void Monocular_calibration::Calibration()
{
    cv::Size imagesize;
    imagesize.width = images[0].cols;
    imagesize.height = images[0].rows;
    calibrateCamera(objectpoints, imagepoints, imagesize, matrix, dist, rvecs, tvecs, 0);
}
/*!
 * @brief ����궨����
 */
void Monocular_calibration::Dateprintf()
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "�궨δ���" << std::endl;
        return;
    }
    std::cout << "����ڲ�������" << std::endl << matrix << std::endl;
    std::cout << "�������" << std::endl << dist << std::endl;
}
/*!
 * @brief ������д�뵽.yml
 * @param path �ļ�д��·��
 * @return
 */
bool Monocular_calibration::DateWriteyml(std::string path)
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "�궨δ���" << std::endl;
        return false;
    }
    cv::FileStorage fs(path, cv::FileStorage::WRITE);
    fs << "M" << matrix;
    fs << "D" << dist;
    return true;
}
/*!
 * ������
 * @param boardsize �ǵ�size
 */
void Monocular_calibration::Error_analysis(cv::Size boardsize)
{
    if (matrix.empty() || dist.empty())
    {
        std::cout << "�궨δ���" << std::endl;
        return;
    }
    std::cout << "��ʼ���۱궨���������������\n" << std::endl;;
    int point_number = boardsize.width * boardsize.height;
    double total_err = 0.0; /* ����ͼ���ƽ�������ܺ� */
    double err = 0.0; /* ÿ��ͼ���ƽ����� */
    std::vector<cv::Point2f> image_points2; /* �������¼���õ���ͶӰ�� */
    std::cout << "\tÿ��ͼ��ı궨��\n" << std::endl;;
    for (int i = 0; i < images.size(); i++)
    {
        std::vector<cv::Point3f> tempPointSet = objectpoints[i];
        /* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */
        projectPoints(tempPointSet, rvecs[i], tvecs[i], matrix, dist, image_points2);
        /* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
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
        std::cout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << std::endl;
        errors.push_back(err);
    }
    total_error = total_err;
    std::cout << "����ƽ����" << total_err / images.size() << "����" << std::endl;
    std::cout << "������ɣ�" << std::endl;
}
/*!
 * �궨�ܺ���
 * @param path ͼ���ļ�·��
 * @param boardsize �ǵ�size
 * @param squaresize �ǵ����
 */
void Monocular_calibration::calibration(std::string path, cv::Size boardsize, cv::Size squaresize)
{
    if (!Get_images(path))
    {
        std::cout << "ͼƬδ���سɹ�" << std::endl;
        return;
    }
    double start = clock();
    Getimagespoints(images, boardsize);
    Getobjectpoints(boardsize, squaresize);
    Calibration();
    double stop = clock();
    usingtime=((double)(stop - start)) / CLK_TCK;
}

//���̸�
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

    std::cout << "�ǵ���ɹ�" << std::endl;

    return points;
}

void Chessboard::Getimagespoints(std::vector<cv::Mat> imgs, cv::Size boardsize)
{
    /*for (int i = 0; i < imgs.size(); i++)
    {
        imagepoints.push_back(Getimagepoints(imgs[i], boardsize));
        std::cout << "��" << i + 1 << "�ż��ɹ�" << std::endl;
    }*/
    if (imgs.size() == 0)
    {
        std::cout << "ͼƬδ���سɹ�" << std::endl;
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








