#include "biped.h"
#include "ui_biped.h"

biped::biped(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::biped)
{
  ui->setupUi(this);
}

biped::~biped()
{
  delete ui;
}
