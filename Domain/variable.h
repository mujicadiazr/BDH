#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

#include "../Common/globals.h"
#include "../Domain/value_datatype_enum.h"

#include "msgpack.hpp"

class Variable
{
    quint32 m_id;
    quint8 m_dataType;
    quint32 m_sampleTime;
    std::string m_description;
public:
    Variable();
    Variable(quint32 id);
    Variable(quint32 id, quint8 dataType, quint32 sampleTime, std::string description);

    quint32 id() const;
    void setId(const quint32 &id);
    quint8 dataType() const;
    void setDataType(const quint8 &dataType);
    quint32 sampleTime() const;
    void setSampleTime(const quint32 &sampleTime);
    std::string description() const;
    void setDescription(const std::string &description);

    bool operator==(const Variable& v1) {
        return this->id() == v1.id();
    }

    MSGPACK_DEFINE(m_id,m_dataType,m_sampleTime,m_description)


};

#endif // VARIABLE_H
