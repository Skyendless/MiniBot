#ifndef FOUR_WHEEL_BOT_H
#define FOUR_WHEEL_BOT_H

#include <QDialog>

#include <QDebug>
#include <QtGui>
#include <QProcess>
#include <QMessageBox>

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
  class four_wheel_bot;
}

class four_wheel_bot : public QDialog
{
  Q_OBJECT

public:
  explicit four_wheel_bot(QWidget *parent = 0);

  void first_data(void);

  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);

  ~four_wheel_bot();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_remote_clicked();

  void on_pushButton_mapping_clicked();

  void readData();

  void handleError(QSerialPort::SerialPortError error);

  void on_readoutput();

  void on_readerr();

  void on_pushButton_navigate_clicked();

  void on_pushButton_grab_clicked();

private:
  Ui::four_wheel_bot *ui;

  QProcess *cmd;

  QSerialPort *serial;
};

#endif // FOUR_WHEEL_BOT_H
