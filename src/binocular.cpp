//
// Created by 16663 on 2022/10/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_binocular.h" resolved

#include "binocular.h"
#include "forms/ui_binocular.h"

/*!
 * 构造函数
 * @param parent
 */
binocular::binocular(QWidget *parent) :
        QWidget(parent), ui(new Ui::binocular)
{
    ui->setupUi(this);

    init_mainwindow();
}
/*!
 * 析构函数
 */
binocular::~binocular()
{
    delete ui;
}
/*!
 * 关闭按钮按下
 */
void binocular::on_close_clicked()
{
    emit binocular_close();
    this->close();

}
/*!
 * 初始化窗口
 */
void binocular::init_mainwindow()
{
    setWindowTitle("单目标定");
    setFixedSize(1800,1000);
    ui->tabWidget->setTabText(0,"参数");
    ui->tabWidget->setTabText(1,"图像");

    ui->spinBox_wid->setValue(11);
    ui->spinBox_hei->setValue(8);
    ui->spinBox_sq->setValue(3);

    ui->radioButton_chess->setChecked(true);
}
/*!
 * 选择左相机文件按钮点击
 */
void binocular::on_pushButton_L_clicked()
{
    imagepath_L = QFileDialog::getOpenFileName(this);

    if (imagepath_L.isEmpty())
    {
        ui->textEdit->append("open images_L path error");
        return ;
    }
    else
    {
        ui->textEdit->append(imagepath_L);
        ui->lineEdit_L->setText(imagepath_L);
    }
}
/*!
 * 选择右文件按钮点击
 */
void binocular::on_pushButton_R_clicked()
{
    imagepath_R = QFileDialog::getOpenFileName(this);

    if (imagepath_L.isEmpty())
    {
        ui->textEdit->append("open images_L path error");
        return ;
    }
    else
    {
        ui->textEdit->append(imagepath_R);
        ui->lineEdit_R->setText(imagepath_R);
    }
}
/*!
 * 开始标定按钮按下
 */
void binocular::on_pushButton_ST_clicked()
{
    if(imagepath_L.isNull()|imagepath_R.isNull())
    {
        switch (QMessageBox::warning(this, tr("error"), tr("please check the input image path!"), QMessageBox::Cancel, QMessageBox::Cancel))
        {
            case QMessageBox::Cancel:
                return;
            default:
                break;
        }
    }
    save_path = QFileDialog::getExistingDirectory(this,"Calibration date save path");

    int k=0;
    if (ui->radioButton_chess->isChecked())
    {
        k=1;
    }else if (ui->radioButton_circle->isChecked())
    {
        k=2;
    } else if (ui->radioButton_ring->isChecked())
    {
        k=3;
    }else if (ui->radioButton_double_ring->isChecked())
    {
        k=4;
    }
    ui->textEdit->append("Calibration...");


}
