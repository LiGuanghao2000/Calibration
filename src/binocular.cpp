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

    ui->tabWidget->setCurrentIndex(0);

    ui->tabWidget->setTabText(0,"参数");
    ui->tabWidget->setTabText(1,"图像");

    ui->spinBox_wid->setValue(11);
    ui->spinBox_hei->setValue(8);
    ui->spinBox_sq->setValue(3);

    ui->radioButton_chess->setChecked(true);

    ui->label_help->setText("Select the. txt file containing the image path and click Start Calibration");

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

    if (th_d == nullptr)
    {
        th_d = new thread_double(imagepath_L,imagepath_R,cv::Size(ui->spinBox_wid->text().toInt(), ui->spinBox_hei->text().toInt()),cv::Size(ui->spinBox_sq->text().toInt(), ui->spinBox_sq->text().toInt()),k);
        connect(th_d,&thread_double::Binocular_over,this,&binocular::Binocular_done);
    }
    th_d->start();

    ui->textEdit->append("Calibration...");
}
/*!
 * 标定完成
 */
void binocular::Binocular_done()
{
    ui->textEdit->append("Calibration done");
    ui->textEdit->append("usingtime :"+QString::number(th_d->Bi->Get_usingtime()));
    if (save_path.isNull())
    {

    }else
    {
        th_d->Bi->DateWriteyml(save_path.toStdString()+"/Calibration_Date.yml");
        ui->textEdit->append(save_path+"/Calibration_Date.yml");
    }

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

    images_L=th_d->Bi->Get_Original_images_L();
    images_R=th_d->Bi->Get_Original_images_R();
    images_E=th_d->Bi->Get_Polar_rectification_images();
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "images_L");
    ui->treeWidget->addTopLevelItem(item);
    for (int i = 0; i < images_L.size(); ++i)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << QString::number(i));
        item->addChild(child);
    }
    item = new QTreeWidgetItem(QStringList() << "images_R");
    ui->treeWidget->addTopLevelItem(item);
    for (int i = 0; i < images_R.size(); ++i)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << QString::number(i));
        item->addChild(child);
    }
    item = new QTreeWidgetItem(QStringList() << "images_E");
    ui->treeWidget->addTopLevelItem(item);
    for (int i = 0; i < images_E.size(); ++i)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << QString::number(i));
        item->addChild(child);
    }
}
/*!
 * treewidget点击
 * @param item
 * @param column
 */
void binocular::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->parent()->text(column) == "images_L")
    {
        int k= item->text(column).toInt();
        //qDebug() << workthread->imgs_b.size();
        cv::Mat image = images_L[k];
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        if (image.empty())
            return;
        QImage img=QImage((const unsigned char*)(image.data),image.cols, image.rows, QImage::Format_RGB888);
        ui->label_title->setText("images_L : "+QString::number(k));
        ui->label_image->setPixmap(QPixmap::fromImage(img.scaled(ui->label_image->size(), Qt::KeepAspectRatio)));
    }
    if (item->parent()->text(column) == "images_R")
    {
        int k= item->text(column).toInt();
        //qDebug() << workthread->imgs_b.size();
        cv::Mat image = images_R[k];
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        if (image.empty())
            return;
        QImage img=QImage((const unsigned char*)(image.data),image.cols, image.rows, QImage::Format_RGB888);
        ui->label_title->setText("images_R : "+QString::number(k));
        ui->label_image->setPixmap(QPixmap::fromImage(img.scaled(ui->label_image->size(), Qt::KeepAspectRatio)));
    }
    if (item->parent()->text(column) == "images_E")
    {
        int k= item->text(column).toInt();
        //qDebug() << workthread->imgs_b.size();
        cv::Mat image = images_E[k];
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        if (image.empty())
            return;
        QImage img=QImage((const unsigned char*)(image.data),image.cols, image.rows, QImage::Format_RGB888);
        ui->label_title->setText("images_E : "+QString::number(k));
        ui->label_image->setPixmap(QPixmap::fromImage(img.scaled(ui->label_image->size(), Qt::KeepAspectRatio)));
    }
}
