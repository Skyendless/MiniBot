#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QScreen>
#include <QRect>
#include <manipulator.h>
#include <inchworm_like.h>
#include <four_legged.h>
#include <four_wheel_bot.h>
#include <snake_like.h>
#include <biped.h>
#include <mobile_manipulator.h>
#include <three_wheel_bot.h>
#include <two_wheel_bot.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_pushButton_quit_clicked();

  void on_pushButton_confirm_clicked();

  void on_Button_1_clicked();

  void on_Button_4_clicked();

  void on_Button_5_clicked();

  void on_Button_2_clicked();

  void on_Button_6_clicked();

  void on_Button_7_clicked();

  void on_Button_3_clicked();

  void on_Button_8_clicked();

  void on_Button_9_clicked();

private:
  Ui::MainWindow *ui;

  manipulator manipulatorbot;
  inchworm_like inchworm_likebot;
  biped bipedbot;
  snake_like snake_likebot;
  four_legged four_leggedbot;
  two_wheel_bot two_wheelbot;
  three_wheel_bot three_wheelbot;
  four_wheel_bot four_wheelbot;
  mobile_manipulator mobile_manipulatorbot;

  void clean_button_back();
};

#endif // MAINWINDOW_H
