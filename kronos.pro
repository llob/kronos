QT       += core widgets svg

CONFIG += c++11

win32-g++ {
   QMAKE_CXXFLAGS += -Werror
}
win32-msvc*{
   QMAKE_CXXFLAGS += /WX
}

RC_ICONS = kronos.ico

SOURCES +=  $$PWD/main.cpp \
            $$PWD/mainwindow.cpp \
            $$PWD/worklogdialog.cpp \
            $$PWD/deleteconfirmationdialog.cpp \
            $$PWD/maincontroller.cpp \
            $$PWD/worklogdialoglistvievitemdelegate.cpp \
            $$PWD/widgets/dailyworklogs.cpp \
            $$PWD/widgets/kronoscalendarwidget.cpp

HEADERS +=  $$PWD/mainwindow.h \
            $$PWD/worklogdialog.h \
            $$PWD/deleteconfirmationdialog.h \
            $$PWD/maincontroller.h \
            $$PWD/worklogdialoglistvievitemdelegate.h \
            $$PWD/widgets/dailyworklogs.h \
            $$PWD/widgets/kronoscalendarwidget.h



FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/worklogdialog.ui \
    $$PWD/deleteconfirmationdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(kronos.pri)

RESOURCES += \
    resources.qrc
