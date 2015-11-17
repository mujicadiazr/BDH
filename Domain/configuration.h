#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../Common/globals.h"
#include "variable.h"
#include "alarm.h"

#include <QList>

#define m_btempSize 4 * 1024

class Configuration
{
    QList<Variable>* m_vars;
    QList<Alarm>* m_alarms;
    //Los size seran en Bytes
    quint64 m_diskSize;
    quint64 m_mainSize;
    quint64 m_blockSize;
public:
    Configuration();
    Configuration(QList<Variable>* vars, QList<Alarm>* alarms, quint64 diskSize, quint64 mainSize, quint64 blockSize);
    Configuration(QList<Variable>* vars, QList<Alarm>* alarms, quint64 diskSize);

    void addVariable(Variable v);
    void addAlarm(Alarm a);

    QList<Variable> *vars() const;
    void setVars(QList<Variable> *vars);
    QList<Alarm> *alarms() const;
    void setAlarms(QList<Alarm> *alarms);
    quint64 diskSize() const;
    void setDiskSize(const quint64 &diskSize);
    quint64 mainSize() const;
    void setMainSize(const quint64 &mainSize);
    quint64 blockSize() const;
    void setBlockSize(const quint64 &blockSize);
};

#endif // CONFIGURATION_H
