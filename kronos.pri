QT += network gui widgets

SOURCES += \
    $$PWD/abstractmodels/abstractissue.cpp \
    $$PWD/abstractmodels/abstractuser.cpp \
    $$PWD/abstractmodels/abstractworklog.cpp \
    $$PWD/abstractmodels/modelobject.cpp \
    $$PWD/authentication/authenticationstate.cpp \
    $$PWD/authentication/authenticationstateprivate.cpp \
    $$PWD/jira/jiraclient.cpp \
    $$PWD/jira/jiraissue.cpp \
    $$PWD/jira/jirauser.cpp \
    $$PWD/jira/jirautils.cpp \
    $$PWD/jira/jiraworklog.cpp \
    $$PWD/registrationdialoglistmodel.cpp \
    $$PWD/settings/settings.cpp \
    $$PWD/settings/settingsprivate.cpp \
    $$PWD/tray/trayiconcontroller.cpp \
    $$PWD/utils/colors.cpp \
    $$PWD/utils/monthlytotalcalculator.cpp \
    $$PWD/utils/recentissues.cpp \
    $$PWD/utils/totalcalculatorprivate.cpp \
    $$PWD/utils/weeklytotalcalculator.cpp \
    $$PWD/widgets/dailyregistrationsmodel.cpp \
    $$PWD/utils/totalcalculator.cpp

HEADERS += \
    $$PWD/abstractmodels/abstractissue.h \
    $$PWD/abstractmodels/abstractuser.h \
    $$PWD/abstractmodels/abstractworklog.h \
    $$PWD/abstractmodels/modelobject.h \
    $$PWD/authentication/authenticationstate.h \
    $$PWD/authentication/authenticationstateprivate.h \
    $$PWD/jira/jiraclient.h \
    $$PWD/jira/jiraissue.h \
    $$PWD/jira/jirauser.h \
    $$PWD/jira/jirautils.h \
    $$PWD/jira/jiraworklog.h \
    $$PWD/registrationdialoglistmodel.h \
    $$PWD/settings/settings.h \
    $$PWD/settings/settingsprivate.h \
    $$PWD/tray/trayiconcontroller.h \
    $$PWD/utils/colors.h \
    $$PWD/utils/condition.h \
    $$PWD/utils/monthlytotalcalculator.h \
    $$PWD/utils/recentissues.h \
    $$PWD/utils/totalcalculatorprivate.h \
    $$PWD/utils/weeklytotalcalculator.h \
    $$PWD/widgets/dailyregistrationsmodel.h \
    $$PWD/utils/totalcalculator.h

INCLUDEPATH += $$PWD

