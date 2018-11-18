#ifndef INCHWORM_LIKE_H
#define INCHWORM_LIKE_H

#include <QDialog>

namespace Ui {
  class inchworm_like;
}

class inchworm_like : public QDialog
{
  Q_OBJECT

public:
  explicit inchworm_like(QWidget *parent = 0);
  ~inchworm_like();

private slots:
  void on_pushButton_clicked();

private:
  Ui::inchworm_like *ui;
};

#endif // INCHWORM_LIKE_H
