#ifndef TWO_WHEEL_BOT_H
#define TWO_WHEEL_BOT_H

#include <QDialog>

namespace Ui {
  class two_wheel_bot;
}

class two_wheel_bot : public QDialog
{
  Q_OBJECT

public:
  explicit two_wheel_bot(QWidget *parent = 0);
  ~two_wheel_bot();

private:
  Ui::two_wheel_bot *ui;
};

#endif // TWO_WHEEL_BOT_H
