#ifndef MOBILE_MANIPULATOR_H
#define MOBILE_MANIPULATOR_H

#include <QDialog>

#include <QDebug>
#include <QtGui>
#include <QProcess>
#include <QMessageBox>

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
  class mobile_manipulator;
}

class mobile_manipulator : public QDialog
{
  Q_OBJECT

public:
  explicit mobile_manipulator(QWidget *parent = 0);

  void first_data(void);

  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);

  ~mobile_manipulator();

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
  Ui::mobile_manipulator *ui;

  QProcess *cmd;

  QSerialPort *serial;
};

#endif // MOBILE_MANIPULATOR_H
