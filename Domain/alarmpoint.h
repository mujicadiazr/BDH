#ifndef ALARMPOINT_H
#define ALARMPOINT_H

#include <QMetaType>

#include <msgpack.hpp>

#include "../Common/globals.h"
#include "../Domain/alarm_states_enum.h"

class AlarmPoint {
  quint32 m_idAlarm;
  AlarmState m_state;
  quint64 m_timeStamp;

 public:
  AlarmPoint() = default;
  AlarmPoint(quint32 idAlarm,AlarmState state, quint64 timeStamp);
  AlarmPoint(const AlarmPoint&) = default;

  quint32 idAlarm() const;
  void setIdAlarm(const quint32 &idAlarm);
  quint64 timeStamp() const;
  void setTimeStamp(const quint64 &timeStamp);
  AlarmState state() const;
  void setState(const AlarmState &state);

  MSGPACK_DEFINE(m_idAlarm, m_timeStamp)
};

Q_DECLARE_METATYPE(AlarmPoint)

#endif // ALARMPOINT_H
