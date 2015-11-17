#include "alarm.h"


quint32 Alarm::idVariable() const
{
    return m_idVariable;
}

void Alarm::setIdVariable(const quint32 &idVariable)
{
    m_idVariable = idVariable;
}

quint32 Alarm::idAlarm() const
{
    return m_idAlarm;
}

void Alarm::setIdAlarm(const quint32 &idAlarm)
{
    m_idAlarm = idAlarm;
}

quint8 Alarm::alarmType() const
{
    return m_alarmType;
}

void Alarm::setAlarmType(const quint8 &alarmType)
{
    m_alarmType = alarmType;
}

quint8 Alarm::severity() const
{
    return m_severity;
}

void Alarm::setSeverity(const quint8 &severity)
{
    m_severity = severity;
}

quint8 Alarm::priority() const
{
    return m_priority;
}

void Alarm::setPriority(const quint8 &priority)
{
    m_priority = priority;
}

QString Alarm::description() const
{
    return m_description;
}

void Alarm::setDescription(const QString &description)
{
    m_description = description;
}
Alarm::Alarm()
{
    m_idVariable = 0;
    m_idAlarm = 0;
    m_alarmType = 0;
    m_priority = 0;
    m_severity = 0;
    m_description = QString();

}

Alarm::Alarm(quint32 idVariable, quint32 idAlarm, quint8 alarmType, quint8 severity, quint8 priority, QString description)
{
    m_idVariable = idVariable;
    m_idAlarm = idAlarm;
    m_alarmType = alarmType;
    m_priority = priority;
    m_severity = severity;
    m_description = description;
}
