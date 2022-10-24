//
// Created by 16663 on 2022/10/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_binocular.h" resolved

#include "binocular.h"
#include "forms/ui_binocular.h"


binocular::binocular(QWidget *parent) :
        QWidget(parent), ui(new Ui::binocular)
{
    ui->setupUi(this);
}

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




