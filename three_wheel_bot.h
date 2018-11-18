#ifndef THREE_WHEEL_BOT_H
#define THREE_WHEEL_BOT_H

#include <QDialog>

namespace Ui {
  class three_wheel_bot;
}

class three_wheel_bot : public QDialog
{
  Q_OBJECT

public:
  explicit three_wheel_bot(QWidget *parent = 0);
  ~three_wheel_bot();

private:
  Ui::three_wheel_bot *ui;
};

#endif // THREE_WHEEL_BOT_H
