//
// Created by 16663 on 2022/10/23.
//

#ifndef CAMERA_DEMO__MONOCULAR_H_
#define CAMERA_DEMO__MONOCULAR_H_

#include <QWidget>
#include "qfiledialog.h"
#include "iostream"
#include "qdebug.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qmessagebox.h"
#include "Monocular_calibration.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Monocular; }
QT_END_NAMESPACE

class Monocular : public QWidget {
 Q_OBJECT

 public:
  explicit Monocular(QWidget *parent = nullptr);
  ~Monocular() override;
 private:
  void init_mianwindow();

 private slots:
  void on_close_clicked();
  void on_pushButton_sec_clicked();
  void on_pushButton_start_clicked();

 signals:
  void monocular_close();

 private:
    Ui::Monocular *ui;
    Monocular_calibration *cal= nullptr;
    QString images_path;
};

#endif //CAMERA_DEMO__MONOCULAR_H_
