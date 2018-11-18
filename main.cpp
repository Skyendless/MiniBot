#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  QScreen *screen=QGuiApplication::primaryScreen ();
  QRect mm=screen->availableGeometry() ;
  w.move((mm.width() - w.width())/2,(mm.height() - w.height())/2);
  w.setMinimumSize(w.size());
  w.setMaximumSize(w.size());
  w.show();

  return a.exec();
}
