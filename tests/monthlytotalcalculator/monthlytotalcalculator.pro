QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    tst_monthlytotalcalculatortest.cpp

include($$PWD/../../kronos.pri)
include($$PWD/../kronos-tests.pri)
