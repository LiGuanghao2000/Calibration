//
// Created by 16663 on 2022/10/24.
//

#ifndef CAMERA_DEMO_BINOCULAR_H
#define CAMERA_DEMO_BINOCULAR_H

#include <QWidget>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qtreewidget.h"

#include "Calibration_thread.h"

#include "opencv2/opencv.hpp"

#include "fstream"


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
    void on_radioButton_chess_clicked();
    void on_radioButton_circle_clicked();
    void on_pushButton_L_clicked();
    void on_pushButton_R_clicked();
    void on_pushButton_ST_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void Binocular_done();

signals:
    void binocular_close();

private:
    void init_mainwindow();

private:
    Ui::binocular *ui;
    thread_double *th_d = nullptr;
    QString imagepath_L;
    QString imagepath_R;
    QString save_path;
    std::vector<cv::Mat> images_L;
    std::vector<cv::Mat> images_R;
    std::vector<cv::Mat> images_E;
};


#endif //CAMERA_DEMO_BINOCULAR_H
