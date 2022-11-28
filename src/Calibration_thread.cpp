//
// Created by 16663 on 2022/10/23.
//

#include "Calibration_thread.h"
/*!
 * 构造函数
 * @param path 图片路径
 * @param mo 单目标定函数对象
 */
thread_single::thread_single(QString path, cv::Size boardsize,cv::Size squaresize,int method)
{
    Path =path;
    Boardsize = boardsize;
    Squaresize = squaresize;
    Method = method;
}
/*!
 * 析构函数
 */
thread_single::~thread_single()
{
    if (Mo!= nullptr)
    {
        delete Mo;
    }
}
/*!
 * 重写线程函数
 */
void thread_single::run()
{
    if(Mo == nullptr)
    {
        if (Method == 1)
        {
            Mo = new Chessboard();
        } else if (Method == 2)
        {
            Mo = new Circle();
        } else if (Method == 3)
        {
            //Mo =new Double_ring();
        } else if (Method == 4)
        {

        }

    }
    Mo->calibration(Path.toStdString(), Boardsize, Squaresize);
    emit Monocular_over();
}
/*!
 * 构造函数
 * @param path_L 左相机图像文件路径
 * @param path_R 右相机图像文件路径
 * @param boardsize 角点尺寸
 * @param squaresize 角点距离
 * @param method 标定方法
 */
thread_double::thread_double(QString path_L, QString path_R, cv::Size boardsize, cv::Size squaresize, int method)
{
    Path_L = path_L;
    Path_R = path_R;
    Boardsize = boardsize;
    Squaresize = squaresize;
    Method = method;
}
/*!
 * 析构函数
 */
thread_double::~thread_double() noexcept
{
    if(Bi!= nullptr)
    {
        delete Bi;
    }
}

void thread_double::run()
{
    if(Bi == nullptr)
    {
        if (Method == 1)
        {
            Bi = new Chessboard_Bin();
        } else if (Method == 2)
        {
            Bi = new Circle_Bin();
        } else if (Method == 3)
        {

        } else if (Method == 4)
        {

        }
    }
    Bi->calibration(Path_L.toStdString(),Path_R.toStdString(),Boardsize,Squaresize);
    emit Binocular_over();
}


