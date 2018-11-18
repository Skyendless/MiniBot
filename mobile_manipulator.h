#ifndef MOBILE_MANIPULATOR_H
#define MOBILE_MANIPULATOR_H

#include <QDialog>

namespace Ui {
  class mobile_manipulator;
}

class mobile_manipulator : public QDialog
{
  Q_OBJECT

public:
  explicit mobile_manipulator(QWidget *parent = 0);
  ~mobile_manipulator();

private:
  Ui::mobile_manipulator *ui;
};

#endif // MOBILE_MANIPULATOR_H
