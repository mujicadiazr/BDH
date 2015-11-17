#include "variable.h"

Variable::Variable()
{
    m_id = 0;
    m_dataType = DOUBLE;
    m_sampleTime = 0;
    m_description = std::string();
}

Variable::Variable(quint32 id)
{
    m_id = id;
    m_dataType = DOUBLE;
    m_sampleTime = 0;
    m_description = std::string();
}

Variable::Variable(quint32 id, quint8 dataType, quint32 sampleTime, std::string description)
{
    m_id = id;
    m_dataType = dataType;
    m_sampleTime = sampleTime;
    m_description = description;
}

quint32 Variable::id() const
{
    return m_id;
}

void Variable::setId(const quint32 &id)
{
    m_id = id;
}

std::string Variable::description() const
{
    return m_description;
}

void Variable::setDescription(const std::string &description)
{
    m_description = description;
}

quint8 Variable::dataType() const
{
    return m_dataType;
}

void Variable::setDataType(const quint8 &dataType)
{
    m_dataType = dataType;
}

quint32 Variable::sampleTime() const
{
    return m_sampleTime;
}

void Variable::setSampleTime(const quint32 &sampleTime)
{
    m_sampleTime = sampleTime;
}



