#include "realtimevariableresponse.h"
#include "../serializer.h"


AHP::RealtimeVariableResponse::RealtimeVariableResponse(VariablePoint data)
    :m_data(data)
{
}

bool AHP::RealtimeVariableResponse::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}

VariablePoint AHP::RealtimeVariableResponse::data() const
{
    return m_data;
}

void AHP::RealtimeVariableResponse::setData(const VariablePoint &data)
{
    m_data = data;
}


