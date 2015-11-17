#ifndef REALTIMEVARIABLEREQUEST_H
#define REALTIMEVARIABLEREQUEST_H

#include "msgpack.hpp"

#include "variablerequest.h"

namespace AHP {

class RealtimeVariableRequest : public VariableRequest
{

public:
    RealtimeVariableRequest() = default;
    RealtimeVariableRequest(const RealtimeVariableRequest&) = default;
    ~RealtimeVariableRequest() = default;
    RealtimeVariableRequest(std::vector<quint32> varsId);

    virtual bool serialize(Serializer *serializer, QByteArray *retValue);

    MSGPACK_DEFINE(m_varsId)
};

}

#endif // REALTIMEVARIABLEREQUEST_H
