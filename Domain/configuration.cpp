#include "configuration.h"

Configuration::Configuration()
{
    m_vars = new QList<Variable>();
    m_alarms = new QList<Alarm>();

    m_diskSize = 1024 * 1024 * 1024;                               //! 1 GB por default
    m_mainSize = 50 * 1024 * 1024;                                 //! 50 MB por default

    //! Por defecto creo 100 bloques: con esta estrategia gano en cuanto a perdida de informacion
    //! pero puedo perder en eficiencia pues tantos bloques pequennos no es efectivo
    m_blockSize = (m_diskSize - (m_mainSize + m_btempSize)) / 100;

}

Configuration::Configuration(QList<Variable> *vars, QList<Alarm> *alarms, quint64 diskSize, quint64 mainSize, quint64 blockSize)
{
    m_vars = vars;
    m_alarms = alarms;
    m_diskSize = diskSize;
    m_mainSize = mainSize;
    m_blockSize = blockSize;
}

Configuration::Configuration(QList<Variable> *vars, QList<Alarm> *alarms, quint64 diskSize)
{
    m_vars = vars;
    m_alarms = alarms;
    m_diskSize = diskSize;
    m_mainSize = 50 * 1024 * 1024;   //! 50 MB por default
    m_blockSize =9 * 1024 * 1024;    //! 9 MB por default
}

void Configuration::addVariable(Variable v)
{
    m_vars->append(v);
}

void Configuration::addAlarm(Alarm a)
{
    m_alarms->append(a);
}

QList<Variable> *Configuration::vars() const
{
    return m_vars;
}

void Configuration::setVars(QList<Variable> *vars)
{
    m_vars = vars;
}

QList<Alarm> *Configuration::alarms() const
{
    return m_alarms;
}

void Configuration::setAlarms(QList<Alarm> *alarms)
{
    m_alarms = alarms;
}

quint64 Configuration::diskSize() const
{
    return m_diskSize;
}

void Configuration::setDiskSize(const quint64 &diskSize)
{
    m_diskSize = diskSize;
    //! update the blockSize from diskSize
    m_blockSize = (m_diskSize - (m_mainSize + m_btempSize)) / 100;
}

quint64 Configuration::mainSize() const
{
    return m_mainSize;
}

void Configuration::setMainSize(const quint64 &mainSize)
{
    m_mainSize = mainSize;
}

quint64 Configuration::blockSize() const
{
    return m_blockSize;
}

void Configuration::setBlockSize(const quint64 &blockSize)
{
    m_blockSize = blockSize;
}

