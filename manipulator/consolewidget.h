#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QPlainTextEdit>

class ConsoleWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    ConsoleWidget(QWidget *parent=0);

signals:
    void getNewCommand(const QString &cmd);

private:
    int historyIndex = 0;
    QStringList history;
    void handleReturnKeyPress();
    void echoErrorMsg(const QString &msg);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
};

#endif // CONSOLEWIDGET_H
