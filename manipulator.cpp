#include "manipulator.h"
#include "ui_manipulator.h"

#include <QSerialPortInfo>
#include <QDebug>

manipulator::manipulator(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::manipulator)
{
  ui->setupUi(this);
  disableControls();

  for (QSerialPortInfo info : QSerialPortInfo::availablePorts()) {
      ui->serial_port_combo_box->addItem(info.portName());
  }

  connect(ui->joints_button_group, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &manipulator::onJointsButtonGroupClicked);
  connect(ui->cartesian_button_group, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &manipulator::onCartesianButtonGroupClicked);
}

manipulator::~manipulator()
{
  delete ui;
}

void manipulator::on_pushButton_clicked()
{
    this->close();
}

void manipulator::onJointsButtonGroupClicked(QAbstractButton *button)
{
    qDebug() << button->objectName() << "clicked!";
    QString buttonName = button->objectName();
    if (buttonName == "j1_d_button") {
        m_commandJointState[0] -= m_angleStep;
    } else if (buttonName == "j2_d_button") {
        m_commandJointState[1] -= m_angleStep;
    } else if (buttonName == "j3_d_button") {
        m_commandJointState[2] -= m_angleStep;
    } else if (buttonName == "j4_d_button") {
        m_commandJointState[3] -= m_angleStep;
    } else if (buttonName == "j5_d_button") {
        m_commandJointState[4] -= m_angleStep;
    } else if (buttonName == "j6_d_button") {
        m_commandJointState[5] -= m_angleStep;
    } else if (buttonName == "j1_i_button") {
        m_commandJointState[0] += m_angleStep;
    } else if (buttonName == "j2_i_button") {
        m_commandJointState[1] += m_angleStep;
    } else if (buttonName == "j3_i_button") {
        m_commandJointState[2] += m_angleStep;
    } else if (buttonName == "j4_i_button") {
        m_commandJointState[3] += m_angleStep;
    } else if (buttonName == "j5_i_button") {
        m_commandJointState[4] += m_angleStep;
    } else if (buttonName == "j6_i_button") {
        m_commandJointState[5] += m_angleStep;
    }

    updateCommandStateLabel();
}

void manipulator::onCartesianButtonGroupClicked(QAbstractButton *button)
{
    qDebug() << button->objectName() << "clicked!";
    QString buttonName = button->objectName();
    if (buttonName == "x_d_button") {
        m_commandPoseState[0] -= m_positionStep;
    } else if (buttonName == "y_d_button") {
        m_commandPoseState[1] -= m_positionStep;
    } else if (buttonName == "z_d_button") {
        m_commandPoseState[2] -= m_positionStep;
    } else if (buttonName == "roll_d_button") {
        m_commandPoseState[3] -= m_angleStep;
    } else if (buttonName == "pitch_d_button") {
        m_commandPoseState[4] -= m_angleStep;
    } else if (buttonName == "yaw_d_button") {
        m_commandPoseState[5] -= m_angleStep;
    } else if (buttonName == "x_i_button") {
        m_commandPoseState[0] += m_positionStep;
    } else if (buttonName == "y_i_button") {
        m_commandPoseState[1] += m_positionStep;
    } else if (buttonName == "z_i_button") {
        m_commandPoseState[2] += m_positionStep;
    } else if (buttonName == "roll_i_button") {
        m_commandPoseState[3] += m_angleStep;
    } else if (buttonName == "pitch_i_button") {
        m_commandPoseState[4] += m_angleStep;
    } else if (buttonName == "yaw_i_button") {
        m_commandPoseState[5] += m_angleStep;
    }

    updateCommandStateLabel();
}

void manipulator::updateCommandStateLabel()
{
    ui->j1_label->setText(QString::number(m_commandJointState[0], 'f', 5));
    ui->j2_label->setText(QString::number(m_commandJointState[1], 'f', 5));
    ui->j3_label->setText(QString::number(m_commandJointState[2], 'f', 5));
    ui->j4_label->setText(QString::number(m_commandJointState[3], 'f', 5));
    ui->j5_label->setText(QString::number(m_commandJointState[4], 'f', 5));
    ui->j6_label->setText(QString::number(m_commandJointState[5], 'f', 5));
    ui->x_label->setText(QString::number(m_commandPoseState[0], 'f', 5));
    ui->y_label->setText(QString::number(m_commandPoseState[1], 'f', 5));
    ui->z_label->setText(QString::number(m_commandPoseState[2], 'f', 5));
    ui->roll_label->setText(QString::number(m_commandPoseState[3], 'f', 5));
    ui->pitch_label->setText(QString::number(m_commandPoseState[4], 'f', 5));
    ui->yaw_label->setText(QString::number(m_commandPoseState[5], 'f', 5));
}

void manipulator::enableControls()
{
    for (QAbstractButton *button : ui->joints_button_group->buttons()) {
        button->setEnabled(true);
    }
    for (QAbstractButton *button: ui->cartesian_button_group->buttons()) {
        button->setEnabled(true);
    }
    ui->connect_button->setEnabled(false);
    ui->disconnect_button->setEnabled(true);
}

void manipulator::disableControls()
{
    for (QAbstractButton *button : ui->joints_button_group->buttons()) {
        button->setEnabled(false);
    }
    for (QAbstractButton *button: ui->cartesian_button_group->buttons()) {
        button->setEnabled(false);
    }
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
}

void manipulator::on_connect_button_clicked()
{
    QList<int> ids;
    for (QString id : ui->ids_line_edit->text().split(",")) {
        ids << id.toInt();
    }
    m_controller.enable(ui->serial_port_combo_box->currentText(), ids, ui->baudrate_line_edit->text().toInt());
    enableControls();
}

void manipulator::on_disconnect_button_clicked()
{
    m_controller.disable();
    disableControls();
}
