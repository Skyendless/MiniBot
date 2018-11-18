#include "mobile_manipulator.h"
#include "ui_mobile_manipulator.h"

mobile_manipulator::mobile_manipulator(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::mobile_manipulator)
{
  ui->setupUi(this);
}

mobile_manipulator::~mobile_manipulator()
{
  delete ui;
}
