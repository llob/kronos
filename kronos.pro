QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32-g++ {
   QMAKE_CXXFLAGS += -Werror
}
win32-msvc*{
   QMAKE_CXXFLAGS += /WX
}

RC_ICONS = kronos.ico

SOURCES +=  $$PWD/main.cpp \
            $$PWD/mainwindow.cpp \
            $$PWD/registrationdialog.cpp \
            $$PWD/deleteconfirmationdialog.cpp \
            $$PWD/maincontroller.cpp \
            $$PWD/registrationdialoglistvievitemdelegate.cpp \
            $$PWD/widgets/dailyregistrations.cpp

HEADERS +=  $$PWD/mainwindow.h \
            $$PWD/registrationdialog.h \
            $$PWD/deleteconfirmationdialog.h \
            $$PWD/maincontroller.h \
            $$PWD/registrationdialoglistvievitemdelegate.h \
            $$PWD/widgets/dailyregistrations.h


FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/registrationdialog.ui \
    $$PWD/deleteconfirmationdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(kronos.pri)

RESOURCES += \
    resources.qrc
