//
// Created by 16663 on 2022/10/24.
//

#ifndef CAMERA_DEMO_BINOCULAR_H
#define CAMERA_DEMO_BINOCULAR_H

#include <QWidget>
#include "qfiledialog.h"
#include "qmessagebox.h"

#include "opencv2/opencv.hpp"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class binocular;
}
QT_END_NAMESPACE

class binocular : public QWidget
{
Q_OBJECT

public:
    explicit binocular(QWidget *parent = nullptr);

    ~binocular() override;



public slots:
    void on_close_clicked();
    void on_pushButton_L_clicked();
    void on_pushButton_R_clicked();
    void on_pushButton_ST_clicked();

signals:
    void binocular_close();

private:
    void init_mainwindow();

private:
    Ui::binocular *ui;
    QString imagepath_L;
    QString imagepath_R;
    QString save_path;
    std::vector<cv::Mat> images_L;
    std::vector<cv::Mat> images_R;
};


#endif //CAMERA_DEMO_BINOCULAR_H
