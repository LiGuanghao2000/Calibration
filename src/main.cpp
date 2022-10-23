#include <QApplication>
#include "headers/camera.h"
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  camera w;
  w.show();
  return QApplication::exec();
}
