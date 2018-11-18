#include "inchworm_like.h"
#include "ui_inchworm_like.h"

inchworm_like::inchworm_like(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::inchworm_like)
{
  ui->setupUi(this);
}

inchworm_like::~inchworm_like()
{
  delete ui;
}

void inchworm_like::on_pushButton_clicked()
{
    this->close();
}
