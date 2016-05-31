#include <QApplication>

#include "MupperWindow.h"

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  MupperWindow w;
  w.show();
  return a.exec();
}
