#ifndef DATAACQUISITION_H
#define DATAACQUISITION_H

#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QDateTime>

#include "Domain/alarm_states_enum.h"

class IntegerVariablePoint;
class AlarmPoint;
class VariablePoint;


class DataAcquisition : public QObject
{
    Q_OBJECT
public:
    explicit DataAcquisition(QObject *parent = 0);

    void run();

signals:
    void incommingVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);
    void incommingAlarmPoint(quint32 idAlarm, AlarmState state, quint64 timeStamp);

public slots:
    void emitVariablePoint();
    void emitAlarmPoint();

private:
    QTimer *m_vpTimer;
    QTimer *m_apTimer;

    quint32 m_vpIdCounter;
    QDateTime m_firstDateTime;

};

#endif // DATAACQUISITION_H
