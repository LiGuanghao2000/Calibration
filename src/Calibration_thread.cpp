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

        } else if (Method == 3)
        {

        } else if (Method == 4)
        {

        }

    }
    Mo->calibration(Path.toStdString(), Boardsize, Squaresize);
    emit Monocular_over();
}