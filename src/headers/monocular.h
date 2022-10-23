//
// Created by 16663 on 2022/10/23.
//

#ifndef CAMERA_DEMO__MONOCULAR_H_
#define CAMERA_DEMO__MONOCULAR_H_

#include <QWidget>
#include "opencv2/opencv.hpp"
#include "qfiledialog.h"
#include "iostream"
#include "qdebug.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qmessagebox.h"
#include "Monocular_calibration.h"
#include "Calibration_thread.h"
#include "qtreewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Monocular; }
QT_END_NAMESPACE

class Monocular : public QWidget {
 Q_OBJECT

 public:
  explicit Monocular(QWidget *parent = nullptr);
  ~Monocular() override;
 private:
  void init_mianwindow();

 private slots:
  void on_close_clicked();
  void on_pushButton_sec_clicked();
  void on_pushButton_start_clicked();
  void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
  void Monocular_Done();

 signals:
  void monocular_close();

 private:
    Ui::Monocular *ui;
    thread_single *th_s= nullptr;
    QString images_path;
    QString save_path;
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> images_C;

};

#endif //CAMERA_DEMO__MONOCULAR_H_
