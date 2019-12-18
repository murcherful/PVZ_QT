#-------------------------------------------------
#
# Project created by QtCreator 2019-11-25T09:28:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PVZ
TEMPLATE = app

#INCLUDEPATH += /usr/local/include/opencv/
#INCLUDEPATH += /usr/local/include/opencv2/

#LIBS += /usr/local/lib/lib*

INCLUDEPATH += D:\setup\opencv\opencv\build\include
INCLUDEPATH += D:\setup\opencv\opencv\build\include\opencv\
INCLUDEPATH += D:\setup\opencv\opencv\build\include\opencv2\

#LIBS += D:\setup\opencv\opencv\build\x64\vc14\bin\opencv_world412.dll
#LIBS += D:\setup\opencv\opencv\build\x64\vc15\lib\opencv_world412.lib
#LIBS += D:\setup\opencv\opencv\build\x64\vc15\lib\opencv_world412d.lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mytool.cpp \
    mainloopthread.cpp \
    mypicture.cpp \
    myscene.cpp \
    mybutton.cpp \
    myobject.cpp

HEADERS += \
        mainwindow.h \
    mytool.h \
    mainloopthread.h \
    mypicture.h \
    myscene.h \
    mybutton.h \
    myobject.h

FORMS += \
        mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../setup/opencv/opencv/build/x64/vc15/lib/ -lopencv_world412
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../setup/opencv/opencv/build/x64/vc15/lib/ -lopencv_world412d
else:unix: LIBS += -L$$PWD/../../../setup/opencv/opencv/build/x64/vc15/lib/ -lopencv_world412

INCLUDEPATH += $$PWD/../../../setup/opencv/opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../setup/opencv/opencv/build/x64/vc15
