#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T13:31:23
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GRecQt
TEMPLATE = app


win32{
    INCLUDEPATH += ChartDirector/include
    LIBS += -L../GRecQt/ChartDirector/lib/ -lchartdir60
    QMAKE_POST_LINK = copy /Y ..\\GRecQt\\ChartDirector\\lib\\chartdir60.dll $(DESTDIR)
}

unix{
    LIBS += -L../../lib -lchartdir
    QMAKE_RPATHDIR += ../../lib
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    recvthread.cpp \
    sendthread.cpp \
    qchartviewer.cpp \
    mainui.cpp \
    menuui.cpp \
    top_form.cpp \
    contentui.cpp \
    mainu1.cpp \
    mainwidget.cpp

HEADERS  += mainwindow.h \
    recvthread.h \
    sendthread.h \
    myhelper.h \
    cmi_pkt.h \
    rw29.h \
    spark_def.h \
    qchartviewer.h \
    mainui.h \
    menuui.h \
    headers.h \
    top_form.h \
    contentui.h \
    mainu1.h \
    mainwidget.h

FORMS    += mainwindow.ui \
    mainui.ui \
    menuui.ui \
    top_form.ui \
    contentui.ui \
    mainu1.ui \
    mainwidget.ui

RESOURCES += \
    icon.qrc

RC_FILE = icon.rc

include(qtsingleapplication/qtsingleapplication.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/quazip-0.7.3/lib/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/quazip-0.7.3/lib/debug/ -lquazipd
else:unix:!macx: LIBS += -L$$PWD/quazip-0.7.3/lib/release/ -lquazip

INCLUDEPATH += $$PWD/quazip-0.7.3/include
DEPENDPATH += $$PWD/quazip-0.7.3/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/quazip-0.7.3/lib/release/libquazip.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/quazip-0.7.3/lib/debug/libquazipd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/quazip-0.7.3/lib/release/quazip.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/quazip-0.7.3/lib/debug/quazipd.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/quazip-0.7.3/lib/release/libquazip.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/zlib128-dll/lib/ -lzdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/zlib128-dll/lib/ -lzdll
else:unix:!macx: LIBS += -L$$PWD/zlib128-dll/lib/ -lzdll

INCLUDEPATH += $$PWD/zlib128-dll/include
DEPENDPATH += $$PWD/zlib128-dll/include
