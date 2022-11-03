//
// Created by 16663 on 2022/10/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_camera.h" resolved

#include "headers/camera.h"
#include "forms/ui_camera.h"

/*!
 *
 * @param parent
 */
camera::camera(QWidget *parent) :
    QWidget(parent), ui(new Ui::camera) {
  ui->setupUi(this);
    init_mainwindow();
}

/*!
 * @brief camera析构函数
 */
camera::~camera() {
  delete ui;
}
/*!
 * @brief 主窗口初始化
 */
void camera::init_mainwindow()
{
  setWindowTitle("model");
  setFixedSize(QSize(500, 290));
  QImage image("../sysinputimages/title.png");
    if (image.isNull())
    {
        std::cout<<"error"<<std::endl;
    } else
    {
        std::cout<<"open"<<std::endl;
    }

  ui->label_image->setPixmap(QPixmap::fromImage(image.scaled(ui->label_image->size(), Qt::KeepAspectRatio)));

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
    binocular_ = new binocular();
    connect(binocular_,&binocular::binocular_close, this,&camera::show_mainwindow);
    binocular_->show();
    this->hide();
}
