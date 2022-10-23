//
// Created by 16663 on 2022/10/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Monocular.h" resolved

#include "monocular.h"
#include "forms/ui_Monocular.h"

Monocular::Monocular(QWidget *parent) :
    QWidget(parent), ui(new Ui::Monocular) {
  ui->setupUi(this);

  init_mianwindow();
}

Monocular::~Monocular() {
  delete ui;
}

void Monocular::init_mianwindow()
{
  setWindowTitle("单目标定");
  setFixedSize(1800,1000);
  ui->tabWidget_main->setTabText(0,"参数");
  ui->tabWidget_main->setTabText(1,"图像");

  ui->spinBox_wid->setValue(11);
  ui->spinBox_hei->setValue(8);
  ui->spinBox_sq->setValue(3);



}

void Monocular::on_close_clicked()
{
  this->close();
  emit monocular_close();
}
