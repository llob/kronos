#include <QtTest>
#include <QCoreApplication>
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
    auto jc = QSharedPointer<JiraClient>(new JiraClient());
    QSignalSpy addWorklogFailedSpy(jc.data(), &JiraClient::addWorklogFailed);
    QSignalSpy deleteWorklogFailedSpy(jc.data(), &JiraClient::deleteWorklogFailed);
    QSignalSpy issueWorklogsFailedSpy(jc.data(), &JiraClient::issueWorklogsFailed);
    QSignalSpy searchFailedSpy(jc.data(), &JiraClient::searchFailed);

    jc->addWorklog(QSharedPointer<JiraWorklog>(new JiraWorklog()));
    jc->deleteWorklog(QSharedPointer<JiraWorklog>(new JiraWorklog()));

    addWorklogFailedSpy.wait();
    deleteWorklogFailedSpy.wait();
    issueWorklogsFailedSpy.wait();
    searchFailedSpy.wait();

}

QTEST_MAIN(jiraclient)

#include "tst_jiraclient.moc"
