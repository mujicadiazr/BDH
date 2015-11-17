#ifndef VARIABLEREQUEST_H
#define VARIABLEREQUEST_H

#include <vector>
#include "Common/globals.h"
#include "Infrastructure/iserializable.h"

namespace AHP { //Namespace AREX Historian Protocol

class VariableRequest : public ISerializable
{

protected:
    std::vector<quint32> m_varsId;
public:
    VariableRequest() = default;
    VariableRequest(const VariableRequest&) = default;
    ~VariableRequest() = default;

    VariableRequest(std::vector<quint32> varsId);

    std::vector<quint32> varsId() const;

    void clearIds();

    void setVarsId(const std::vector<quint32> &varsId);
    void appendIdVariable(quint32 variableId);

    MSGPACK_DEFINE(m_varsId)
};

}

#endif // VARIABLEREQUEST_H
