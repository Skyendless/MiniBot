#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QDialog>

#include "manipulator/manipulatorcontroller.h"

class QAbstractButton;

namespace Ui {
    class manipulator;
}

class manipulator : public QDialog
{
    Q_OBJECT

public:
    explicit manipulator(QWidget *parent = 0);
    ~manipulator();

private slots:
    void on_pushButton_clicked();
    void onJointsButtonGroupClicked(QAbstractButton *button);
    void onCartesianButtonGroupClicked(QAbstractButton *button);

    void on_connect_button_clicked();

    void on_disconnect_button_clicked();

private:
    Ui::manipulator *ui;

    double m_commandPoseState[6] = {0, };
    double m_commandJointState[6] = {0, };
    double m_positionStep = 5;
    double m_angleStep = 0.1;

    ManipulatorController m_controller;

    void updateCommandStateLabel();
    void disableControls();
    void enableControls();
};

#endif // MANIPULATOR_H
