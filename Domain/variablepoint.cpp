#include "variablepoint.h"



VariablePoint::VariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, Value value, quint8 dataType)
{
    m_idVariable = idVariable;
    m_quality = quality;
    m_timeStamp = timeStamp;
    m_value = value;
    m_dataType = dataType;
}

quint32 VariablePoint::idVariable() const
{
    return m_idVariable;
}

void VariablePoint::setIdVariable(const quint32 &idVariable)
{
    m_idVariable = idVariable;
}

quint8 VariablePoint::quality() const
{
    return m_quality;
}

void VariablePoint::setQuality(const quint8 &quality)
{
    m_quality = quality;
}

quint64 VariablePoint::timeStamp() const
{
    return m_timeStamp;
}

quint8 VariablePoint::dataType() const
{
    return m_dataType;
}

void VariablePoint::setDataType(const quint8 &dataType)
{
    m_dataType = dataType;
}

void VariablePoint::setTimeStamp(const quint64 &timeStamp)
{
    m_timeStamp = timeStamp;
}

QVariant VariablePoint::value()
{
    if (m_dataType == (quint8)INTEGER )
        return QVariant(m_value.x);
    return QVariant(m_value.y);
}

//std::ostream VariablePoint::operator<<(std::ostream& out, const VariablePoint &vp)
//{
//       out << "ID: " << m_idVariable <<"\n"
//        << "Data type: " << m_dataType <<"\n"
//        << "Value: " << (m_dataType == INTEGER) ? m_value.x : m_value.y; <<"\n"
//        << "Timestamp: " << m_timeStamp <<"\n"
//        << "Quality: " << m_quality <<"\n";

//    return out;

//}
