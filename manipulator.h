#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QDialog>

namespace Ui {
  class manipulator;
}

class manipulator : public QDialog
{
  Q_OBJECT

public:
  explicit manipulator(QWidget *parent = 0);
  ~manipulator();

private slots:
  void on_pushButton_clicked();

private:
  Ui::manipulator *ui;
};

#endif // MANIPULATOR_H
