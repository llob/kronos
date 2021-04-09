QT += network gui widgets svg

SOURCES += \
    $$PWD/abstractmodels/abstractissue.cpp \
    $$PWD/abstractmodels/abstractuser.cpp \
    $$PWD/abstractmodels/abstractworklog.cpp \
    $$PWD/abstractmodels/modelobject.cpp \
    $$PWD/authentication/authenticationstate.cpp \
    $$PWD/authentication/authenticationstateprivate.cpp \
    $$PWD/cache/issuecache.cpp \
    $$PWD/cache/issuecacheprivate.cpp \
    $$PWD/jira/jiraclient.cpp \
    $$PWD/jira/jiraissue.cpp \
    $$PWD/jira/jirauser.cpp \
    $$PWD/jira/jirautils.cpp \
    $$PWD/jira/jiraworklog.cpp \
    $$PWD/nagging/afternoonnag.cpp \
    $$PWD/nagging/morningnag.cpp \
    $$PWD/nagging/nag.cpp \
    $$PWD/nagging/nagengine.cpp \
    $$PWD/nagging/nagstate.cpp \
    $$PWD/nagging/randomnag.cpp \
    $$PWD/settings/settings.cpp \
    $$PWD/settings/settingsprivate.cpp \
    $$PWD/tray/trayiconcontroller.cpp \
    $$PWD/utils/colors.cpp \
    $$PWD/utils/monthlytotalcalculator.cpp \
    $$PWD/utils/recentissues.cpp \
    $$PWD/utils/totalcalculatorprivate.cpp \
    $$PWD/utils/weeklytotalcalculator.cpp \
    $$PWD/utils/totalcalculator.cpp \
    $$PWD/widgets/dailyworklogsmodel.cpp \
    $$PWD/worklogdialoglistmodel.cpp

HEADERS += \
    $$PWD/abstractmodels/abstractissue.h \
    $$PWD/abstractmodels/abstractuser.h \
    $$PWD/abstractmodels/abstractworklog.h \
    $$PWD/abstractmodels/modelobject.h \
    $$PWD/authentication/authenticationstate.h \
    $$PWD/authentication/authenticationstateprivate.h \
    $$PWD/cache/issuecache.h \
    $$PWD/cache/issuecacheprivate.h \
    $$PWD/jira/jiraclient.h \
    $$PWD/jira/jiraissue.h \
    $$PWD/jira/jirauser.h \
    $$PWD/jira/jirautils.h \
    $$PWD/jira/jiraworklog.h \
    $$PWD/nagging/afternoonnag.h \
    $$PWD/nagging/morningnag.h \
    $$PWD/nagging/nag.h \
    $$PWD/nagging/nagengine.h \
    $$PWD/nagging/nagstate.h \
    $$PWD/nagging/randomnag.h \
    $$PWD/settings/settings.h \
    $$PWD/settings/settingsprivate.h \
    $$PWD/tray/trayiconcontroller.h \
    $$PWD/utils/colors.h \
    $$PWD/utils/condition.h \
    $$PWD/utils/monthlytotalcalculator.h \
    $$PWD/utils/recentissues.h \
    $$PWD/utils/totalcalculatorprivate.h \
    $$PWD/utils/weeklytotalcalculator.h \
    $$PWD/utils/totalcalculator.h \
    $$PWD/widgets/dailyworklogsmodel.h \
    $$PWD/worklogdialoglistmodel.h

INCLUDEPATH += $$PWD

