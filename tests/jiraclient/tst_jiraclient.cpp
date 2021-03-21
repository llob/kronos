#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "jira/jiraclient.h"

class jiraclient : public QObject
{
    Q_OBJECT

public:
    jiraclient();
    ~jiraclient();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

jiraclient::jiraclient()
{

}

jiraclient::~jiraclient()
{

}

void jiraclient::initTestCase()
{

}

void jiraclient::cleanupTestCase()
{

}

void jiraclient::test_case1()
{
    auto jc = new JiraClient();
    jc->registrationsByDate(QDate(2021, 3, 10));
    QSignalSpy spy(jc, &JiraClient::registrationsByDateFinished);
    spy.wait();
}

QTEST_MAIN(jiraclient)

#include "tst_jiraclient.moc"
