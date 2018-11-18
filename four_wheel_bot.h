#ifndef FOUR_WHEEL_BOT_H
#define FOUR_WHEEL_BOT_H

#include <QDialog>

namespace Ui {
  class four_wheel_bot;
}

class four_wheel_bot : public QDialog
{
  Q_OBJECT

public:
  explicit four_wheel_bot(QWidget *parent = 0);
  ~four_wheel_bot();

private:
  Ui::four_wheel_bot *ui;
};

#endif // FOUR_WHEEL_BOT_H
