#ifndef SNAKE_LIKE_H
#define SNAKE_LIKE_H

#include <QDialog>

namespace Ui {
  class snake_like;
}

class snake_like : public QDialog
{
  Q_OBJECT

public:
  explicit snake_like(QWidget *parent = 0);
  ~snake_like();

private:
  Ui::snake_like *ui;
};

#endif // SNAKE_LIKE_H
