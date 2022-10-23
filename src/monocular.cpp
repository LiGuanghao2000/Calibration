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
  if(th_s!= nullptr)
  {
      delete th_s;
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
    th_s = new thread_single(images_path,cv::Size(11, 8), cv::Size(3, 3));
    connect(th_s,&thread_single::Monocular_over,this,&Monocular::Monocular_Done);
    th_s->start();
    ui->textEdit->append("Calibration...");
}
/*!
 * 标定完成
 */
void Monocular::Monocular_Done()
{
    ui->textEdit->append("Calibration done");
    double time=th_s->Mo->Get_usingtime();
    ui->textEdit->append("usingtime: "+QString::number(time));
    std::vector<double> errors;
    th_s->Mo->Error_analysis(cv::Size(11, 8));
    errors=th_s->Mo->Get_errors();
    for (int i = 0; i < errors.size(); ++i)
    {
        ui->textEdit->append("error " +QString::number(i)+" : "+QString::number(errors[i]));
    }

}











