#ifndef VARIABLEPOINT_H
#define VARIABLEPOINT_H

#include <QMetaType>
#include <QVariant>
#include <msgpack.hpp>

#include "../Common/globals.h"
#include "value_datatype_enum.h"

union Value {
    qlonglong x;
    qreal y;

    Value() = default;
    ~Value() = default;
    Value(const Value&) = default;

    Value(qlonglong x){this->x = x;}
    Value(qreal y){this->y = y;}

    Value operator=(qlonglong x){
        Value ret;
        ret.x = x;
        return ret;
    }

    Value operator=(qreal y){
        Value ret;
        ret.y = y;
        return ret;
    }

    MSGPACK_DEFINE(x,y)
};
Q_DECLARE_METATYPE(Value)

class VariablePoint
{
private:
    quint32 m_idVariable;   //! 4 bytes
    quint8 m_quality;       //! 1 byte
    quint64 m_timeStamp;    //! 8 bytes
    Value m_value;          //! 8 bytes
    quint8 m_dataType;      //! 1 byte

public:
    VariablePoint() = default;
    VariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, Value value, quint8 dataType);
    VariablePoint(const VariablePoint& ) = default;

    quint32 idVariable() const;
    void setIdVariable(const quint32 &idVariable);
    quint8 quality() const;
    void setQuality(const quint8 &quality);
    quint64 timeStamp() const;
    void setTimeStamp(const quint64 &timeStamp);
    quint8 dataType() const;
    void setDataType(const quint8 &dataType);

    QVariant value();

//    std::ostream& operator<<(std::ostream&, const VariablePoint&);

    MSGPACK_DEFINE(m_idVariable,m_quality,m_timeStamp,m_value,m_dataType)

};

Q_DECLARE_METATYPE(VariablePoint)

#endif // VARIABLEPOINT_H
