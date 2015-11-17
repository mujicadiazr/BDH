#ifndef REALTIMEVARIABLERESPONSE_H
#define REALTIMEVARIABLERESPONSE_H

#include <msgpack.hpp>
#include <map>

#include "Common/globals.h"
#include "Domain/variablepoint.h"
#include "Infrastructure/iserializable.h"

namespace AHP {

class RealtimeVariableResponse : public ISerializable
{
    VariablePoint m_data;

public:
    RealtimeVariableResponse() = default;
    RealtimeVariableResponse(const RealtimeVariableResponse&) = default;
    ~RealtimeVariableResponse() = default;
    RealtimeVariableResponse(VariablePoint data);

    virtual bool serialize(Serializer *serializer, QByteArray *retValue);

    VariablePoint data() const;
    void setData(const VariablePoint &data);

    MSGPACK_DEFINE(m_data)
};

}

Q_DECLARE_METATYPE(AHP::RealtimeVariableResponse)

#endif // REALTIMEVARIABLERESPONSE_H
