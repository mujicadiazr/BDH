#ifndef VARIABLESCONFIGURED_H
#define VARIABLESCONFIGURED_H

#include <vector>

#include "Domain/variable.h"
#include "Infrastructure/iserializable.h"

namespace AHP {

class VariablesConfigured : public ISerializable
{
    std::vector<Variable> m_vars;

public:
    VariablesConfigured(std::vector<Variable> vars);
    VariablesConfigured() = default;
    VariablesConfigured(const VariablesConfigured&) = default;
    ~VariablesConfigured() = default;

    std::vector<Variable> vars() const;
    void setVars(const std::vector<Variable> &vars);

    virtual bool serialize(Serializer *serializer, QByteArray *retValue);

    MSGPACK_DEFINE(m_vars)
};

}

#endif // VARIABLESCONFIGURED_H
