#include "manipulator.h"
#include "ui_manipulator.h"

#include "manipulator/kine.h"

#include <QSerialPortInfo>
#include <QDebug>

#define PI 3.14159265

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
  connect(ui->console_widget, &ConsoleWidget::getNewCommand, this, &manipulator::hanleNewCommand);
  connect(&m_controller, &ManipulatorController::jointStateUpdated, this, &manipulator::onJointStateUpated);
  connect(&m_controller, &ManipulatorController::poseStateUpdated, this, &manipulator::onPoseStateUpdated);
}

manipulator::~manipulator()
{
  delete ui;
}

void manipulator::onJointsButtonGroupClicked(QAbstractButton *button)
{
    qDebug() << button->objectName() << "clicked!";
    QString buttonName = button->objectName();
    double originalJoints[6];
    for (int i=0; i<6; i++) {
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

    if (exceedRange(m_commandJointState)) {
        for (int i=0; i<6; i++) {
            m_commandJointState[i] = originalJoints[i];
        }
        return;
    }

    m_controller.movj(m_commandJointState[0], m_commandJointState[1], m_commandJointState[2],
                      m_commandJointState[3], m_commandJointState[4], m_commandJointState[5], 0.1);

    _fkine(m_commandJointState, m_commandPoseState);
    updateCommandStateLabel();
}

void manipulator::onCartesianButtonGroupClicked(QAbstractButton *button)
{
    qDebug() << button->objectName() << "clicked!";
    QString buttonName = button->objectName();
    double originalPose[6];
    for (int i=0; i<6; i++) {
        originalPose[i] = m_commandPoseState[i];
    }

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

    if (!hasIkine(m_commandPoseState)) {
        for (int i=0; i<6; i++) {
            m_commandPoseState[i] = originalPose[i];
        }
        return;
    }

    _ikine(m_commandPoseState, m_commandJointState);

    m_controller.movp(m_commandPoseState[0], m_commandPoseState[1], m_commandPoseState[2],
                      m_commandPoseState[3], m_commandPoseState[4], m_commandPoseState[5], m_commandJointState[5], 0.1);

    updateCommandStateLabel();
}

void manipulator::onJointStateUpated(const JointState &state)
{
    ui->j1_state_label->setText(QString::number(state.jointAngles[0], 'f', 5));
    ui->j2_state_label->setText(QString::number(state.jointAngles[1], 'f', 5));
    ui->j3_state_label->setText(QString::number(state.jointAngles[2], 'f', 5));
    ui->j4_state_label->setText(QString::number(state.jointAngles[3], 'f', 5));
    ui->j5_state_label->setText(QString::number(state.jointAngles[4], 'f', 5));
    ui->j6_state_label->setText(QString::number(state.jointAngles[5], 'f', 5));
}

void manipulator::onPoseStateUpdated(const PoseState &state)
{
    ui->x_state_label->setText(QString::number(state.x, 'f', 5));
    ui->y_state_label->setText(QString::number(state.y, 'f', 5));
    ui->z_state_label->setText(QString::number(state.z, 'f', 5));
    ui->roll_state_label->setText(QString::number(state.roll, 'f', 5));
    ui->pitch_state_label->setText(QString::number(state.pitch, 'f', 5));
    ui->yaw_state_label->setText(QString::number(state.yaw, 'f', 5));
}

void manipulator::hanleNewCommand(const QString &cmd)
{
    qDebug() << "handle new command: " << cmd;
    QStringList cmdList = cmd.split(' ');
    if (cmdList.length() > 0) {
        if (cmdList[0] == "movj") {
            if (cmdList.length() == 7) {
                double originalJoints[6];
                for (int i=0; i<6; i++) {
                    originalJoints[i] = m_commandJointState[i];
                    m_commandJointState[i] = cmdList[i+1].toFloat();
                }
                if (exceedRange(m_commandJointState)) {
                    for (int i=0; i<6; i++) {
                        m_commandJointState[i] = originalJoints[i];
                    }
                    return;
                }
                m_controller.movj(m_commandJointState[0], m_commandJointState[1], m_commandJointState[2],
                                  m_commandJointState[3], m_commandJointState[4], m_commandJointState[5], 0.1);
            }
        } else if (cmdList[0] == "movp") {
            if (cmdList.length() == 8) {
                double originalPose[6];
                double originalTool = m_commandJointState[5];
                for (int i=0; i<6; i++) {
                    originalPose[i] = m_commandPoseState[i];
                    m_commandPoseState[i] = cmdList[i+1].toFloat();
                }
                m_commandJointState[5] = cmdList[7].toFloat();
                if (!hasIkine(m_commandPoseState)) {
                    for (int i=0; i<6; i++) {
                        m_commandPoseState[i] = originalPose[i];
                    }
                    m_commandJointState[5] = originalTool;
                    return;
                }

                m_controller.movp(m_commandPoseState[0], m_commandPoseState[1], m_commandPoseState[2],
                                  m_commandPoseState[3], m_commandPoseState[4], m_commandPoseState[5], m_commandJointState[5], 0.1);
            }
        } else if (cmdList[0] == "home") {
            m_controller.movj(0, 0, 0, 0, 0, 0, 0.05);
            for (int i=0; i<6; i++) {
                m_commandJointState[i] = 0;
            }
            _fkine(m_commandJointState, m_commandPoseState);
        }
        updateCommandStateLabel();
    }
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
    ui->record_button->setEnabled(true);
    ui->run_button->setEnabled(true);
    ui->console_widget->setEnabled(true);
    ui->program_text_edit->setEnabled(true);
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
    ui->record_button->setEnabled(false);
    ui->run_button->setEnabled(false);
    ui->console_widget->setEnabled(false);
    ui->program_text_edit->setEnabled(false);
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
}

bool manipulator::exceedRange(double joints[])
{
    for (int i=0; i<5; i++) {
        if (joints[i] < -PI/2 || joints[i] > PI/2 ) {
            return true;
        }
    }
    return false;
}

bool manipulator::hasIkine(double pose[])
{
    double j[5];
    return _ikine(pose, j);
}

bool manipulator::_ikine(double pose[], double joints[])
{
    float p[6], j[5];
    for (int i=0; i<6; i++) {
        p[i] = pose[i];
    }
    if (ikineSmart(p, j) > 0) {
        for (int i=0; i<5; i++) {
            joints[i] = j[i];
        }
        return true;
    }
    return false;
}

void manipulator::_fkine(double joints[], double pose[])
{
    float p[6], j[5];
    for (int i=0; i<5; i++) {
        j[i] = joints[i];
    }
    fkine(j, p);
    for (int i=0; i<6; i++) {
        pose[i] = p[i];
    }
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

void manipulator::on_quit_button_clicked()
{
    m_controller.disable();
    this->close();
}

void manipulator::on_record_button_clicked()
{
    QString cmd = QString("movj %1 %2 %3 %4 %5 %6\n").arg(m_commandJointState[0])
            .arg(m_commandJointState[1]).arg(m_commandJointState[2]).arg(m_commandJointState[3])
            .arg(m_commandJointState[4]).arg(m_commandJointState[5]);
    ui->program_text_edit->moveCursor(QTextCursor::End);
    ui->program_text_edit->insertPlainText(cmd);
}

void manipulator::on_run_button_clicked()
{
    for (QString cmd : ui->program_text_edit->toPlainText().split('\n')) {
        QStringList argList = cmd.split(' ');
        if (argList.length() >= 7 && argList[0] == "movj") {
            double rate = 0.3;
            if (argList.length() >= 8) {
                rate = argList[7].toFloat();
            }
            m_controller.movj(argList[1].toFloat(),
                              argList[2].toFloat(),
                              argList[3].toFloat(),
                              argList[4].toFloat(),
                              argList[5].toFloat(),
                              argList[6].toFloat(), rate);
            QThread::msleep(50);
        }
    }
}
