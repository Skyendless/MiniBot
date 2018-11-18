#include "two_wheel_bot.h"
#include "ui_two_wheel_bot.h"

two_wheel_bot::two_wheel_bot(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::two_wheel_bot)
{
  ui->setupUi(this);
}

two_wheel_bot::~two_wheel_bot()
{
  delete ui;
}
