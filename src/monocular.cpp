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
    save_path = QFileDialog::getExistingDirectory(this);
    th_s = new thread_single(images_path,cv::Size(ui->spinBox_wid->text().toInt(), ui->spinBox_hei->text().toInt()), cv::Size(ui->spinBox_sq->text().toInt(), ui->spinBox_sq->text().toInt()));
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
    th_s->Mo->DateWriteyml(save_path.toStdString()+"/Calibration_Date.yml");
    ui->textEdit->append(save_path+"/Calibration_Date.yml");
    double time=th_s->Mo->Get_usingtime();
    ui->textEdit->append("usingtime: "+QString::number(time));

    std::vector<double> errors;
    th_s->Mo->Error_analysis(cv::Size(11, 8));
    errors=th_s->Mo->Get_errors();
    for (int i = 0; i < errors.size(); ++i)
    {
        ui->textEdit->append("error " +QString::number(i)+" : "+QString::number(errors[i]));
    }
    ui->textEdit->append("avererror: "+QString::number(th_s->Mo->Get_totalerror()/errors.size()));

    images=th_s->Mo->Get_images_orgin();
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "images_Calibration");
    ui->treeWidget->addTopLevelItem(item);
    for (int i = 0; i < images.size(); ++i)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << QString::number(i));
        item->addChild(child);
    }

//    images_C=th_s->Mo->Get_images_C();
//    QTreeWidgetItem* item1 = new QTreeWidgetItem(QStringList() << "images_Calibration");
//    ui->treeWidget->addTopLevelItem(item1);
//    for (int i = 0; i < images_C.size(); ++i)
//    {
//        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << QString::number(i));
//        item1->addChild(child);
//    }
}
/*!
 * 查看标定图片
 * @param item
 * @param column
 */
void Monocular::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->parent()->text(column) == "images_Calibration")
    {
        int k= item->text(column).toInt();
        //qDebug() << workthread->imgs_b.size();
        cv::Mat image = images[k];
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        if (image.empty())
            return;
        QImage img=QImage((const unsigned char*)(image.data),image.cols, image.rows, QImage::Format_RGB888);
        ui->label_image_title->setText(QString::number(k));
        ui->label_images->setPixmap(QPixmap::fromImage(img.scaled(ui->label_images->size(), Qt::KeepAspectRatio)));
    }
}









