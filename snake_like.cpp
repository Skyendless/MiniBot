#include "snake_like.h"
#include "ui_snake_like.h"

snake_like::snake_like(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::snake_like)
{
  ui->setupUi(this);
}

snake_like::~snake_like()
{
  delete ui;
}
