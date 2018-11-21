#include "consolewidget.h"
#include <QDebug>
#include <QRegExp>

ConsoleWidget::ConsoleWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    insertPlainText("> ");
}

void ConsoleWidget::handleReturnKeyPress()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 2);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    qDebug() << cursor.selectedText();
    QString cmd = cursor.selectedText();
    cmd = cmd.trimmed();

    if (cmd != "")
    {
        emit getNewCommand(cmd);
    }

    history.append(cmd);
    historyIndex = history.length();

    if (!textCursor().atEnd())
    {
        moveCursor(QTextCursor::EndOfLine);
    }
    insertPlainText("\n");
    insertPlainText("> ");
}

void ConsoleWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Return:
        handleReturnKeyPress();
        break;
    case Qt::Key_Backspace:
        if (textCursor().columnNumber() != 2)
            textCursor().deletePreviousChar();
        break;
    case Qt::Key_Left:
        if (textCursor().columnNumber() != 2)
            moveCursor(QTextCursor::Left);
        break;
    case Qt::Key_Right:
        moveCursor(QTextCursor::Right);
        break;
    case Qt::Key_Up:
        if (historyIndex > 0)
        {
            historyIndex--;
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::StartOfLine);
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 2);
            cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            insertPlainText(history[historyIndex]);
        }
        break;
    case Qt::Key_Down:
        if (historyIndex < history.length() - 1)
        {
            historyIndex++;
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::StartOfLine);
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 2);
            cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            insertPlainText(history[historyIndex]);
        }
        break;
    case Qt::Key_Home:
        moveCursor(QTextCursor::StartOfLine);
        moveCursor(QTextCursor::Right);
        moveCursor(QTextCursor::Right);
        break;
    case Qt::Key_End:
        moveCursor(QTextCursor::EndOfLine);
        break;
    default:
        insertPlainText(e->text());
        break;
    }
}

void ConsoleWidget::echoErrorMsg(const QString &msg)
{
    appendHtml("<span style=\"color:red;\">" + msg + "</span> ");
}
