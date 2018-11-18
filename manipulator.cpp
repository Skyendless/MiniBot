#include "manipulator.h"
#include "ui_manipulator.h"

manipulator::manipulator(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::manipulator)
{
  ui->setupUi(this);
}

manipulator::~manipulator()
{
  delete ui;
}

void manipulator::on_pushButton_clicked()
{
    this->close();
}
