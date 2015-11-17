#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <msgpack.hpp>

#include "Common/globals.h"

class Serializer;

class ISerializable
{
    bool cojon;
public:
    ISerializable() = default;
    ~ISerializable() = default;

    virtual bool serialize(Serializer *serializer,QByteArray *retValue);

    MSGPACK_DEFINE(cojon)
};

#endif // ISERIALIZABLE_H
