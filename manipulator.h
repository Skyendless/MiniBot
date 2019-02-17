#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QDialog>

#include "manipulator/manipulatorcontroller.h"

class QAbstractButton;
class QMessageBox;

namespace Ui {
    class manipulator;
}

class manipulator : public QDialog
{
    Q_OBJECT

public:
    explicit manipulator(QWidget *parent = nullptr);
    ~manipulator();

private slots:
    void onJointsButtonGroupClicked(QAbstractButton *button);
    void onJointsButtonGroupPressed(QAbstractButton *button);
    void onJointsButtonGroupReleased(QAbstractButton *button);

    void onCartesianButtonGroupClicked(QAbstractButton *button);
    void onCartesianButtonGroupPressed(QAbstractButton *button);
    void onCartesianButtonGroupReleased(QAbstractButton *button);

    void onJointStateUpated(const JointState &state);
    void onPoseStateUpdated(const PoseState &state);
    void hanleNewCommand(const QString &cmd);

    void on_connect_button_clicked();
    void on_disconnect_button_clicked();
    void on_quit_button_clicked();
    void on_record_button_clicked();
    void on_run_button_clicked();

private:
    Ui::manipulator *ui;

    double m_commandPoseState[6] = {0, };
    double m_commandJointState[6] = {0, };
    double m_positionStep = 1;
    double m_angleStep = 0.05;

    QAbstractButton *m_pressedButton;
    QTimer *m_jointsButtonGroupHoldTimer;
    QTimer *m_cartesianButtonGroupTimer;

    QMessageBox * m_noIkineMessageBox;
    QMessageBox * m_exceedRangeMessageBox;

    ManipulatorController m_controller;

    void updateCommandStateLabel();
    void disableControls();
    void enableControls();

    bool exceedRange(double joints[6]);
    bool hasIkine(double pose[6]);
    bool _ikine(double pose[6], double joints[5]);
    void _fkine(double joints[5], double pose[6]);
};

#endif // MANIPULATOR_H
