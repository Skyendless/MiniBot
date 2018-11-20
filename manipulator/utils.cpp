#include "utils.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#endif
#include <time.h>

#include <QGlobalStatic>

Q_GLOBAL_STATIC(Utils, g_utils)

Utils::Utils(QObject *parent) : QObject(parent)
{
}

Utils * Utils::instance()
{
    return g_utils;
}

double Utils::getCurrentTime()
{
#if defined(Q_OS_WIN)
    LARGE_INTEGER freq_, counter_;
    QueryPerformanceCounter(&counter_);
    QueryPerformanceFrequency(&freq_);
    return (double)counter_.QuadPart / (double)freq_.QuadPart * 1000.0;
#elif defined(Q_OS_LINUX)
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    return ((double)tv.tv_sec * 1000.0 + (double)tv.tv_nsec * 0.001 * 0.001);
#else
    return 0.0;
#endif
}
