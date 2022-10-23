//
// Created by 16663 on 2022/10/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_camera.h" resolved

#include "headers/camera.h"
#include "forms/ui_camera.h"

camera::camera(QWidget *parent) :
    QWidget(parent), ui(new Ui::camera) {
  ui->setupUi(this);
}

camera::~camera() {
  delete ui;
}

void camera::init_mainwindow()
{

  setWindowTitle("model");
  setFixedSize(QSize(500, 290));
  QImage image("../sysinputimages/title.jpg");

  ui->label_image->setPixmap(QPixmap::fromImage(image.scaled(ui->label_image->size(), Qt::KeepAspectRatio)));
  //ui->label_name->setText("lj");
  ui->label_name->setText(tr("name:LGH"));
  ui->label_time->setText("time:2022.7.3");
}
/**
 * @brief  显示主窗口
 * */
void camera::show_mainwindow()
{
  this->show();
}
/*!
 * @brief 关闭主窗口
 */
void camera::on_close_clicked()
{
  this->close();
}
/*!
 * @brief 显示单目标定窗口
 */
void camera::on_pushButtonsingle_clicked()
{
  monocular_=new Monocular();
  connect(monocular_,&Monocular::monocular_close,this,&camera::show_mainwindow);
  monocular_->show();
  this->hide();
}
/*!
 * @brief 显示双目标定窗口
 */
void camera::on_pushButtondouble_clicked()
{
}
