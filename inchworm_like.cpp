#include "inchworm_like.h"
#include "ui_inchworm_like.h"

#include <QDebug>
#include <QSerialPortInfo>

#define PI 3.14159265

inchworm_like::inchworm_like(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::inchworm_like)
{
  ui->setupUi(this);
  disableControls();

  for (QSerialPortInfo info : QSerialPortInfo::availablePorts()) {
#ifdef Q_OS_WIN
      ui->serial_port_combo_box->addItem(info.portName());
#else
      ui->serial_port_combo_box->addItem(info.systemLocation());
#endif
  }

  connect(ui->joints_button_group, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &inchworm_like::onJointsButtonGroupClicked);
  connect(&m_controller, &InchwormController::jointStateUpdated, this, &inchworm_like::onJointStateUpated);
}

inchworm_like::~inchworm_like()
{
  delete ui;
}

void inchworm_like::onJointsButtonGroupClicked(QAbstractButton *button)
{
    qDebug() << button->objectName() << "clicked!";
    QString buttonName = button->objectName();
    double originalJoints[5];
    for (int i=0; i<5; i++) {
        originalJoints[i] = m_commandJointState[i];
    }

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
    }

    if (exceedRange(m_commandJointState)) {
        for (int i=0; i<5; i++) {
            m_commandJointState[i] = originalJoints[i];
        }
        return;
    }

    m_controller.movj(m_commandJointState[0], m_commandJointState[1], m_commandJointState[2],
                      m_commandJointState[3], m_commandJointState[4], 0.1);

    updateCommandStateLabel();
}

void inchworm_like::onJointStateUpated(const JointState &state)
{
    ui->j1_state_label->setText(QString::number(state.jointAngles[0], 'f', 5));
    ui->j2_state_label->setText(QString::number(state.jointAngles[1], 'f', 5));
    ui->j3_state_label->setText(QString::number(state.jointAngles[2], 'f', 5));
    ui->j4_state_label->setText(QString::number(state.jointAngles[3], 'f', 5));
    ui->j5_state_label->setText(QString::number(state.jointAngles[4], 'f', 5));
}

void inchworm_like::updateCommandStateLabel()
{
    ui->j1_label->setText(QString::number(m_commandJointState[0], 'f', 5));
    ui->j2_label->setText(QString::number(m_commandJointState[1], 'f', 5));
    ui->j3_label->setText(QString::number(m_commandJointState[2], 'f', 5));
    ui->j4_label->setText(QString::number(m_commandJointState[3], 'f', 5));
    ui->j5_label->setText(QString::number(m_commandJointState[4], 'f', 5));
}

void inchworm_like::enableControls()
{
    for (QAbstractButton *button : ui->joints_button_group->buttons()) {
        button->setEnabled(true);
    }
    ui->record_button->setEnabled(true);
    ui->run_button->setEnabled(true);
    ui->program_text_edit->setEnabled(true);
    ui->connect_button->setEnabled(false);
    ui->disconnect_button->setEnabled(true);
}

void inchworm_like::disableControls()
{
    for (QAbstractButton *button : ui->joints_button_group->buttons()) {
        button->setEnabled(false);
    }
    ui->record_button->setEnabled(false);
    ui->run_button->setEnabled(false);
    ui->program_text_edit->setEnabled(false);
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
}

void inchworm_like::on_quit_button_clicked()
{
    m_controller.disable();
    this->close();
}

void inchworm_like::on_connect_button_clicked()
{
    QList<int> ids;
    for (QString id : ui->ids_line_edit->text().split(",")) {
        ids << id.toInt();
    }
    m_controller.enable(ui->serial_port_combo_box->currentText(), ids, ui->baudrate_line_edit->text().toInt());
    enableControls();
}

void inchworm_like::on_disconnect_button_clicked()
{
    m_controller.disable();
    disableControls();
}

void inchworm_like::on_record_button_clicked()
{
    QString cmd = QString("movj %1 %2 %3 %4 %5\n").arg(m_commandJointState[0])
            .arg(m_commandJointState[1]).arg(m_commandJointState[2]).arg(m_commandJointState[3])
            .arg(m_commandJointState[4]);
    ui->program_text_edit->moveCursor(QTextCursor::End);
    ui->program_text_edit->insertPlainText(cmd);
}

void inchworm_like::on_run_button_clicked()
{
    for (QString cmd : ui->program_text_edit->toPlainText().split('\n')) {
        QStringList argList = cmd.split(' ');
        if (argList.length() >= 6 && argList[0] == "movj") {
            double rate = 0.3;
            if (argList.length() >= 7) {
                rate = argList[6].toFloat();
            }
            m_controller.movj(argList[1].toFloat(),
                              argList[2].toFloat(),
                              argList[3].toFloat(),
                              argList[4].toFloat(),
                              argList[5].toFloat(), rate);
            QThread::msleep(50);
        }
    }
}

void inchworm_like::on_repeat_button_clicked()
{

}

bool inchworm_like::exceedRange(double joints[])
{
    for (int i=0; i<5; i++) {
        if (joints[i] < -PI/2 || joints[i] > PI/2 ) {
            return true;
        }
    }
    return false;
}
