#include "settings.h"

#include <QApplication>
#include <QGlobalStatic>

Q_GLOBAL_STATIC(Settings, g_settings)

Settings::Settings(QObject *parent) : QObject(parent)
{
#if defined(Q_OS_WIN)
    m_settings = new QSettings(QApplication::applicationDirPath() + "/manipulator_settings.ini", QSettings::IniFormat, this);
#elif defined(Q_OS_LINUX)
    m_settings = new QSettings();
#endif
}

Settings * Settings::instance()
{
    return g_settings;
}
