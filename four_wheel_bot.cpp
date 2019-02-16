#include "four_wheel_bot.h"
#include "ui_four_wheel_bot.h"

extern int bot_selet;
static QByteArray buff;
static const char * str_data;

four_wheel_bot::four_wheel_bot(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::four_wheel_bot)
{
  ui->setupUi(this);

  cmd = new QProcess(this);   //初始化
  connect(cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(on_readoutput()));
  connect(cmd,SIGNAL(readyReadStandardError()),this,SLOT(on_readerr()));

  serial = new QSerialPort(this);
  connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
}

four_wheel_bot::~four_wheel_bot()
{
  delete ui;
}

void four_wheel_bot::on_pushButton_clicked()
{
  cmd->close();
  serial->close();
  this->close();
}

/* search available serial ports
 *
 * foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
{
//   QSerialPort serial;
   serial->setPort(info);
   if(serial->open(QIODevice::ReadWrite))
   {
       ui->textEdit->append(serial->portName());
       serial->close();
   }
}*/

void four_wheel_bot::on_pushButton_remote_clicked()
{
  if(serial->isOpen()){
      serial->close();
      ui->label_sta->setText("Remote Disabled");
    }
  else{
      serial->setBaudRate(QSerialPort::Baud115200);
      serial->setPortName("/dev/ttyUSB0");
//      serial->setPortName("COM11");
      serial->setDataBits(QSerialPort::Data8);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
      serial->setFlowControl(QSerialPort::NoFlowControl);
      serial->open(QIODevice::ReadWrite);
      if(serial->isOpen()){
          ui->label_sta->setText("Remote Enabled");
        }
    }
}

void four_wheel_bot::on_pushButton_mapping_clicked()
{
  ui->textEdit->append("Resource Loading...Please Wait..");
  ui->label_sta->setText("Mapping");
  cmd->setWorkingDirectory("/home/huws/");
  cmd->start("bash");
  cmd->waitForStarted();
  cmd->write("source ~/catkin_ws/devel/setup.bash\n");
  cmd->write("roslaunch four_wheel robot_mapping.launch\n");
}

void four_wheel_bot::on_pushButton_navigate_clicked()
{
  QMessageBox::critical(this,tr("Sorry"),tr("Navigation function is still under development."));
}

void four_wheel_bot::on_readoutput()
{
  ui->textEdit->append(cmd->readAllStandardOutput().data());
}

void four_wheel_bot::on_readerr()
{
  ui->textEdit->append(cmd->readAllStandardError().data());
}

void four_wheel_bot::readData()
{
/* Read data */
//  QByteArray data = serial->readAll();
//  if(!data.isEmpty())
//    {
//      QString str = ui->textEdit->toPlainText();
//      str+=tr(data);
//      ui->textEdit->clear();
//      ui->textEdit->append(str);
//    }
//  data.clear();
}

void four_wheel_bot::handleError(QSerialPort::SerialPortError error)
{
  if(error == QSerialPort::ResourceError)
  {
      QMessageBox::critical(this,tr("Critical Error"),serial->errorString());
      serial->close();
  }
}

static int spd= 50;
static QString speed;
void four_wheel_bot::keyPressEvent(QKeyEvent *e)
{
  switch(e->key())
  {
    case Qt::Key_W:
      ui->pushButton_W->setStyleSheet("background-color:rgb(0,0,255)");
      speed=QString::number(spd);
      ui->label_speed->setText(speed);
      first_data();
      buff.append('W');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_S:
      ui->pushButton_S->setStyleSheet("background-color:rgb(0,0,255)");
      first_data();
      buff.append('S');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_A:
      ui->pushButton_A->setStyleSheet("background-color:rgb(0,0,255)");
      first_data();
      buff.append('A');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_D:
      ui->pushButton_D->setStyleSheet("background-color:rgb(0,0,255)");
      first_data();
      buff.append('D');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_I:
      ui->pushButton_I->setStyleSheet("background-color:rgb(0,0,255)");
      spd+=10;
      if(spd>100)
        {spd=100;}
      speed=QString::number(spd);
      ui->label_speed->setText(speed);
      first_data();
      buff.append('I');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_K:
      ui->pushButton_K->setStyleSheet("background-color:rgb(0,0,255)");
      spd-=10;
      if(spd<50)
        {spd=50;}
      speed=QString::number(spd);
      ui->label_speed->setText(speed);
      first_data();
      buff.append('K');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_J:
      ui->pushButton_J->setStyleSheet("background-color:rgb(0,0,255)");
      first_data();
      buff.append('J');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    case Qt::Key_L:
      ui->pushButton_L->setStyleSheet("background-color:rgb(0,0,255)");
      first_data();
      buff.append('L');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
    default:
      spd=50;
      speed=QString::number(spd);
      ui->label_speed->setText(speed);
      first_data();
      buff.append('G');
      str_data = buff.data();
      serial->write(str_data);
      qDebug()<<buff<<endl;
      buff.clear();
      break;
   }
}

void four_wheel_bot::keyReleaseEvent(QKeyEvent *e)
{
  switch(e->key())
  {
    case Qt::Key_W:
      ui->pushButton_W->setStyleSheet("background-color:rgb(255,255,255)");

      break;
    case Qt::Key_S:
      ui->pushButton_S->setStyleSheet("background-color:rgb(255,255,255)");

      break;
    case Qt::Key_A:
      ui->pushButton_A->setStyleSheet("background-color:rgb(255,255,255)");
      break;
    case Qt::Key_D:
      ui->pushButton_D->setStyleSheet("background-color:rgb(255,255,255)");
      break;

    case Qt::Key_I:
      ui->pushButton_I->setStyleSheet("background-color:rgb(255,255,255)");
      break;
    case Qt::Key_K:
      ui->pushButton_K->setStyleSheet("background-color:rgb(255,255,255)");
      break;
    case Qt::Key_J:
      ui->pushButton_J->setStyleSheet("background-color:rgb(255,255,255)");
      break;
    case Qt::Key_L:
      ui->pushButton_L->setStyleSheet("background-color:rgb(255,255,255)");
      break;
   }
}

void four_wheel_bot::first_data(void)
{
  buff = "zxc";
  if(bot_selet == 2)
   {
    buff.append('2');
   }else if(bot_selet == 3){
    buff.append('3');
  }else if(bot_selet == 4){
    buff.append('4');
  } else {
      buff.append('0');
  }
}


void four_wheel_bot::on_pushButton_grab_clicked()
{
    first_data();
    buff.append("M");
    serial->write(buff.data());
    qDebug()<<buff<<endl;
    buff.clear();
}
