#include <QApplication>
#include "headers/camera.h"
#include <QStyleFactory>
#include "qdebug.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

//  QStringList sty = QStyleFactory::keys();
//  foreach(QString st,sty)
//        {
//            qDebug()<<st;
//        }
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  camera w;
  w.show();
  return QApplication::exec();
}
