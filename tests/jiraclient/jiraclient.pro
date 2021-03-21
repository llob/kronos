QT += testlib network gui widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    $$PWD/tst_jiraclient.cpp

include($$PWD/../../time-registration.pri)
