#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T09:54:22
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport
QT       += texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

#TARGET = work_time_agat
TARGET = Time_Agat
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    cworktime_main.cpp \
    csavetofile.cpp \
    form_settings.cpp \
    form_statistics.cpp \
    ../qcustomplot/qcustomplot.cpp

HEADERS  += widget.h \
    cworktime_main.h \
    csavetofile.h \
    form_settings.h \
    form_statistics.h \
    ../qcustomplot/qcustomplot.h

FORMS    += widget.ui \
    form_settings.ui \
    form_statistics.ui

CONFIG += mobility
MOBILITY = 

