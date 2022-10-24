//
// Created by 16663 on 2022/10/24.
//

#ifndef CAMERA_DEMO_BINOCULAR_H
#define CAMERA_DEMO_BINOCULAR_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class binocular;
}
QT_END_NAMESPACE

class binocular : public QWidget
{
Q_OBJECT

public:
    explicit binocular(QWidget *parent = nullptr);

    ~binocular() override;

public slots:
    void on_close_clicked();

signals:
    void binocular_close();

private:
    Ui::binocular *ui;
};


#endif //CAMERA_DEMO_BINOCULAR_H
