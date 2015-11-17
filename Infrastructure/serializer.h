#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <msgpack.hpp>

#include "Common/globals.h"
#include "iserializable.h"
#include "Infrastructure/AHP/historianvariablerequest.h"
#include "Infrastructure/AHP/historianvariableresponse.h"
#include "Infrastructure/AHP/historianalarmresponse.h"
#include "Infrastructure/AHP/historianalarmrequest.h"
#include "Infrastructure/AHP/realtimevariablerequest.h"
#include "Infrastructure/AHP/realtimevariableresponse.h"
#include "Infrastructure/AHP/variablesconfigured.h"

class Serializer
{
    msgpack::sbuffer m_buffer;

public:
    Serializer() = default;
    ~Serializer() = default;

    bool serialize(ISerializable object, QByteArray *retValue);
    bool serialize(AHP::HistorianVariableRequest object, QByteArray *retValue);
    bool serialize(AHP::HistorianVariableResponse object, QByteArray *retValue);
    bool serialize(AHP::HistorianAlarmRequest object, QByteArray *retValue);
    bool serialize(AHP::HistorianAlarmResponse object, QByteArray *retValue);
    bool serialize(AHP::RealtimeVariableRequest object, QByteArray *retValue);
    bool serialize(AHP::RealtimeVariableResponse object, QByteArray *retValue);
    bool serialize(AHP::VariablesConfigured object, QByteArray *retValue);

    bool deserialize(QByteArray &serializedObject, AHP::HistorianVariableRequest &object);
    bool deserialize(QByteArray &serializedObject, AHP::HistorianVariableResponse &object);
    bool deserialize(QByteArray &serializedObject, AHP::HistorianAlarmRequest &object);
    bool deserialize(QByteArray &serializedObject, AHP::HistorianAlarmResponse &object);
    bool deserialize(QByteArray &serializedObject, AHP::RealtimeVariableRequest &object);
    bool deserialize(QByteArray &serializedObject, AHP::RealtimeVariableResponse &object);
    bool deserialize(QByteArray &serializedObject, AHP::VariablesConfigured &object);
};

#endif // SERIALIZER_H
