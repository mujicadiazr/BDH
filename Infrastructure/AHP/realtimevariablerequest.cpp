#include "realtimevariablerequest.h"
#include "../serializer.h"

AHP::RealtimeVariableRequest::RealtimeVariableRequest(std::vector<quint32> varsId)
    :VariableRequest(varsId)
{
}

bool AHP::RealtimeVariableRequest::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}
