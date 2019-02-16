#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setMinimumSize(this->size());
  this->setMaximumSize(this->size());
  ui->statusBar->showMessage(tr(" Version : 0.1 "));
}


MainWindow::~MainWindow()
{
  delete ui;
}

char state= 0 ;

void MainWindow::on_pushButton_quit_clicked()
{
    this->close();
}

int bot_selet = 0;

void MainWindow::on_pushButton_confirm_clicked()
{
  switch (state) {
    case 1: this->close();
            manipulatorbot.show();
            manipulatorbot.exec();
            this->show();
      break;
    case 2: bot_selet = 2;
            this->close();
            four_wheelbot.show();
            four_wheelbot.exec();
            this->show();
      break;
    case 3: bot_selet = 3;
            this->close();
            four_wheelbot.show();
            four_wheelbot.exec();
            this->show();
      break;
    case 4: this->close();
            inchworm_likebot.show();
            inchworm_likebot.exec();
            this->show();
      break;
    case 5: this->close();
            snake_likebot.show();
            snake_likebot.exec();
            this->show();
      break;
    case 6: this->close();
            four_leggedbot.show();
            four_leggedbot.exec();
            this->show();
      break;
    case 7: this->close();
            bipedbot.show();
            bipedbot.exec();
            this->show();
      break;
    case 8: bot_selet = 4;
            this->close();
            four_wheelbot.show();
            four_wheelbot.exec();
            this->show();
      break;
    case 9: this->close();
            mobile_manipulatorbot.show();
            mobile_manipulatorbot.exec();
            this->show();
      break;

    default:QMessageBox::information(this, tr("Tip"), tr("Please choose a robot to control!\n"));
      break;
    }
}

void MainWindow::on_Button_1_clicked()
{
  state=1;
  clean_button_back();
  ui->Button_1->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_2_clicked()
{
  state=2;
  clean_button_back();
  ui->Button_2->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_3_clicked()
{
  state=3;
  clean_button_back();
  ui->Button_3->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_4_clicked()
{
  state=4;
  clean_button_back();
  ui->Button_4->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_5_clicked()
{
  state=5;
  clean_button_back();
  ui->Button_5->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_6_clicked()
{
  state=6;
  clean_button_back();
  ui->Button_6->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_7_clicked()
{
  state=7;
  clean_button_back();
  ui->Button_7->setStyleSheet("background-color: rgb(255, 165, 0);");
}

void MainWindow::on_Button_8_clicked()
{
  state=8;
  clean_button_back();
  ui->Button_8->setStyleSheet("background-color: rgb(255, 165, 0);");

}

void MainWindow::on_Button_9_clicked()
{
  state=9;
  clean_button_back();
  ui->Button_9->setStyleSheet("background-color: rgb(255, 165, 0);");

}

void MainWindow::clean_button_back()
{
  ui->Button_1->setStyleSheet("background-image:url(0)");
  ui->Button_2->setStyleSheet("background-image:url(0)");
  ui->Button_3->setStyleSheet("background-image:url(0)");
  ui->Button_4->setStyleSheet("background-image:url(0)");
  ui->Button_5->setStyleSheet("background-image:url(0)");
  ui->Button_6->setStyleSheet("background-image:url(0)");
  ui->Button_7->setStyleSheet("background-image:url(0)");
  ui->Button_8->setStyleSheet("background-image:url(0)");
  ui->Button_9->setStyleSheet("background-image:url(0)");
}
