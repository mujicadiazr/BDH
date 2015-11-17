#include "serializer.h"
#include <QDebug>

bool Serializer::serialize(ISerializable object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::HistorianVariableRequest object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::HistorianVariableResponse object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::RealtimeVariableRequest object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::RealtimeVariableResponse object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::VariablesConfigured object, QByteArray *retValue)
{
    m_buffer.clear();

    msgpack::pack(m_buffer, object);

    retValue->setRawData(m_buffer.data(),m_buffer.size());

    return retValue->size() > 0;
}

bool Serializer::serialize(AHP::HistorianAlarmRequest object, QByteArray *retValue) {
  m_buffer.clear();
  msgpack::pack(m_buffer, object);
  retValue->setRawData(m_buffer.data(),m_buffer.size());
  return retValue->size() > 0;
}

bool Serializer::serialize(AHP::HistorianAlarmResponse object, QByteArray *retValue) {
  m_buffer.clear();
  msgpack::pack(m_buffer, object);
  retValue->setRawData(m_buffer.data(),m_buffer.size());
  return retValue->size() > 0;
}

bool Serializer::deserialize(QByteArray &serializedObject,AHP::HistorianVariableRequest &object)
{
    m_buffer.clear();

    m_buffer.write(serializedObject.data(),serializedObject.size());

    msgpack::unpacked msg;
    msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());

    msgpack::object obj = msg.get();

    AHP::HistorianVariableRequest o2;

    try {
        obj.convert(&o2);
        object = o2;
    } catch (msgpack::type_error e) {
        qDebug() << "**DESERIALIZED FAIL** !!!\n";
        return false;
    }
//    qDebug() << "DESERIALIZED OK !!!\n";
//    qDebug() << "Data deserialized: "<<serializedObject<<"\n";
    return true;

}

bool Serializer::deserialize(QByteArray &serializedObject,
                             AHP::HistorianAlarmRequest &object) {
  m_buffer.clear();
  m_buffer.write(serializedObject.data(),serializedObject.size());
  msgpack::unpacked msg;
  msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());
  msgpack::object obj = msg.get();
  AHP::HistorianAlarmRequest o2;
  try {
      obj.convert(&o2);
      object = o2;
  } catch (msgpack::type_error e) {
      qDebug() << "**DESERIALIZED FAIL** !!!\n";
      return false;
  }
//    qDebug() << "DESERIALIZED OK !!!\n";
//    qDebug() << "Data deserialized: "<<serializedObject<<"\n";
  return true;
}

bool Serializer::deserialize(QByteArray &serializedObject, AHP::HistorianVariableResponse &object)
{
    m_buffer.clear();

    m_buffer.write(serializedObject.data(),serializedObject.size());

    msgpack::unpacked msg;
    msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());

    msgpack::object obj = msg.get();

    AHP::HistorianVariableResponse o2;

    try {
        obj.convert(&o2);
        object = o2;
    } catch (msgpack::type_error e) {
        qDebug() << "**DESERIALIZED FAIL** !!!\n";
        return false;
    }
//    qDebug() << "DESERIALIZED OK !!!\n";
    return true;
}

bool Serializer::deserialize(QByteArray &serializedObject,
                             AHP::HistorianAlarmResponse &object) {
  m_buffer.clear();
  m_buffer.write(serializedObject.data(),serializedObject.size());
  msgpack::unpacked msg;
  msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());
  msgpack::object obj = msg.get();
  AHP::HistorianAlarmResponse o2;
  try {
      obj.convert(&o2);
      object = o2;
  } catch (msgpack::type_error e) {
      qDebug() << "**DESERIALIZED FAIL** !!!\n";
      return false;
  }
//    qDebug() << "DESERIALIZED OK !!!\n";
  return true;
}

bool Serializer::deserialize(QByteArray &serializedObject, AHP::RealtimeVariableRequest &object)
{
    m_buffer.clear();

    m_buffer.write(serializedObject.data(),serializedObject.size());

    msgpack::unpacked msg;
    msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());

    msgpack::object obj = msg.get();

    AHP::RealtimeVariableRequest o2;

    try {
        obj.convert(&o2);
        object = o2;
    } catch (msgpack::type_error e) {
        qDebug() << "**DESERIALIZED FAIL** !!!\n";
        return false;
    }
//    qDebug() << "DESERIALIZED OK !!!\n";
    return true;
}

bool Serializer::deserialize(QByteArray &serializedObject, AHP::RealtimeVariableResponse &object)
{
    m_buffer.clear();

    m_buffer.write(serializedObject.data(),serializedObject.size());

    msgpack::unpacked msg;
    msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());

    msgpack::object obj = msg.get();

    AHP::RealtimeVariableResponse o2;

    try {
        obj.convert(&o2);
        object = o2;
    } catch (msgpack::type_error e) {
        qDebug() << "**DESERIALIZED FAIL** !!!\n";
        return false;
    }
//    qDebug() << "DESERIALIZED OK !!!\n";
    return true;
}

bool Serializer::deserialize(QByteArray &serializedObject, AHP::VariablesConfigured &object)
{
    m_buffer.clear();

    m_buffer.write(serializedObject.data(),serializedObject.size());

    msgpack::unpacked msg;
    msgpack::unpack(&msg, m_buffer.data(), m_buffer.size());

    msgpack::object obj = msg.get();

    AHP::VariablesConfigured o2;

    try {
        obj.convert(&o2);
        object = o2;
    } catch (msgpack::type_error e) {
        qDebug() << "**DESERIALIZED FAIL** !!!\n";
        return false;
    }
//    qDebug() << "DESERIALIZED OK !!!\n";
    return true;
}
