#include "alarmpoint.h"

//AlarmPoint::AlarmPoint()
//{
//    m_idAlarm = 0;
//    m_timeStamp = 0;
//    m_state = ACTIVATED;
//}

AlarmPoint::AlarmPoint(quint32 idAlarm, AlarmState state, quint64 timeStamp)
{
    m_idAlarm = idAlarm;
    m_timeStamp = timeStamp;
    m_state = state;
}

quint32 AlarmPoint::idAlarm() const
{
    return m_idAlarm;
}

void AlarmPoint::setIdAlarm(const quint32 &idAlarm)
{
    m_idAlarm = idAlarm;
}

quint64 AlarmPoint::timeStamp() const
{
    return m_timeStamp;
}

void AlarmPoint::setTimeStamp(const quint64 &timeStamp)
{
    m_timeStamp = timeStamp;
}

AlarmState AlarmPoint::state() const
{
    return m_state;
}

void AlarmPoint::setState(const AlarmState &state)
{
    m_state = state;
}

