#ifndef FOUR_LEGGED_H
#define FOUR_LEGGED_H

#include <QDialog>

namespace Ui {
  class four_legged;
}

class four_legged : public QDialog
{
  Q_OBJECT

public:
  explicit four_legged(QWidget *parent = 0);
  ~four_legged();

private:
  Ui::four_legged *ui;
};

#endif // FOUR_LEGGED_H
