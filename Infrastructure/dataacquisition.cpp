#include <QTimer>
#include <ctime>


#include "dataacquisition.h"
#include "Common/globals.h"
#include "Domain/variablepoint.h"
#include "Domain/alarmpoint.h"

DataAcquisition::DataAcquisition(QObject *parent) :
    QObject(parent)
{
    qsrand(uint(std::time(NULL)));

    m_vpTimer = new QTimer(this),
    m_apTimer = new QTimer(this);

    m_vpIdCounter = 0;

    m_firstDateTime = QDateTime();

    connect(m_vpTimer,SIGNAL(timeout()),this,SLOT(emitVariablePoint()));
    connect(m_apTimer,SIGNAL(timeout()),this,SLOT(emitAlarmPoint()));

}

void DataAcquisition::run()
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    m_firstDateTime = QDateTime::currentDateTime();

    m_vpTimer->start(500);
    m_apTimer->start(100);
}

void DataAcquisition::emitVariablePoint()
{
    m_firstDateTime = m_firstDateTime.addSecs(60);

    emit incommingVariablePoint(m_vpIdCounter++, qrand() % 5, m_firstDateTime.toTime_t(), QVariant(qint64(qrand() % 6)));

    if (m_vpIdCounter == 9)
        m_vpIdCounter = 0;
}

void DataAcquisition::emitAlarmPoint()
{
    emit incommingAlarmPoint(qrand() % 150, static_cast<AlarmState>(qrand() % 2), QDateTime::currentDateTime().toTime_t());
}
