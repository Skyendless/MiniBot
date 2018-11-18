#ifndef BIPED_H
#define BIPED_H

#include <QDialog>

namespace Ui {
  class biped;
}

class biped : public QDialog
{
  Q_OBJECT

public:
  explicit biped(QWidget *parent = 0);
  ~biped();

private:
  Ui::biped *ui;
};

#endif // BIPED_H
