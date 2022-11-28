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


//  ui->pushButton_sec->setToolTip(s.toUtf8());
//  ui->pushButton_start->setToolTip(s.toUtf8());
//  ui->spinBox_sq->setToolTip("角点之间距离");
//  ui->spinBox_wid->setToolTip("角点的列数");
//  ui->spinBox_hei->setToolTip("角点行数");
//  ui->radioButton_chess->setToolTip("棋盘格");
//  ui->radioButton_ellipse->setToolTip("椭圆纠偏");
//  ui->radioButton_doublering->setToolTip("双圆环");
//  ui->radioButton_circle->setToolTip("圆盘");

  ui->label_help->setText("Select the. txt file containing the image path and click Start Calibration");

  ui->radioButton_chess->setChecked(true);

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
    save_path = QFileDialog::getExistingDirectory(this,"Calibration date save path");
    int k=0;
    if (ui->radioButton_chess->isChecked())
    {
        k=1;
    }else if (ui->radioButton_circle->isChecked())
    {
        k=2;
    } else if (ui->radioButton_doublering->isChecked())
    {
        k=3;
    }else if (ui->radioButton_ellipse->isChecked())
    {
        k=4;
    }
    th_s = new thread_single(images_path,cv::Size(ui->spinBox_wid->text().toInt(), ui->spinBox_hei->text().toInt()), cv::Size(ui->spinBox_sq->text().toInt(), ui->spinBox_sq->text().toInt()),k);
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
    if (save_path.isNull())
    {

    }else
    {
        th_s->Mo->DateWriteyml(save_path.toStdString()+"/Calibration_Date.yml");
        ui->textEdit->append(save_path+"/Calibration_Date.yml");
    }
    double time=th_s->Mo->Get_usingtime();
    ui->textEdit->append("usingtime: "+QString::number(time));
    ui->textEdit->append("error: "+QString::number(th_s->Mo->Get_error()));

    std::string imagename;
    std::ifstream fin(save_path.toStdString()+"/Calibration_Date.yml");
    if (!fin.is_open())
    {
        ui->textEdit->append("Calibration_Date.yml open error");
    }
    while (std::getline(fin, imagename))
    {
        ui->textEdit->append(QString::fromStdString(imagename));
    }

    std::vector<double> errors;
//    th_s->Mo->Error_analysis(cv::Size(11, 8));
//    errors=th_s->Mo->Get_errors();
//    for (int i = 0; i < errors.size(); ++i)
//    {
//        ui->textEdit->append("error " +QString::number(i)+" : "+QString::number(errors[i]));
//    }
//    ui->textEdit->append("avererror: "+QString::number(th_s->Mo->Get_totalerror()/errors.size()));

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
/*!
 * 棋盘格标定方法按钮点击
 */
void Monocular::on_radioButton_chess_clicked()
{
    ui->spinBox_wid->setValue(11);
    ui->spinBox_hei->setValue(8);
    ui->spinBox_sq->setValue(3);
}
/*!
 * 圆盘标定方法按钮点击
 */
void Monocular::on_radioButton_circle_clicked()
{
    ui->spinBox_wid->setValue(7);
    ui->spinBox_hei->setValue(5);
    ui->spinBox_sq->setValue(9);
}








