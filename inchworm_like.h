#ifndef INCHWORM_LIKE_H
#define INCHWORM_LIKE_H

#include <QDialog>
#include "manipulator/jointstate.h"
#include "inchworm/inchwormcontroller.h"

class QAbstractButton;
class QMessageBox;

namespace Ui {
    class inchworm_like;
}

class inchworm_like : public QDialog
{
    Q_OBJECT

public:
    explicit inchworm_like(QWidget *parent = nullptr);
    ~inchworm_like();

private slots:
    void onJointsButtonGroupClicked(QAbstractButton *button);
    void onJointsButtonGroupPressed(QAbstractButton *button);
    void onJointsButtonGroupReleased(QAbstractButton *button);
    void onJointStateUpated(const JointState &state);

    void on_quit_button_clicked();
    void on_connect_button_clicked();
    void on_disconnect_button_clicked();
    void on_record_button_clicked();
    void on_run_button_clicked();
    void on_repeat_button_clicked();

private:
    Ui::inchworm_like *ui;
    InchwormController m_controller;

    double m_commandJointState[6] = {0, };
    double m_angleStep = 0.05;

    QAbstractButton *m_pressedButton;
    QTimer *m_jointsButtonGroupHoldTimer;

    QMessageBox * m_exceedRangeMessageBox;

    void updateCommandStateLabel();
    void disableControls();
    void enableControls();

    bool exceedRange(double joints[5]);
};

#endif // INCHWORM_LIKE_H
