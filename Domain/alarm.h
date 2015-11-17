#ifndef ALARM_H
#define ALARM_H

#include "../Common/globals.h"

class Alarm
{
    quint32 m_idVariable;
    quint32 m_idAlarm;
    quint8 m_alarmType;
    quint8 m_severity;
    quint8 m_priority;
    QString m_description;

public:
    Alarm();
    Alarm(quint32 idVariable, quint32 idAlarm, quint8 alarmType, quint8 severity, quint8 priority, QString description);

    quint32 idVariable() const;
    void setIdVariable(const quint32 &idVariable);
    quint32 idAlarm() const;
    void setIdAlarm(const quint32 &idAlarm);
    quint8 alarmType() const;
    void setAlarmType(const quint8 &alarmType);
    quint8 severity() const;
    void setSeverity(const quint8 &severity);
    quint8 priority() const;
    void setPriority(const quint8 &priority);
    QString description() const;
    void setDescription(const QString &description);
};

#endif // ALARM_H
