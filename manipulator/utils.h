#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);

    static Utils* instance();
    double getCurrentTime();

signals:

public slots:
};

#endif // UTILS_H
