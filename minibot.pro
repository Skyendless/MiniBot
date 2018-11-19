#-------------------------------------------------
#
# Project created by QtCreator 2018-11-10T09:10:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minibot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./NewApi/include/#API
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    manipulator.cpp \
    inchworm_like.cpp \
    snake_like.cpp \
    two_wheel_bot.cpp \
    four_legged.cpp \
    biped.cpp \
    three_wheel_bot.cpp \
    four_wheel_bot.cpp \
    mobile_manipulator.cpp \
    snake.cpp \
    uisnakemove.cpp \
    NewApi/source/dev_operating.cpp \
    NewApi/source/group_bulk_read.cpp \
    NewApi/source/group_bulk_write.cpp \
    NewApi/source/group_sync_read.cpp \
    NewApi/source/group_sync_write.cpp \
    NewApi/source/packet_handler.cpp \
    NewApi/source/port_handler.cpp \
    NewApi/source/port_handler_arduino.cpp \
    NewApi/source/port_handler_linux.cpp \
    NewApi/source/port_handler_mac.cpp \
    NewApi/source/port_handler_windows.cpp \
    NewApi/source/protocol1_packet_handler.cpp

HEADERS += \
        mainwindow.h \
    manipulator.h \
    inchworm_like.h \
    snake_like.h \
    two_wheel_bot.h \
    four_legged.h \
    biped.h \
    three_wheel_bot.h \
    four_wheel_bot.h \
    mobile_manipulator.h \
    snake.h \
    uisnakemove.h \
    NewApi/include/dev_operating.h \
    NewApi/include/dynamixel_sdk.h \
    NewApi/include/group_bulk_read.h \
    NewApi/include/group_bulk_write.h \
    NewApi/include/group_sync_read.h \
    NewApi/include/group_sync_write.h \
    NewApi/include/packet_handler.h \
    NewApi/include/port_handler.h \
    NewApi/include/port_handler_arduino.h \
    NewApi/include/port_handler_linux.h \
    NewApi/include/port_handler_mac.h \
    NewApi/include/port_handler_windows.h \
    NewApi/include/protocol1_packet_handler.h

FORMS += \
        mainwindow.ui \
    manipulator.ui \
    inchworm_like.ui \
    snake_like.ui \
    two_wheel_bot.ui \
    four_legged.ui \
    three_wheel_bot.ui \
    four_wheel_bot.ui \
    mobile_manipulator.ui \
    uisnakemove.ui \
    biped.ui

RESOURCES += \
    pictures/res.qrc

DISTFILES +=
