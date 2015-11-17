#include "iserializable.h"
#include "serializer.h"
#include "serializer.h"

bool ISerializable::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}



