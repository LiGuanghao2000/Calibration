//
// Created by 16663 on 2022/10/23.
//

#ifndef CAMERA_DEMO__CAMERA_H_
#define CAMERA_DEMO__CAMERA_H_

#include <QWidget>
#include "iostream"
#include "qdebug.h"
#include "monocular.h"

QT_BEGIN_NAMESPACE
namespace Ui { class camera; }
QT_END_NAMESPACE

class camera : public QWidget {
 Q_OBJECT

 public:
  explicit camera(QWidget *parent = nullptr);
  ~camera() override;

 private:
  void init_mainwindow();
  void show_mainwindow();

 private slots:
  void on_close_clicked();
  void on_pushButtonsingle_clicked();
  void on_pushButtondouble_clicked();

 private:
  Ui::camera *ui;
  Monocular *monocular_;
};

#endif //CAMERA_DEMO__CAMERA_H_
