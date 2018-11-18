#include "four_legged.h"
#include "ui_four_legged.h"

four_legged::four_legged(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::four_legged)
{
  ui->setupUi(this);
}

four_legged::~four_legged()
{
  delete ui;
}
