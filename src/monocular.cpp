//
// Created by 16663 on 2022/10/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Monocular.h" resolved

#include "monocular.h"
#include "forms/ui_Monocular.h"

/*!
 * 构造函数
 * @param parent
 */
Monocular::Monocular(QWidget *parent) :
    QWidget(parent), ui(new Ui::Monocular) {
  ui->setupUi(this);

  init_mianwindow();
}
/*!
 * 析构函数
 */
Monocular::~Monocular() {
  delete ui;
}
/*!
 * 主窗口初始化
 */
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
/*!
 * 关闭主窗口
 */
void Monocular::on_close_clicked()
{
  this->close();
  if(cal!= nullptr)
  {
      delete cal;
  }
  emit monocular_close();
}
/*!
 * 选择文件按钮按下
 */
void Monocular::on_pushButton_sec_clicked()
{
    images_path = QFileDialog::getOpenFileName(this);

    if (images_path.isEmpty())
    {
        ui->textEdit->append("open images path error");
        return ;
    }
    else
    {
        ui->textEdit->append(images_path);
        ui->lineEdit->setText(images_path);
    }
}
/*!
 * 开始标定
 */
void Monocular::on_pushButton_start_clicked()
{
    if(images_path.isNull())
    {
        switch (QMessageBox::warning(this, tr("error"), tr("please check the input image path!"), QMessageBox::Cancel, QMessageBox::Cancel))
        {
            case QMessageBox::Cancel:
                return;
            default:
                break;
        }
    }
    ui->textEdit->append("Calibration...");
    cal = new Chessboard();
    cal->calibration(images_path.toStdString(), cv::Size(11, 8), cv::Size(3, 3));

    ui->textEdit->append("Calibration done");
}












