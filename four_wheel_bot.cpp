#include "four_wheel_bot.h"
#include "ui_four_wheel_bot.h"

four_wheel_bot::four_wheel_bot(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::four_wheel_bot)
{
  ui->setupUi(this);
}

four_wheel_bot::~four_wheel_bot()
{
  delete ui;
}
