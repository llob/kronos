#include <QtTest>
#include "utils/monthlytotalcalculator.h"

class MonthlyTotalCalculatorTest : public QObject
{
    Q_OBJECT

public:
    MonthlyTotalCalculatorTest();
    ~MonthlyTotalCalculatorTest();

private slots:
    void test_case1();

};

MonthlyTotalCalculatorTest::MonthlyTotalCalculatorTest()
{

}

MonthlyTotalCalculatorTest::~MonthlyTotalCalculatorTest()
{

}

void MonthlyTotalCalculatorTest::test_case1()
{
    MonthlyTotalCalculator mc;
}

QTEST_APPLESS_MAIN(MonthlyTotalCalculatorTest)

#include "tst_monthlytotalcalculatortest.moc"
