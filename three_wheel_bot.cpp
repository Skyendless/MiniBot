#include "three_wheel_bot.h"
#include "ui_three_wheel_bot.h"

three_wheel_bot::three_wheel_bot(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::three_wheel_bot)
{
  ui->setupUi(this);
}

three_wheel_bot::~three_wheel_bot()
{
  delete ui;
}
